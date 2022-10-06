#include "DYRun.h"
#include "Core/ProjectSerializer.h"

namespace Daqromancy {

	//Maps two uint32_t's to a single uint32_t
	static uint32_t SzudzikPairingFunction(uint32_t v1, uint32_t v2)
	{
		return v1 >= v2 ? (v1 * v1 + v1 + v2) : (v2 * v2 + v1);
	}

	DYRun::DYRun() :
		m_isRunning(false), m_processingThread(nullptr)
	{
	}

	DYRun::~DYRun()
	{
		StopRun();
	}

	void DYRun::InitFiles(const DYProject::Ref& project)
	{
		std::filesystem::path runPath = project->CreateRunDirectory();

		//Write a run local copy of the daq settings
		ProjectSerializer serializer(runPath / "settings.yaml");
		serializer.SerializeData(project);

		const std::vector<DigitizerArgs>& argList = project->GetDigitizerArgsList();

		std::string filename;
		uint32_t key;
		for (const auto& args : argList)
		{
			for (int i = 0; i < args.channels; i++)
			{
				filename = args.name + "_CH_" + std::to_string(i) + ".dybin";
				key = SzudzikPairingFunction(args.handle, i);
				m_files.insert_or_assign(key, DYFile(runPath / filename));
				if (!(m_files[key].IsOpen()))
				{
					DY_WARN("File named {0} could not be opened!", filename);
				}
			}
		}
	}

	void DYRun::StartRun(const DYProject::Ref& project)
	{
		if (m_isRunning)
			StopRun();

		//m_ringConsumerID = RingBuffer::Attach();
		m_dataHandle = DataDistributor::Connect();
		InitFiles(project);
		m_isRunning = true;

		m_processingThread = new std::thread(&DYRun::ProcessData, std::ref(*this));
	}

	void DYRun::StopRun()
	{
		if (m_dataHandle.dataQueue == nullptr)
			return;
		else if(!m_dataHandle.dataQueue->IsEmpty())
		{
			DY_INFO("Finishing writing data to file before stopping... Still have {0} events to process", m_dataHandle.dataQueue->Size());
			while (!m_dataHandle.dataQueue->IsEmpty())
			{
				continue;
			}
		}

		m_isRunning = false;
		m_dataHandle.dataQueue->ForceWakeup();
		if (m_processingThread != nullptr && m_processingThread->joinable())
		{
			m_processingThread->join();
			delete m_processingThread;
			m_processingThread = nullptr;
			DataDistributor::Disconnect(m_dataHandle);

			//Close all files and clear the map to be ready for re-use
			for (auto& fileIter : m_files)
				fileIter.second.Close();
			m_files.clear();
		}
		else if(m_processingThread != nullptr)
			DY_WARN("Unable to destroy the processing thread for file writing!");
		m_dataHandle.dataQueue->ResetWakeup();
	}

	void DYRun::ProcessData()
	{
		std::vector<DYData> dataBuffer;
		uint32_t key;
		while (m_isRunning)
		{
			m_dataHandle.dataQueue->Wait(); //Block for the queue to be full
			if (!m_dataHandle.dataQueue->IsEmpty()) //Protect for edge cases
			{
				dataBuffer = m_dataHandle.dataQueue->Front();
				for (auto& datum : dataBuffer)
				{
					key = SzudzikPairingFunction(datum.board, datum.channel);
					m_files[key].Write(datum);
				}
				m_dataHandle.dataQueue->PopFront();
			}
		}
	}
}
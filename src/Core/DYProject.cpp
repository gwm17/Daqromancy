#include "DYProject.h"

namespace Daqromancy {

	DYProject::DYProject() :
		m_runNumber(0), m_dppMode(DPPAcqMode::List)
	{
	}

	DYProject::~DYProject() {}

	void DYProject::InternalClear()
	{
		m_argList.clear();
		m_boardParamList.clear();
		m_psdChannelMap.clear();
		m_phaChannelMap.clear();
		m_psdWaveMap.clear();
		m_phaWaveMap.clear();
	}

	bool DYProject::SetProjectPath(const std::filesystem::path& path)
	{
		bool status = true;
		m_projectPath = path.lexically_normal();
		if (!std::filesystem::exists(m_projectPath))
		{
			status = std::filesystem::create_directory(path);
			if (!status)
				m_projectPath.clear();
		}
		return status;
	}

	//void DYProject::SetDigitizerData(const std::vector<Digitizer::Ref>& chain)
	void DYProject::SetDigitizerData(const DigitizerChain& chain)
	{
		InternalClear();

		//buffers
		std::vector<PSDParameters> psdChannels;
		std::vector<PHAParameters> phaChannels;
		PSDWaveParameters psdWaves;
		PHAWaveParameters phaWaves;

		for (auto& digitizer : chain)
		{
			auto& args = digitizer->GetDigitizerArgs();
			m_argList.push_back(args);
			m_boardParamList.push_back(digitizer->GetDigitizerParameters());
			if (args.firmware == CAEN_DGTZ_DPPFirmware_PHA)
			{
				digitizer->GetChannelParameters(phaChannels);
				digitizer->GetWaveformParameters(phaWaves);
				m_phaChannelMap[args.handle] = phaChannels;
				m_phaWaveMap[args.handle] = phaWaves;
			}
			else if(args.firmware == CAEN_DGTZ_DPPFirmware_PSD)
			{
				digitizer->GetChannelParameters(psdChannels);
				digitizer->GetWaveformParameters(psdWaves);
				m_psdChannelMap[args.handle] = psdChannels;
				m_psdWaveMap[args.handle] = psdWaves;
			}
			else
			{
				DY_ERROR("Invalid digitizer firmware detected at DYProject::SetDigitizerData!");
			}
		}
	}

	void DYProject::SetDigitizerArgsList(const std::vector<DigitizerArgs>& args)
	{
		m_argList = args;
	}

	void DYProject::SetDigitizerParameterList(const std::vector<DigitizerParameters>& params)
	{
		m_boardParamList = params;
	}

	void DYProject::SetDigitizerParameters(int handle, const DigitizerParameters& params)
	{
		if (handle >= m_boardParamList.size() || handle == -1)
		{
			DY_ERROR("Attempting to set digitizer parameters for non-extant board: given handle {0}, number of boards {1}", handle, m_boardParamList.size());
			return;
		}

		m_boardParamList[handle] = params;
	}

	void DYProject::SetPHAParameters(int handle, const std::vector<PHAParameters>& params)
	{
		m_phaChannelMap[handle] = params;
	}

	void DYProject::SetPSDParameters(int handle, const std::vector<PSDParameters>& params)
	{
		m_psdChannelMap[handle] = params;
	}

	void DYProject::SetPHAWaveParameters(int handle, const PHAWaveParameters& params)
	{
		m_phaWaveMap[handle] = params;
	}

	void DYProject::SetPSDWaveParameters(int handle, const PSDWaveParameters& params)
	{
		m_psdWaveMap[handle] = params;
	}

	void DYProject::SetRunNumber(uint32_t number)
	{
		m_runNumber = number;
	}

	void DYProject::IncrementRunNumber()
	{
		++m_runNumber;
	}

	void DYProject::SetDPPAcqMode(DPPAcqMode mode)
	{
		m_dppMode = mode;
	}

	const std::filesystem::path& DYProject::GetProjectPath()
	{
		return m_projectPath;
	}

	const std::vector<DigitizerArgs>& DYProject::GetDigitizerArgsList()
	{
		return m_argList;
	}

	DigitizerArgs DYProject::GetDigitizerArgs(int handle)
	{
		if (handle >= m_argList.size() || handle == -1)
		{
			DY_ERROR("Attempting to get digitizer args for non-extant board: given handle {0}, number of boards {1}", handle, m_argList.size());
			return DigitizerArgs();
		}
		return m_argList[handle];
	}

	const std::vector<DigitizerParameters>& DYProject::GetDigitizerParameterList()
	{
		return m_boardParamList;
	}

	DigitizerParameters DYProject::GetDigitizerParameters(int handle)
	{
		if (handle >= m_boardParamList.size() || handle == -1)
		{
			DY_ERROR("Attempting to get digitizer parameters for non-extant board: given handle {0}, number of boards {1}", handle, m_boardParamList.size());
			return DigitizerParameters();
		}
		return m_boardParamList[handle];
	}

	const std::vector<PHAParameters>& DYProject::GetPHAParameters(int handle)
	{
		auto iter = m_phaChannelMap.find(handle);
		if (iter != m_phaChannelMap.end())
			return iter->second;
		DY_ERROR("Invalid handle {0} given to DYProject::GetPHAParameters!", handle);
		return m_nullphaChannels;
	}
	
	const std::vector<PSDParameters>& DYProject::GetPSDParameters(int handle)
	{
		auto iter = m_psdChannelMap.find(handle);
		if (iter != m_psdChannelMap.end())
			return iter->second;
		DY_ERROR("Invalid handle {0} given to DYProject::GetPHAParameters!", handle);
		return m_nullpsdChannels;
	}

	PHAWaveParameters DYProject::GetPHAWaveParameters(int handle)
	{
		auto iter = m_phaWaveMap.find(handle);
		if (iter != m_phaWaveMap.end())
			return iter->second;
		DY_ERROR("Invalid handle {0} given to DYProject::GetPHAParameters!", handle);
		return PHAWaveParameters();
	}

	PSDWaveParameters DYProject::GetPSDWaveParameters(int handle)
	{
		auto iter = m_psdWaveMap.find(handle);
		if (iter != m_psdWaveMap.end())
			return iter->second;
		DY_ERROR("Invalid handle {0} given to DYProject::GetPHAParameters!", handle);
		return PSDWaveParameters();
	}

	uint32_t DYProject::GetRunNumber()
	{
		return m_runNumber;
	}

	DPPAcqMode DYProject::GetDPPAcqMode()
	{
		return m_dppMode;
	}

	size_t DYProject::GetNumberOfBoards()
	{
		return m_argList.size();
	}


	std::filesystem::path DYProject::CreateRunDirectory()
	{
		std::string runName = "run_" + std::to_string(m_runNumber);
		std::filesystem::path runPath = m_projectPath / runName;

		if (std::filesystem::exists(runPath))
		{
			DY_WARN("Run directory {0} already exists! Could lead to overwritting data!", runPath);
		}
		else
		{
			std::filesystem::create_directory(runPath);
		}

		return runPath;
	}
}
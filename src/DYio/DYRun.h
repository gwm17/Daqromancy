#ifndef DY_RUN_H
#define DY_RUN_H

#include "DYFile.h"
#include "Core/DYProject.h"
#include "Core/DataDistributor.h"

#include <atomic>
#include <thread>

namespace Daqromancy {

	class DYRun
	{
	public:
		DYRun();
		~DYRun();

		void StartRun(const DYProject::Ref& project);
		void StopRun();
		bool IsRunning() const { return m_isRunning; }

	private:
		void InitFiles(const DYProject::Ref& project);
		void ProcessData();
		
		std::unordered_map<uint32_t, DYFile> m_files;
		DistributorClient m_dataHandle;
		std::atomic<bool> m_isRunning;
		std::thread* m_processingThread;
	};
}

#endif
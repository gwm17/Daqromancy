#ifndef DY_FILE_H
#define DY_FILE_H

#include <filesystem>
#include "DAQ/DigitizerDefs.h"

namespace Daqromancy {

	class DYFile
	{
	public:
		DYFile();
		DYFile(const std::filesystem::path& path);
		~DYFile();

		void Open(const std::filesystem::path& path);
		void Write(const DYData& data);
		void Close();
		bool IsOpen() const { return m_fileHandle == nullptr ? false : m_fileHandle->is_open(); }
		const std::filesystem::path& GetPath() const { return m_path; }

	private:
		void Flush();

		std::vector<char> m_buffer;
		std::shared_ptr<std::ofstream> m_fileHandle;
		std::filesystem::path m_path;
		std::size_t m_currentLocation;

		static constexpr uint64_t s_bufferSize = Data::dataSize * 100; //Store 100 hits before flushing? Idk

	};
}

#endif
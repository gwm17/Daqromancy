#include "DYFile.h"
#include "DYListData.h"
#include <bit>

namespace Daqromancy {

	DYFile::DYFile() :
		m_fileHandle(nullptr), m_currentLocation(0)
	{
		if (std::endian::native == std::endian::big)
			
			("You are using a big endian system! Data will be encoded in reverse byte order on most machines");
		m_buffer.resize(s_bufferSize);
	}

	DYFile::DYFile(const std::filesystem::path& path) :
		m_fileHandle(nullptr), m_currentLocation(0)
	{
		if (std::endian::native == std::endian::big)
			DY_WARN("You are using a big endian system! Data will be encoded in reverse byte order on most machines");
		m_buffer.resize(s_bufferSize);
		Open(path);
	}

	DYFile::~DYFile()
	{
		if (IsOpen())
			Close();
	}

	void DYFile::Open(const std::filesystem::path& path)
	{
		m_path = path;
		m_fileHandle = std::make_shared<std::ofstream>(path);
	}

	void DYFile::Close()
	{
		if (m_currentLocation != 0)
			Flush();
		m_fileHandle->close();
	}

	void DYFile::Flush()
	{
		m_fileHandle->write(m_buffer.data(), m_currentLocation);
		m_currentLocation = 0;
	}

	//See BSListData.h for details on loading data to buffer. 
	void DYFile::Write(const DYData& data)
	{
		m_currentLocation = LoadDYDataToBuffer(m_buffer, data);
		if (m_currentLocation == s_bufferSize)
			Flush();
	}
}
#ifndef PROJECT_SERIALIZER_H
#define PROJECT_SERIALIZER_H

#include "DYProject.h"

#include <filesystem>

namespace Daqromancy {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(const std::filesystem::path& path);
		~ProjectSerializer();
		
		void SerializeData(const DYProject::Ref& project);
		void DeserializeData(const DYProject::Ref& project);

	private:
		std::filesystem::path m_filepath;

	};
}

#endif
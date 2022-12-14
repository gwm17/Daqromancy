#include "FileDialog.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Daqromancy {

	//Helper function to handle file size printing
	std::string ConvertFileSystemSizeToString(std::uintmax_t value)
	{
		int i = 0;
		double mantissa = (double)value;
		for (; mantissa >= 1024.0; ++i)
			mantissa /= 1024.0;
		mantissa = std::ceil(mantissa * 10.0) / 10.0;
		return std::to_string(int(mantissa)) + "BKMGTPE"[i];
	}

	FileDialog::FileDialog() :
		m_currentPath(std::filesystem::current_path()), m_type(Type::None), m_selectedItem(""), m_openDialogFlag(false)
	{
		table_flags = ImGuiTableFlags_BordersH | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_RowBg;
		select_flags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups;
	}

	FileDialog::~FileDialog() {}

	//Each type of action has its own render function

	std::pair<std::string, FileDialog::Type> FileDialog::RenderFileDialog(const std::string& ext)
	{
		if (m_openDialogFlag)
		{
			m_selectedItem = "";
			m_openDialogFlag = false;
			m_currentPath = std::filesystem::current_path();
			ImGui::OpenPopup("File Dialog");
		}

		//Open & Save Dir are equivalent operations, but can be useful to specify for handling later
		std::string result = "";
		if (ImGui::BeginPopupModal("File Dialog"))
		{
			switch (m_type)
			{
			case Type::OpenFile:
			{
				result = ImGuiRenderOpenFile(ext);
				break;
			}
			case Type::SaveFile:
			{
				result = ImGuiRenderSaveFile(ext);
				break;
			}
			case Type::OpenDir:
			{
				result = ImGuiRenderOpenDir();
				break;
			}
			case Type::SaveDir:
			{
				result = ImGuiRenderOpenDir();
				break;
			}
			case Type::None: break;
			}
			ImGui::EndPopup();
		}

		return std::make_pair(result, m_type);
	}

	std::string FileDialog::ImGuiRenderOpenFile(const std::string& ext)
	{
		std::string result = "";
		std::string text = "";

		ImGui::Text("%s", ("Current Directory: " + m_currentPath.lexically_normal().string()).c_str());
		ImGui::SameLine();
		ImGui::Text("%s", ("Extension Filter: " + ext).c_str());
		ImGui::InputText("Selected", &m_selectedItem);
		if (ImGui::Button("Ok"))
		{
			std::filesystem::path filepath = m_currentPath / m_selectedItem;
			result = filepath.string();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		if (ImGui::BeginTable("File System", 2, table_flags, ImVec2(-1, -1)))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Size");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable(" ..", false, select_flags))
			{
				m_selectedItem.clear();
				m_currentPath.append("..");
			}
			ImGui::TableNextColumn();
			ImGui::Text("N/A");
			for (auto& entry : std::filesystem::directory_iterator(m_currentPath, std::filesystem::directory_options::skip_permission_denied))
			{
				if (entry.is_directory())
				{
					try //Windows C:/ errors can only be try caught.
					{
						text = std::filesystem::relative(entry.path(), m_currentPath).string();
					}
					catch (std::exception& e)
					{
						continue;
					}
					ImGui::TableNextRow();
					
					ImGui::TableNextColumn();
					if (ImGui::Selectable(text.c_str(), false, select_flags))
					{
						m_selectedItem.clear();
						m_currentPath /= entry.path();
					}
					ImGui::TableNextColumn();
					ImGui::Text("N/A");
				}
				else if (entry.path().filename().extension() == ext)
				{
					ImGui::TableNextRow();
					text = entry.path().filename().string();
					ImGui::TableNextColumn();
					if (ImGui::Selectable(text.c_str(), false, select_flags))
						m_selectedItem = entry.path().filename().string();
					ImGui::TableNextColumn();
					ImGui::Text("%s", ConvertFileSystemSizeToString(entry.file_size()).c_str());
				}
			}
			ImGui::EndTable();
		}
		return result;
	}

	std::string FileDialog::ImGuiRenderSaveFile(const std::string& ext)
	{
		std::string result = "";
		std::string text = "";

		ImGui::Text("%s", ("Current Directory: " + m_currentPath.lexically_normal().string()).c_str());
		ImGui::SameLine();
		ImGui::Text("%s", ("Extension Filter: " + ext).c_str());
		ImGui::InputText("Selected", &m_selectedItem);
		if (ImGui::Button("Ok"))
		{
			std::filesystem::path filepath = m_currentPath / m_selectedItem;
			result = filepath.string();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		if (ImGui::BeginTable("File System", 2, table_flags, ImVec2(-1, -1)))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Size");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable(" ..", false, select_flags))
			{
				m_selectedItem.clear();
				m_currentPath.append("..");
			}
			ImGui::TableNextColumn();
			ImGui::Text("N/A");
			for (auto& entry : std::filesystem::directory_iterator(m_currentPath, std::filesystem::directory_options::skip_permission_denied))
			{
				if (entry.is_directory())
				{
					try //Windows C:/ errors can only be try caught.
					{
						text = std::filesystem::relative(entry.path(), m_currentPath).string();
					}
					catch (std::exception& e)
					{
						continue;
					}
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					if (ImGui::Selectable(text.c_str(), false, select_flags))
					{
						m_selectedItem.clear();
						m_currentPath /= entry.path();
					}
					ImGui::TableNextColumn();
					ImGui::Text("N/A");
				}
				else if (entry.path().filename().extension() == ext)
				{
					ImGui::TableNextRow();
					text = entry.path().filename().string();
					ImGui::TableNextColumn();
					if (ImGui::Selectable(text.c_str(), false, select_flags))
						m_selectedItem = entry.path().filename().string();
					ImGui::TableNextColumn();
					ImGui::Text("%s", ConvertFileSystemSizeToString(entry.file_size()).c_str());
				}
			}
			ImGui::EndTable();
		}
		return result;
	}

	std::string FileDialog::ImGuiRenderOpenDir()
	{
		std::string result = "";
		std::string text = "";

		ImGui::Text("%s", ("Current Directory: " + m_currentPath.lexically_normal().string()).c_str());
		ImGui::InputText("Selected", &m_selectedItem);
		if (ImGui::Button("Ok"))
		{
			result = m_selectedItem;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();

		if (ImGui::BeginTable("File System", 2, table_flags, ImVec2(-1, -1)))
		{
			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Size");
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable(" ..", false, select_flags))
			{
				m_currentPath.append("..");
				m_selectedItem = m_currentPath.string();
			}
			ImGui::TableNextColumn();
			ImGui::Text("N/A");
			for (auto& entry : std::filesystem::directory_iterator(m_currentPath, std::filesystem::directory_options::skip_permission_denied))
			{
				ImGui::TableNextRow();
				if (entry.is_directory())
				{
					try //Windows C:/ errors can only be try caught.
					{
						text = std::filesystem::relative(entry.path(), m_currentPath).string();
					}
					catch (std::exception& e)
					{
						continue;
					}
					ImGui::TableNextColumn();
					if (ImGui::Selectable(text.c_str(), false, select_flags))
					{
						m_currentPath /= entry.path();
						m_selectedItem = m_currentPath.string();
					}
					ImGui::TableNextColumn();
					ImGui::Text("N/A");
				}
				else
				{
					text = entry.path().filename().string();
					ImGui::TableNextColumn();
					ImGui::Text("%s", text.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", ConvertFileSystemSizeToString(entry.file_size()).c_str());
				}
			}
			ImGui::EndTable();
		}
		return result;
	}

}
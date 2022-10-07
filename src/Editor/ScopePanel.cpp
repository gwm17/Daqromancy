#include "ScopePanel.h"

#include "DAQ/DigitizerDefs.h"

#include "imgui.h"
#include "implot.h"

namespace Daqromancy {

	ScopePanel::ScopePanel(const DYProject::Ref& project) :
		m_dataReady(false), m_selectedBoard(-1), m_selectedChannel(-1), m_selectedSamplingPeriod(0.0), m_project(project)
	{
		m_dataHandle = DataDistributor::Connect();
	}

	ScopePanel::~ScopePanel()
	{
		DataDistributor::Disconnect(m_dataHandle);
	}

	void ScopePanel::OnUpdate()
	{
		if (!m_dataHandle.dataQueue->IsEmpty())
		{
			m_buffer = m_dataHandle.dataQueue->Front();
			m_dataHandle.dataQueue->PopFront();
			for (auto& hit : m_buffer)
			{
				if (hit.board == m_selectedBoard && hit.channel == m_selectedChannel)
				{
					m_selectedHit = hit;
					m_selectedSamplingPeriod = GetSamplingPeriod(m_project->GetDigitizerArgs(m_selectedBoard).model);
					break;
				}
			}
		}
	}

	void ScopePanel::OnImGuiRender()
	{
		static std::string selectedBoardString = "";
		static std::string selectedChannelString = "";
		static int availChannels = -1;
		static std::string analog1 = "##analog1";
		static std::string analog2 = "##analog2";
		static std::string digital1 = "##digital1";
		static std::string digital2 = "##digital2";

		if (ImGui::Begin("Oscilloscope"))
		{
			if (ImGui::BeginCombo("Board", selectedBoardString.c_str()))
			{
				for (const auto& boardArgs : m_project->GetDigitizerArgsList())
				{
					if (ImGui::Selectable(boardArgs.name.c_str(), boardArgs.handle == m_selectedBoard))
					{
						m_selectedBoard = boardArgs.handle;
						availChannels = boardArgs.channels;
						selectedBoardString = boardArgs.name;
						if (boardArgs.firmware == CAEN_DGTZ_DPPFirmware_PHA)
						{
							const auto& phaParams = m_project->GetPHAWaveParameters(boardArgs.handle);
							analog1 = PHAVirtualProbe1ToString(phaParams.analogProbe1);
							analog2 = PHAVirtualProbe2ToString(phaParams.analogProbe2);
							digital1 = PHADigitalProbeToString(phaParams.digitalProbe1);
							digital2 = "Trigger";
						}
						else if (boardArgs.firmware == CAEN_DGTZ_DPPFirmware_PSD)
						{
							const auto& psdParams = m_project->GetPSDWaveParameters(boardArgs.handle);
							analog1 = PSDVirtualProbe1ToString(psdParams.analogProbe1);
							analog2 = PSDVirtualProbe2ToString(psdParams.analogProbe2);
							digital1 = PSDDigitalProbe1ToString(psdParams.digitalProbe1);
							digital2 = PSDDigitalProbe2ToString(psdParams.digitalProbe2);
						}
					}
				}
				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Channel", selectedChannelString.c_str()))
			{
				for (int channel = 0; channel < availChannels; channel++)
				{
					if (ImGui::Selectable(fmt::format("{0}", channel).c_str(), channel == m_selectedChannel))
					{
						m_selectedChannel = channel;
						selectedChannelString = fmt::format("{0}", m_selectedChannel);
					}
				}
				ImGui::EndCombo();
			}
			if (ImPlot::BeginPlot("ScopeView", ImVec2(-1,-1)))
			{
				if (m_selectedHit.waveSize != 0)
				{
					ImPlot::PlotLine(analog1.c_str(), (ImU16*)m_selectedHit.trace1Samples.data(), m_selectedHit.trace1Samples.size(), m_selectedSamplingPeriod);
					ImPlot::PlotLine(analog2.c_str(), (ImU16*)m_selectedHit.trace2Samples.data(), m_selectedHit.trace2Samples.size(), m_selectedSamplingPeriod);
					ImPlot::PlotLine(digital1.c_str(), (ImU8*)m_selectedHit.digitalTrace1Samples.data(), m_selectedHit.digitalTrace1Samples.size(), m_selectedSamplingPeriod);
					ImPlot::PlotLine(digital2.c_str(), (ImU8*)m_selectedHit.digitalTrace2Samples.data(), m_selectedHit.digitalTrace2Samples.size(), m_selectedSamplingPeriod);
				}
				ImPlot::EndPlot();
			}
		}
		ImGui::End();
	}
}
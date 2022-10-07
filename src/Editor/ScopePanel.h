#ifndef SCOPE_PANEL_H
#define SCOPE_PANEL_H

#include "DAQ/DigitizerDefs.h"
#include "Core/DataDistributor.h"
#include "Core/DYProject.h"

namespace Daqromancy {

	class ScopePanel
	{
	public:
		ScopePanel(const DYProject::Ref& project);
		~ScopePanel();

		void OnUpdate();
		void OnImGuiRender();

	private:
		DistributorClient m_dataHandle;
		std::vector<DYData> m_buffer; //Buffered data retrieved from ring
		DYData m_selectedHit; //Hit associated with selected board/channel

		bool m_dataReady;
		int m_selectedBoard;
		int m_selectedChannel;
		double m_selectedSamplingPeriod;


		DYProject::Ref m_project;
	};
}

#endif
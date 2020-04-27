#ifndef __STATUSPANEL
#define __STATUSPANEL

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "DynamicArrays.h"
#include "ProgressPanel.h"
#include "StatsgenLog.h"

class StatusPanel : public wxPanel
{
	public:
		StatusPanel();
		~StatusPanel();
		void EventUpdating(bool value);
		bool GetEventUpdating();
		void Reset();
		void WakeUp();
		long CurrentValue();
		void OnResize(wxSizeEvent &event);
		static wxString PanelName(int windowID);
		void NormaliseWidths();
		void ChoosePanel(int windowID);
		void UpdateLabel(const char *label);
		void Finalise();
		void CreatePanels();
		void UpdateColouredBar();
		void Update(long value);
		void SetOffset(long value);
		void EnableTimeToGo();
		void DisableTimeToGo();
		void SetStatus(wxString &statusText);
		void SetStatus(const char *statusText);
		void Initiate(long topValue,
				const char *units,
				long scale,
				const char *rateUnits,
				long rateScale);
		void LogError(wxString &errorText,
				int severity=SeverityNotRun);
		void ClearErrors();
		void SetDynamicPanel(ProgressPanel *panelIn);
		ProgressPanel *WhichPanel();
		void DisableUpdate();
		bool StageRan(int windowID);
		bool RecoveryMode();
		void StoreProgress(wxString &filename);
		void SetClone(ProgressPanel *panelIn);
		void UpdateClone(ProgressPanel *clonePanel);
	protected:

	public:
		int		currentWindowID;
	private:
		wxArrayInt	panelIDs;	
		ArrayOfPointers	panelPtrs;
		ProgressPanel	*dynamicPanel;
		ProgressPanel	*clonePanel;
		StatsgenLog	logger;
		bool		updateDisabled;

		DECLARE_EVENT_TABLE()
};

#endif

#ifndef __SCHEDULEEDITORPANEL
#define __SCHEDULEEDITORPANEL

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"

class ScheduleEditorPanel : public wxPanel
{
	public:
		ScheduleEditorPanel(wxString &valueIn,
					bool intervalWithBaseIn);
		virtual ~ScheduleEditorPanel();

		wxString GetValue();
		void OnResize(wxSizeEvent &event);
	protected:

	private:
		void SetValue();
		void CreatePanel();
		void SplitValue(
				wxString &baseHourStr,
				wxString &baseMinuteStr,
				wxString &baseSecondStr,
				wxString &intervalHourStr,
				wxString &intervalMinuteStr,
				wxString &intervalSecondStr);

		void ResizeTime(int startX,int startY,
				wxStaticText *intervalLabel,
				wxTextCtrl *hour,
				wxTextCtrl *minute,
				wxTextCtrl *second,
				int *width,
				int *height);

		wxTextCtrl		*baseHours;
		wxTextCtrl		*baseMinutes;
		wxTextCtrl		*baseSeconds;
		wxTextCtrl		*intervalHours;
		wxTextCtrl		*intervalMinutes;
		wxTextCtrl		*intervalSeconds;
		wxStaticText		*baseLabel;
		wxStaticText		*intervalLabel;
		bool			intervalWithBase;
		wxString		value;

		DECLARE_EVENT_TABLE()
};

#endif

#ifndef __PRIORITYPANEL
#define __PRIORITYPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/slider.h>

// Statsgen Includes
#include "WindowIDs.h"

class PriorityPanel : public wxPanel
{
	public:
		PriorityPanel();
		void CreateScreen();
		void OnSliderChange(wxScrollEvent &event);
		void OnResize(wxSizeEvent &event);
	protected:
		wxSlider		*fullRunPriority;

	private:
		int columnToSort;
		wxString	configKey;
		wxString	labelText;
		wxStaticText	fullLabel;
		DECLARE_EVENT_TABLE()
};

#endif

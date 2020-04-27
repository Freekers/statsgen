#ifndef __ERRORPANEL
#define __ERRORPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>

// Statsgen Includes
#include "WindowIDs.h"

class ErrorPanel : public wxPanel
{
	public:
		ErrorPanel(wxArrayString &errorsIn,
			wxArrayInt &severitiesIn);
		void CreateScreen();
		void OnResize(wxSizeEvent &event);

	protected:
		wxListCtrl		*errorDisplay;

	private:
		void Update();
		wxArrayString	errors;
		wxArrayInt	severities;
		wxImageList	imageList;
		DECLARE_EVENT_TABLE()
};

#endif

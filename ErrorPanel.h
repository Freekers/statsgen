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

	protected:
		wxListCtrl		*mErrorDisplay;

	private:
		void Update();
		wxArrayString	mErrors;
		wxArrayInt	mSeverities;
		wxImageList	mImageList;
		wxBoxSizer	*mMainSizer;
};

#endif

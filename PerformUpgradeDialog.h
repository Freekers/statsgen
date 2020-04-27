#ifndef __PERFORMUPGRADEDIALOG
#define __PERFORMUPGRADEDIALOG

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "ProgressPanel.h"

class PerformUpgradeDialog : public wxDialog
{
	public:
		PerformUpgradeDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~PerformUpgradeDialog();

		void OnQuit(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnResize(wxSizeEvent& event);
		void CreateDialog();

		bool DisplayDialog();

	protected:

		wxButton	quitButton;
		wxButton	saveButton;
		ProgressPanel	*progressPanel;
		wxListCtrl	*upgradeList;
		wxArrayString	upgradeFiles;
	private:
		DECLARE_EVENT_TABLE()
};

#endif

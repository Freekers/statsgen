#ifndef __WEBSITECONFIGGUI
#define __WEBSITECONFIGGUI

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"

class WebsiteConfigGUI : public wxDialog
{
	public:
		WebsiteConfigGUI(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~WebsiteConfigGUI();

		void OnQuit(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);

		void DisplayDialog();

	protected:
		void CreateDialog();
		void ConfigureSizer();

	private:
		//GroupedConfigItemsPanel	*configItems;
		ArrayOfPointers		configItems;
		wxButton	saveButton;
		wxButton	quitButton;
		wxBoxSizer	*mMainSizer;
		wxBoxSizer	*mControlsSizer;
		DECLARE_EVENT_TABLE()
};

#endif

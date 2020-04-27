#ifndef __GENERICCONFIGGUI
#define __GENERICCONFIGGUI

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"
#include "GenericConfigPanel.h"

class GenericConfigGUI : public wxDialog
{
	public:
		GenericConfigGUI(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~GenericConfigGUI();

		void OnQuit(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);

		void DisplayDialog();
		void SetConfigPanel(GenericConfigPanel *configPanel);

	protected:
		void ConfigureSizer();
		void CreateDialog();

	private:
		GenericConfigPanel	*configPanel;
		wxButton	saveButton;
		wxButton	quitButton;
		wxBoxSizer	*mMainSizer;
		wxBoxSizer	*mControlsSizer;
		DECLARE_EVENT_TABLE()
};

#endif

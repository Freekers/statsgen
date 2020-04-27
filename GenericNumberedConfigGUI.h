#ifndef __GENERICNUMBEREDCONFIGGUI
#define __GENERICNUMBEREDCONFIGGUI

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"
#include "GenericConfigPanel.h"

class GenericNumberedConfigGUI : public wxDialog
{
	public:
		GenericNumberedConfigGUI(
			wxPanel * (*newPanelFunctionIn)(wxWindow *parentID,wxString &listID),
			//GenericConfigPanel * (*newPanelFunctionIn)(wxWindow *parentID,wxString &listID),
				wxString &group,
				wxString &listGroupPrefix,
				wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~GenericNumberedConfigGUI();

		void OnQuit(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnNew(wxCommandEvent& event);
		void OnDelete(wxCommandEvent& event);
		void OnListItemSelected(wxCommandEvent &event);

		void DisplayDialog();
		//void SetConfigPanel(GenericConfigPanel *configPanel);
		void SetConfigPanel(wxPanel *configPanel);

	protected:
		void CreateDialog();
		void ConfigureSizer();
	private:
		void WriteIDList();
		wxComboBox		mIDList;
		//GenericConfigPanel * (*newPanelFunction)(wxWindow *parentID,wxString &listID);
		wxPanel * (*mNewPanelFunction)(wxWindow *parentID,wxString &listID);
		//GenericConfigPanel	*configPanel;
		wxPanel		*mConfigPanel;
		wxButton	mSaveButton;
		wxButton	mQuitButton;
		wxButton	mNewButton;
		wxButton	mDeleteButton;
		wxString	mListGroup;
		wxString	mListGroupPrefix;

		wxBoxSizer	*mMainSizer;
		wxBoxSizer	*mControlsSizer;
		DECLARE_EVENT_TABLE()
};

#endif

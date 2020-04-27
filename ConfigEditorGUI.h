#ifndef __CONFIGEDITORGUI
#define __CONFIGEDITORGUI

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BoxedDropDown.h"
#include "TextConfigItemGUI.h"
#include "GroupedConfigItemsPanel.h"
#include "ImagePanel.h"

class ConfigEditorGUI : public wxDialog
{
	public:
		ConfigEditorGUI(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				BoxedDropDown *filter1,
				BoxedDropDown *filter2,
				GroupedConfigItemsPanel *panelIn,
				bool drawImages);
		virtual ~ConfigEditorGUI();

		void OnQuit(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnListItemSelected(wxListEvent& event);
		void OnResize(wxSizeEvent& event);
		void OnTextChange(wxCommandEvent& event);
		void OnFilterChanged(wxCommandEvent& event);

		void AddConfigKey(
				wxString &group,
				wxString &configKey,
				bool useRealName);
		void DisplayDialog();

	protected:
		void CreateDialog();

		wxArrayString	configKeys;
		wxArrayString	configBaseKeys;
		wxArrayString	configDisplayNames;
		wxListCtrl	configItems;
		//wxTextCtrl	valueText;
		TextConfigItemGUI	valueText;
		wxButton	saveButton;
		wxButton	quitButton;
		BoxedDropDown	*filter1;
		BoxedDropDown	*filter2;
		GroupedConfigItemsPanel		*configPanel;
		bool		drawImages;
		ImagePanel	*imagePanel;
		int		imagePanelWidth;
		int		imagePanelHeight;
		int		imagePanelX;
		int		imagePanelY;

	private:
		void PopulateConfigItems();

		wxString	lastSelectedKey;

		DECLARE_EVENT_TABLE()
};

#endif

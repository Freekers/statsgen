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
		void OnTextChange(wxCommandEvent& event);
		void OnFilterChanged(wxCommandEvent& event);

		void AddConfigKey(
				wxString &group,
				wxString &configKey,
				bool useRealName);
		void DisplayDialog();

	protected:
		void ConfigureSizer();
		void CreateDialog();

		wxArrayString			mConfigKeys;
		wxArrayString			mConfigBaseKeys;
		wxArrayString			mConfigDisplayNames;
		wxListCtrl				mConfigItems;
		TextConfigItemGUI		mValueText;
		wxButton				mSaveButton;
		wxButton				mQuitButton;
		BoxedDropDown			*mFilter1;
		BoxedDropDown			*mFilter2;
		GroupedConfigItemsPanel	*mConfigPanel;
		bool					mDrawImages;
		ImagePanel				*mImagePanel;
		wxBoxSizer				*mMainSizer;

	private:
		void PopulateConfigItems();

		wxString	lastSelectedKey;

		DECLARE_EVENT_TABLE()
};

#endif

#ifndef __SELECTIONCONFIGITEMGUI
#define __SELECTIONCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class SelectionConfigItemGUI : public BaseConfigItemGUI
{
	public:
		SelectionConfigItemGUI();
		virtual ~SelectionConfigItemGUI();
		void CreateDisplay(wxWindow *parent,int id,wxString labelText);
		void OnTextChange(wxCommandEvent& event);
		void SetSelection(wxArrayString &codes, wxArrayString &names);
		virtual void ApplyConfigKeyChange();

		wxString GetValue();
		wxString GetNameFromCode(wxString code);
	protected:

	private:
		wxComboBox		*mTextEdit;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		wxString		mDefaultValue;
		wxArrayString		mCodes;
		wxArrayString		mNames;

		DECLARE_EVENT_TABLE()
};

#endif

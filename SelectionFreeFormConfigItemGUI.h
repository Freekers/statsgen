#ifndef __SELECTIONFREEFORMCONFIGITEMGUI
#define __SELECTIONFREEFORMCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class SelectionFreeFormConfigItemGUI : public BaseConfigItemGUI
{
	public:
		SelectionFreeFormConfigItemGUI();
		virtual ~SelectionFreeFormConfigItemGUI();
		void CreateDisplay(wxWindow *parent,int id,wxString labelText);
		void OnTextChange(wxCommandEvent& event);
		wxString GetNameFromCode(wxString code);
		void SetSelection(wxArrayString &codes,wxArrayString &names);
		virtual void ApplyConfigKeyChange();
		void OnSelectionChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		wxString GetValue();
		void SetValue(wxString &value);
	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxComboBox		*mChoices;
		wxBitmapButton	*mButton;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		wxArrayString	mCodes;
		wxArrayString	mNames;

		DECLARE_EVENT_TABLE()
};

#endif

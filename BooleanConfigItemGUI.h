#ifndef __BOOLEANCONFIGITEMGUI
#define __BOOLEANCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/checkbox.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class BooleanConfigItemGUI : public BaseConfigItemGUI
{
	public:
		BooleanConfigItemGUI();
		virtual ~BooleanConfigItemGUI();
		void CreateDisplay(wxWindow *parent,
						int id,
						wxString labelText);
		void OnTextChange(wxCommandEvent& event);
		virtual void ApplyConfigKeyChange();

		wxString GetValue();
	protected:

	private:
		wxCheckBox		*mTextEdit;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		DECLARE_EVENT_TABLE()
};

#endif

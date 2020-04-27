#ifndef __TEXTCONFIGITEMGUI
#define __TEXTCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class TextConfigItemGUI : public BaseConfigItemGUI
{
	public:
		TextConfigItemGUI();
		virtual ~TextConfigItemGUI();
		void CreateDisplay(wxWindow *parent,int id,wxString labelText);
		void OnTextChange(wxCommandEvent& event);
		virtual void ApplyConfigKeyChange();
		wxString GetValue();
	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		DECLARE_EVENT_TABLE()
};

#endif

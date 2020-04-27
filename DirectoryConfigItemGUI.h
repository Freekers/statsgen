#ifndef __DIRECTORYCONFIGITEMGUI
#define __DIRECTORYCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class DirectoryConfigItemGUI : public BaseConfigItemGUI
{
	public:
		DirectoryConfigItemGUI();
		virtual ~DirectoryConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		virtual void ApplyConfigKeyChange();
		void CreateDisplay(wxWindow *parent,int id,wxString &labelText);

		wxString GetValue();
		void SetValue(wxString &value);
	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxBitmapButton	*mButton;
		wxStaticText	*mLabel;
		wxString		mLabelText;

		DECLARE_EVENT_TABLE()
};

#endif

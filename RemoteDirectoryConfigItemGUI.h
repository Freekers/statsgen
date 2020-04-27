#ifndef __REMOTEDIRECTORYCONFIGITEMGUI
#define __REMOTEDIRECTORYCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class RemoteDirectoryConfigItemGUI : public BaseConfigItemGUI
{
	public:
		RemoteDirectoryConfigItemGUI();
		virtual ~RemoteDirectoryConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		void CreateDisplay(wxWindow *parent,
						int id,
						wxString &labelText);
		virtual void ApplyConfigKeyChange();

		wxString GetValue();
		void SetConfigKey(wxString configKey,wxString defaultValue,wxString id);
		void SetValue(wxString &value);
	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxBitmapButton	*mButton;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		wxString		mFTPID;

		DECLARE_EVENT_TABLE()
};

#endif

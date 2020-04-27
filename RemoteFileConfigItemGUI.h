#ifndef __REMOTEFILECONFIGITEMGUI
#define __REMOTEFILECONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"
#include "RemoteDirectoryConfigItemGUI.h"

class RemoteFileConfigItemGUI : public BaseConfigItemGUI
{
	public:
		RemoteFileConfigItemGUI();
		virtual ~RemoteFileConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		void CreateDisplay(wxWindow *parent,
						int id,
						wxString &labelText,
						RemoteDirectoryConfigItemGUI *directoryGUI=NULL);
		void SetConfigKey(wxString &configKey,wxString &defaultValue,wxString &ftpID);
		virtual void ApplyConfigKeyChange();

		wxString GetValue();
	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxBitmapButton	*mButton;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		wxString		mDefaultValue;
		RemoteDirectoryConfigItemGUI	*mDirectoryConfig;
		wxString		mFTPID;

		DECLARE_EVENT_TABLE()
};

#endif

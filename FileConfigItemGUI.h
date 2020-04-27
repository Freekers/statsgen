#ifndef __FILECONFIGITEMGUI
#define __FILECONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"
#include "DirectoryConfigItemGUI.h"

class FileConfigItemGUI : public BaseConfigItemGUI
{
	public:
		FileConfigItemGUI();
		virtual ~FileConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		void CreateDisplay(wxWindow *parent,
			int id,
			wxString &labelText,
			DirectoryConfigItemGUI *directoryConfig);
		virtual void ApplyConfigKeyChange();
		wxString GetValue();
	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxBitmapButton	*mButton;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		DirectoryConfigItemGUI	*mDirectoryConfig;

		DECLARE_EVENT_TABLE()
};

#endif

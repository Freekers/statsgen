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

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			int	maxCharactersIn,
			wxString &FTPIDIn,
			RemoteDirectoryConfigItemGUI	*directoryGUIIn=NULL);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
	protected:

	private:
		wxTextCtrl		textEdit;
		wxBitmapButton	button;
		wxStaticText	label;
		bool			maxCharactersSet;
		int				maxCharacters;
		wxString		labelText;
		wxString		defaultValue;
		RemoteDirectoryConfigItemGUI	*directoryConfig;
		wxString		FTPID;

		DECLARE_EVENT_TABLE()
};

#endif

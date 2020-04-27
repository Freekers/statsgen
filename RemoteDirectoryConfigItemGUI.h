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

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			int	maxCharactersIn,
			wxString &FTPIDIn);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
		void SetValue(wxString &value);
	protected:

	private:
		wxTextCtrl		textEdit;
		wxBitmapButton	button;
		wxStaticText	label;
		bool			maxCharactersSet;
		int				maxCharacters;
		wxString		labelText;
		wxString		defaultValue;
		wxString		FTPID;

		DECLARE_EVENT_TABLE()
};

#endif

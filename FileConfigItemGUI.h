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

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			int	maxCharactersIn,
			DirectoryConfigItemGUI	*directoryGUIIn=NULL);
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
		DirectoryConfigItemGUI	*directoryConfig;

		DECLARE_EVENT_TABLE()
};

#endif

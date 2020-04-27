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

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			int	maxCharactersIn);
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

		DECLARE_EVENT_TABLE()
};

#endif

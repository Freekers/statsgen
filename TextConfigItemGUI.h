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
		void OnTextChange(wxCommandEvent& event);

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			int	maxCharactersIn);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
	protected:

	private:
		wxTextCtrl		textEdit;
		wxStaticText	label;
		bool			maxCharactersSet;
		int				maxCharacters;
		wxString		labelText;
		wxString		defaultValue;

		DECLARE_EVENT_TABLE()
};

#endif

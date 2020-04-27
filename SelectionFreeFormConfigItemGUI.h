#ifndef __SELECTIONFREEFORMCONFIGITEMGUI
#define __SELECTIONFREEFORMCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class SelectionFreeFormConfigItemGUI : public BaseConfigItemGUI
{
	public:
		SelectionFreeFormConfigItemGUI();
		virtual ~SelectionFreeFormConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);
		void OnSelectionChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			wxArrayString &codes,
			wxArrayString &names,
			int	maxCharactersIn);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
		void SetValue(wxString &value);
	protected:

	private:
		wxTextCtrl		textEdit;
		wxComboBox		choices;
		wxBitmapButton	button;
		wxStaticText	label;
		bool			maxCharactersSet;
		int				maxCharacters;
		wxString		labelText;
		wxString		defaultValue;
		wxArrayString	codes;
		wxArrayString	names;

		DECLARE_EVENT_TABLE()
};

#endif

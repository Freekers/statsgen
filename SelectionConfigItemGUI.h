#ifndef __SELECTIONCONFIGITEMGUI
#define __SELECTIONCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class SelectionConfigItemGUI : public BaseConfigItemGUI
{
	public:
		SelectionConfigItemGUI();
		virtual ~SelectionConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			wxArrayString &codes,
			wxArrayString &names);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
	protected:

	private:
		wxComboBox		textEdit;
		wxStaticText	label;
		wxString		labelText;
		wxString		defaultValue;
		wxArrayString		codes;
		wxArrayString		names;

		DECLARE_EVENT_TABLE()
};

#endif

#ifndef __BOOLEANCONFIGITEMGUI
#define __BOOLEANCONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/checkbox.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class BooleanConfigItemGUI : public BaseConfigItemGUI
{
	public:
		BooleanConfigItemGUI();
		virtual ~BooleanConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
	protected:

	private:
		wxCheckBox		textEdit;
		wxStaticText	label;
		wxString		labelText;
		wxString		defaultValue;

		DECLARE_EVENT_TABLE()
};

#endif

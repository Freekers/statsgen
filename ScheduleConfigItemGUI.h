#ifndef __SCHEDULECONFIGITEMGUI
#define __SCHEDULECONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class ScheduleConfigItemGUI : public BaseConfigItemGUI
{
	public:
		ScheduleConfigItemGUI();
		virtual ~ScheduleConfigItemGUI();
		void OnTextChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);

		void SetConfigKey(wxString &configKeyIn);
		void SetLabelWidth(int width);
		int GetLabelWidth();
		void Set(wxString &configKeyIn,
			wxString &labelText,
			wxString &defaultValueIn,
			bool intervalWithBaseIn);
			void OnResize(wxSizeEvent &event);

		wxString GetValue();
	protected:

	private:
		wxTextCtrl		textEdit;
		wxBitmapButton	button;
		wxStaticText	label;
		bool			maxCharactersSet;
		wxString		labelText;
		wxString		defaultValue;
		bool			intervalWithBase;

		DECLARE_EVENT_TABLE()
};

#endif

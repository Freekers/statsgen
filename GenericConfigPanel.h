#ifndef __GENERICCONFIGPANEL
#define __GENERICCONFIGPANEL

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"
#include "GenericNumberedConfigPanel.h"

class GenericConfigPanel : public wxPanel
{
	public:
		GenericConfigPanel(wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~GenericConfigPanel();

		void OnResize(wxSizeEvent& event);

		void AddConfigGroup(GroupedConfigItemsPanel *configPanel);
		void AddConfigGroup(GenericNumberedConfigPanel *configPanel);
		void AddConfigGroup(wxPanel *configPanel);
		void OnTextChange(wxCommandEvent &event);
		bool UpdateFromTrigger();

	protected:

	private:
		ArrayOfPointers		configItems;
		wxArrayInt		panelTypes;
		DECLARE_EVENT_TABLE()
};

#endif

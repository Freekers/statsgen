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

		void AddConfigGroup(GroupedConfigItemsPanel *configPanel,int sizerProportion=0,int sizerFlags=wxEXPAND);
		void AddConfigGroup(GenericNumberedConfigPanel *configPanel,int sizerProportion=0,int sizerFlags=wxEXPAND);
		void AddConfigGroup(wxPanel *configPanel,int sizerProportion=0,int sizerFlags=wxEXPAND);
		void OnTextChange(wxCommandEvent &event);
		bool UpdateFromTrigger();

		void ConfigureSizer();
	protected:

	private:
		ArrayOfPointers		configItems;
		wxArrayInt		panelTypes;
		wxBoxSizer		*mMainSizer;
		DECLARE_EVENT_TABLE()
};

#endif

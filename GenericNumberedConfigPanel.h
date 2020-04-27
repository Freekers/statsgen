#ifndef __GENERICNUMBEREDCONFIGPANEL
#define __GENERICNUMBEREDCONFIGPANEL

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"

class GenericConfigPanel;
class GenericNumberedConfigPanel : public wxPanel
{
	public:
		GenericNumberedConfigPanel(
			GenericConfigPanel *(*newPanelFunctionIn)(wxWindow *parentID,wxString &listID),
				wxString &group,
				wxString &listGroupPrefix,
				wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name);
		virtual ~GenericNumberedConfigPanel();
		bool UpdateFromTrigger();
		

		void OnListItemSelected(wxCommandEvent &event);
		void OnTextChange(wxCommandEvent &event);
		void OnResize(wxSizeEvent &event);

		void CreateDialog();
		void SetConfigPanel(GenericConfigPanel *configPanel);

	protected:

	private:
		wxComboBox		idList;
		GenericConfigPanel * (*newPanelFunction)(wxWindow *parentID,wxString &listID);
		GenericConfigPanel	*configPanel;
		wxString	listGroup;
		wxString	listGroupPrefix;
		DECLARE_EVENT_TABLE()
};

#endif

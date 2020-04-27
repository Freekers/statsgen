// Statsgen Includes
#include "GenericNumberedConfigPanel.h"
#include "GenericConfigPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericNumberedConfigPanel, wxPanel)
		EVT_COMBOBOX(WINDOW_ID_LISTBOX_CONFIGITEMS,GenericNumberedConfigPanel::OnListItemSelected)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GenericNumberedConfigPanel::OnTextChange)
END_EVENT_TABLE()

GenericNumberedConfigPanel::GenericNumberedConfigPanel(
				GenericConfigPanel * (*newPanelFunctionIn)(wxWindow *parentID,wxString &listID),
				wxString &listGroupIn,
				wxString &listGroupPrefixIn,
				wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name):
			wxPanel(parent,
				id,
				pos,
				size,
				style,
				name)

{
	mConfigPanel		= NULL;
	mListGroup			= listGroupIn;
	mListGroupPrefix	= listGroupPrefixIn;

	mNewPanelFunction=newPanelFunctionIn;
}

bool GenericNumberedConfigPanel::UpdateFromTrigger()
{
	if (mConfigPanel!=NULL)
	{
		mConfigPanel->UpdateFromTrigger();
	}
	return (true);
}

void GenericNumberedConfigPanel::OnTextChange(wxCommandEvent &event)
{
	UpdateFromTrigger();
	if (GetParent()!=NULL)
	{
		//GetParent()->AddPendingEvent(event);
		GetParent()->GetEventHandler()->AddPendingEvent(event);
	}
}

void GenericNumberedConfigPanel::OnListItemSelected(wxCommandEvent& event)
{
	// we have a new selection
	// remove the current panel (if present) and retrieve
	// a new one using the retrieve function
	wxString	currentSelection;

	if (mConfigPanel!=NULL)
	{
		// first remove the old panel
		mConfigPanel->Destroy();
		mConfigPanel = NULL;
	}
	currentSelection=mIDList.GetValue();
	if (currentSelection.Length()>0)
	{
		mConfigPanel = mNewPanelFunction(this,currentSelection);
		SetConfigPanel(mConfigPanel);
	}
}

GenericNumberedConfigPanel::~GenericNumberedConfigPanel()
{
}

void GenericNumberedConfigPanel::SetConfigPanel(GenericConfigPanel *configPanelIn)
{
	mConfigPanel	= configPanelIn;
}

void GenericNumberedConfigPanel::CreateDialog()
{
	wxString		label="Generic Configuration";
	wxString		defaultValue="";
	wxString		configKey;
	wxCommandEvent		event;
	GroupedConfigItemsPanel	*configPanel;

	wxPoint			configItemsPosition=wxDefaultPosition;
	wxSize			configItemsSize=wxDefaultSize;
	wxArrayString	ids;
	wxString		initialSelection="";
	

	globalStatistics.configData.ReadList(mListGroup,ids);

	if (ids.GetCount()>0)
	{
		initialSelection=ids.Item(0);
	}
	mIDList.Create(this, 
					WINDOW_ID_LISTBOX_CONFIGITEMS,
					initialSelection,
					wxDefaultPosition,
					wxDefaultSize,
					ids,
					wxCB_DROPDOWN | wxCB_READONLY
					);
					
	OnListItemSelected(event);
}


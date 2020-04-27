// Statsgen Includes
#include "GenericNumberedConfigPanel.h"
#include "GenericConfigPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericNumberedConfigPanel, wxPanel)
		EVT_SIZE(GenericNumberedConfigPanel::OnResize)
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
	configPanel=NULL;
	listGroup=listGroupIn;
	listGroupPrefix=listGroupPrefixIn;
	newPanelFunction=newPanelFunctionIn;
}

bool GenericNumberedConfigPanel::UpdateFromTrigger()
{
	if (configPanel!=NULL)
	{
		configPanel->UpdateFromTrigger();
	}
	return (true);
}

void GenericNumberedConfigPanel::OnTextChange(wxCommandEvent &event)
{
	UpdateFromTrigger();
	if (GetParent()!=NULL)
	{
		GetParent()->AddPendingEvent(event);
	}
}


void GenericNumberedConfigPanel::OnListItemSelected(wxCommandEvent& event)
{
	wxSizeEvent		dummyEvent;
	// we have a new selection
	// remove the current panel (if present) and retrieve
	// a new one using the retrieve function
	wxString	currentSelection;

	if (configPanel!=NULL)
	{
		// first remove the old panel
		configPanel->Destroy();
		configPanel=NULL;
	}
	currentSelection=idList.GetValue();
	if (currentSelection.Length()>0)
	{
		configPanel=newPanelFunction(this,currentSelection);
		configPanel->OnResize(dummyEvent);
		SetConfigPanel(configPanel);
	}
	OnResize(dummyEvent);
}

GenericNumberedConfigPanel::~GenericNumberedConfigPanel()
{
}

void GenericNumberedConfigPanel::SetConfigPanel(GenericConfigPanel *configPanelIn)
{
	configPanel=configPanelIn;
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
	

	globalStatistics.configData.ReadList(listGroup,ids);

	if (ids.GetCount()>0)
	{
		initialSelection=ids.Item(0);
	}
	idList.Create(this, 
					WINDOW_ID_LISTBOX_CONFIGITEMS,
					initialSelection,
					wxDefaultPosition,
					wxDefaultSize,
					ids,
					wxCB_DROPDOWN | wxCB_READONLY
					);
					
	OnListItemSelected(event);
}

void GenericNumberedConfigPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		configPanelWidth;
	int		configPanelHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		yPosition;
	int		listWidth;
	int		listHeight;

	
	itemSize=GetSize();
	dialogWidth=itemSize.GetWidth();
	dialogHeight=itemSize.GetHeight();

	itemSize=idList.GetSize();
	listWidth=itemSize.GetWidth();
	if (listWidth<150)
	{
		listWidth=150;
	}
	idList.SetSize(listWidth,listHeight);
	listHeight=itemSize.GetHeight();

	configPanelHeight=dialogHeight-listHeight;
	configPanelWidth=dialogWidth;

	// ID List
	idList.SetSize(0,0,listWidth,listHeight);
	// Config Panel
	yPosition=listHeight;
	// Config Panels resize themselves to fit
	if (configPanel!=NULL)
	{
		configPanel->SetSize(0,yPosition,configPanelWidth,configPanelHeight);
	}

}


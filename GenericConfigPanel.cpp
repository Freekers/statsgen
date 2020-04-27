// Statsgen Includes
#include "GenericConfigPanel.h"
#include "GenericNumberedConfigPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericConfigPanel, wxPanel)
		EVT_SIZE(GenericConfigPanel::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GenericConfigPanel::OnTextChange)
END_EVENT_TABLE()

GenericConfigPanel::GenericConfigPanel(wxWindow *parent, 
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
}

GenericConfigPanel::~GenericConfigPanel()
{
}

void GenericConfigPanel::AddConfigGroup(GroupedConfigItemsPanel *configPanel)
{
	wxSizeEvent	dummy;
	panelTypes.Add(1);
	configItems.Add((void *)configPanel);
	OnResize(dummy);
}

void GenericConfigPanel::AddConfigGroup(wxPanel *configPanel)
{
	wxSizeEvent	dummy;
	panelTypes.Add(0);
	configItems.Add((void *)configPanel);
	OnResize(dummy);
}

void GenericConfigPanel::AddConfigGroup(GenericNumberedConfigPanel *configPanel)
{
	wxSizeEvent	dummy;
	panelTypes.Add(2);
	configItems.Add((void *)configPanel);
	OnResize(dummy);
}

bool GenericConfigPanel::UpdateFromTrigger()
{
	bool					panelTriggered;
	int						panelCount;
	int						panelIndex;
	int						panelType;

	panelTriggered=false;
	panelCount=configItems.GetCount();
	for (panelIndex=0;panelIndex<panelCount;panelIndex++)
	{
		panelType=panelTypes.Item(panelIndex);
		if (panelType==1)
		{
			GroupedConfigItemsPanel	*configPanel;
			configPanel=(GroupedConfigItemsPanel *)configItems.Item(panelIndex);
			if (configPanel->UpdateFromTrigger())
			{
				panelTriggered=true;
			}
		}
		if (panelType==2)
		{
			GenericNumberedConfigPanel	*configPanel;
			configPanel=(GenericNumberedConfigPanel *)configItems.Item(panelIndex);
			if (configPanel->UpdateFromTrigger())
			{
				panelTriggered=true;
			}
		}
	}
	wxSizeEvent	dummyEvent;
	OnResize(dummyEvent);
	return (panelTriggered);
}

void GenericConfigPanel::OnTextChange(wxCommandEvent &event)
{
	UpdateFromTrigger();
}


void GenericConfigPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		configPanelHeight;
	int		configPanelWidth;
	GroupedConfigItemsPanel	*configPanel;
	int		configPanelCount;
	int		configPanelIndex;
	int		yPosition;
	int		panelType;

	itemSize=GetSize();
	dialogWidth=itemSize.GetWidth();
	dialogHeight=itemSize.GetHeight();

	configPanelHeight=dialogHeight;
	configPanelWidth=dialogWidth;

	// Config Panel
	configPanelCount=configItems.GetCount();
	yPosition=0;
	// Config Panels resize themselves to fit
	for (configPanelIndex=0;configPanelIndex<configPanelCount;configPanelIndex++)
	{
		configPanel=(GroupedConfigItemsPanel *)configItems.Item(configPanelIndex);
		panelType=panelTypes.Item(configPanelIndex);
		configPanelHeight=0;
		switch (panelType)
		{
			case 1:
				{
					GroupedConfigItemsPanel	*thisPanel;
					thisPanel=(GroupedConfigItemsPanel *)configItems.Item(configPanelIndex);
					configPanelHeight=thisPanel->PreferredHeight();
				}
				break;
			case 2:
				{
					GenericNumberedConfigPanel	*thisPanel;
					thisPanel=(GenericNumberedConfigPanel *)configItems.Item(configPanelIndex);
					configPanelHeight=dialogHeight-yPosition;
				}
				break;
			default:
				configPanelHeight=dialogHeight-yPosition;
				break;
		}
		if (configPanelHeight<10)
		{
			configPanelHeight=10;
		}
		configPanel->SetSize(0,yPosition,configPanelWidth,configPanelHeight);
		// Move to the next panel
		yPosition+=configPanelHeight;
	}
}


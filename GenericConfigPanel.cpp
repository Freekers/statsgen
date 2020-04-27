// Statsgen Includes
#include "GenericConfigPanel.h"
#include "GenericNumberedConfigPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(GenericConfigPanel, wxPanel)
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
	mMainSizer = new wxBoxSizer(wxVERTICAL);
}

GenericConfigPanel::~GenericConfigPanel()
{
}

void GenericConfigPanel::AddConfigGroup(GroupedConfigItemsPanel *configPanel,int sizerProportion,int sizerFlags)
{
	panelTypes.Add(1);
	configItems.Add((void *)configPanel);
	mMainSizer->Add(configPanel,sizerProportion,sizerFlags);
	ConfigureSizer();
}

void GenericConfigPanel::AddConfigGroup(wxPanel *configPanel,int sizerProportion,int sizerFlags)
{
	panelTypes.Add(0);
	configItems.Add((void *)configPanel);
	mMainSizer->Add(configPanel,sizerProportion,sizerFlags);
	ConfigureSizer();
}

void GenericConfigPanel::AddConfigGroup(GenericNumberedConfigPanel *configPanel,int sizerProportion,int sizerFlags)
{
	panelTypes.Add(2);
	configItems.Add((void *)configPanel);
	mMainSizer->Add(configPanel,sizerProportion,sizerFlags);
	ConfigureSizer();
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
	return (panelTriggered);
}

void GenericConfigPanel::OnTextChange(wxCommandEvent &event)
{
	UpdateFromTrigger();
}

void GenericConfigPanel::ConfigureSizer()
{
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
}

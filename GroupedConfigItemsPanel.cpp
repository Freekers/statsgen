// Statsgen Includes
#include "GroupedConfigItemsPanel.h"
#include "GlobalStatistics.h"
#include "BaseConfigItemGUI.h"
#include "TextConfigItemGUI.h"
#include "FileConfigItemGUI.h"
#include "DirectoryConfigItemGUI.h"
#include "BooleanConfigItemGUI.h"
#include "RemoteFileConfigItemGUI.h"
#include "RemoteDirectoryConfigItemGUI.h"
#include "SelectionConfigItemGUI.h"
#include "ScheduleConfigItemGUI.h"
#include "Progress.h"


BEGIN_EVENT_TABLE(GroupedConfigItemsPanel, wxPanel)
		EVT_SIZE(GroupedConfigItemsPanel::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GroupedConfigItemsPanel::OnTextChange)
END_EVENT_TABLE()


GroupedConfigItemsPanel::GroupedConfigItemsPanel(char *titleIn)
{
	title=titleIn;
	staticBoxAdded=false;
}

GroupedConfigItemsPanel::~GroupedConfigItemsPanel()
{
}

bool GroupedConfigItemsPanel::UpdateFromTrigger()
{
	bool				panelTriggered;
	int					panelCount;
	int					panelIndex;
	BaseConfigItemGUI	*configPanel;

	panelTriggered=false;
	panelCount=configItemPanels.GetCount();
	for (panelIndex=0;panelIndex<panelCount;panelIndex++)
	{
		configPanel=(BaseConfigItemGUI *)configItemPanels.Item(panelIndex);
		if (configPanel->UpdateFromTrigger())
		{
			panelTriggered=true;
		}
	}
	wxSizeEvent dummyEvent;
	OnResize(dummyEvent);
	return (panelTriggered);
}

void GroupedConfigItemsPanel::OnTextChange(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","OnTextChange")
	UpdateFromTrigger();
	if (GetParent()!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Popping Event")
		GetParent()->AddPendingEvent(event);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::Add(char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	TextConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new TextConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,charWidth);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
}

int GroupedConfigItemsPanel::PreferredHeight()
{
	BaseConfigItemGUI	*configItemGUI;
	int					configItemCount;
	int					configItemIndex;
	int					panelHeight;

	panelHeight=0;
	configItemCount=configItemPanels.GetCount();
	for (configItemIndex=0;configItemIndex<configItemCount;configItemIndex++)
	{
		configItemGUI=(BaseConfigItemGUI *)configItemPanels.Item(configItemIndex);
		if (configItemGUI->IsShown())
		{
			panelHeight+=configItemGUI->PreferredHeight();
		}
	}

	panelHeight+=STATIC_BOX_TOP_GAP;
	panelHeight+=STATIC_BOX_BOTTOM_GAP;

	return (panelHeight);
}

void GroupedConfigItemsPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;
	int			configItemCount;
	int			configItemIndex;
	BaseConfigItemGUI	*configItemGUI;
	int			yPosition;
	wxSize		itemSize;
	int			panelWidth;
	int			panelHeight;
	int			itemWidth;
	int			itemHeight;
	int			labelWidth;
	int			maxLabelWidth;
	int			staticBoxWidth;
	int			staticBoxHeight;
	wxPoint		panelPosition;

	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();

	staticBoxWidth=panelWidth-STATIC_BOX_LEFT_GAP-STATIC_BOX_RIGHT_GAP;
	staticBoxHeight=panelHeight;


	configItemCount=configItemPanels.GetCount();

	yPosition=STATIC_BOX_TOP_GAP;

	// First - normalise the labels
	maxLabelWidth=0;
	for (configItemIndex=0;configItemIndex<configItemCount;configItemIndex++)
	{
		configItemGUI=(BaseConfigItemGUI *)configItemPanels.Item(configItemIndex);
		if (configItemGUI->IsShown())
		{
			labelWidth=configItemGUI->GetLabelWidth();
			if (labelWidth>maxLabelWidth)
			{
				maxLabelWidth=labelWidth;
			}
		}
	}

	for (configItemIndex=0;configItemIndex<configItemCount;configItemIndex++)
	{
		configItemGUI=(BaseConfigItemGUI *)configItemPanels.Item(configItemIndex);
		if (configItemGUI->IsShown())
		{
			configItemGUI->SetLabelWidth(maxLabelWidth);
			itemSize=configItemGUI->GetSize();
			itemWidth=itemSize.GetWidth();
			itemHeight=configItemGUI->PreferredHeight();
			if (itemHeight<2)
			{
				itemHeight=2;
			}

			itemWidth=staticBoxWidth-STATIC_BOX_LEFT_GAP-
								STATIC_BOX_RIGHT_GAP-
								STATIC_BOX_LEFT_GAP;


			configItemGUI->SetSize(STATIC_BOX_LEFT_GAP+STATIC_BOX_LEFT_GAP,
								yPosition,itemWidth,itemHeight);
			yPosition+=itemHeight;
		}
	}

	staticBoxHeight=yPosition+STATIC_BOX_BOTTOM_GAP;
	staticBox.SetSize(STATIC_BOX_LEFT_GAP,
					0,
					staticBoxWidth,
					staticBoxHeight);

	panelPosition=GetPosition();
	if (panelWidth<10)
	{
		panelWidth=10;
	}
	if (staticBoxHeight<10)
	{
		staticBoxHeight=10;
	}
	/*SetSize(panelWidth,
			staticBoxHeight);*/
	/*
	SetSize(panelPosition.x,
			panelPosition.y,
			panelWidth,
			staticBoxHeight);
	*/
	
}

void GroupedConfigItemsPanel::AddBoolean(char *label,
							wxString &configKey,
							bool defaultValue,
							GUITriggerList *triggerList)
{
	BooleanConfigItemGUI	*newPanel;
	wxString				labelStr=label;
	wxString				defaultStr;

	newPanel=new BooleanConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	if(defaultValue)
	{
		defaultStr="Y";
	}
	else
	{
		defaultStr="N";
	}
	newPanel->Set(configKey,labelStr,defaultStr);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
}

FileConfigItemGUI *GroupedConfigItemsPanel::AddFile(char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							DirectoryConfigItemGUI *directoryGUI,
							GUITriggerList *triggerList)
{
	FileConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new FileConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,charWidth,directoryGUI);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	return (newPanel);
}

DirectoryConfigItemGUI *GroupedConfigItemsPanel::AddDirectory(char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	DirectoryConfigItemGUI	*newPanel;
	wxString				labelStr=label;
	wxString				defaultStr=defaultValue;

	newPanel=new DirectoryConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,charWidth);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	return (newPanel);
}

RemoteFileConfigItemGUI *GroupedConfigItemsPanel::AddRemoteFile(char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							wxString &FTPID,
							RemoteDirectoryConfigItemGUI *directoryGUI,
							GUITriggerList *triggerList)
{
	RemoteFileConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new RemoteFileConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,charWidth,
					FTPID,directoryGUI);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	return (newPanel);
}

RemoteDirectoryConfigItemGUI *GroupedConfigItemsPanel::AddRemoteDirectory(
							char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							wxString &FTPID,
							GUITriggerList *triggerList)
{
	RemoteDirectoryConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new RemoteDirectoryConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,charWidth,
					FTPID);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	return (newPanel);
}

void GroupedConfigItemsPanel::AddSelection(char *label,
							wxString &configKey,
							char *defaultValue,
							wxArrayString &codes,
							wxArrayString &names,
							GUITriggerList *triggerList)
{
	SelectionConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new SelectionConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,codes,names);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
}

void GroupedConfigItemsPanel::AddSchedule(char *label,
							wxString &configKey,
							wxString &defaultValue,
							bool intervalWithBase,
							GUITriggerList *triggerList)
{
	ScheduleConfigItemGUI	*newPanel;
	wxString				labelStr=label;

	newPanel=new ScheduleConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultValue,intervalWithBase);

	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
}

void GroupedConfigItemsPanel::AddSelectionFreeForm(char *label,
							wxString &configKey,
							char *defaultValue,
							wxArrayString &codes,
							wxArrayString &names,
							int charWidth,
							GUITriggerList *triggerList)
{
	SelectionFreeFormConfigItemGUI	*newPanel;
	wxString						labelStr=label;
	wxString						defaultStr=defaultValue;

	newPanel=new SelectionFreeFormConfigItemGUI();
	newPanel->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	newPanel->Set(configKey,labelStr,defaultStr,codes,names,charWidth);

	configItemPanels.Add((void *)newPanel);
	if (!staticBoxAdded)
	{
		staticBoxAdded=true;
		staticBox.Create(this,-1,title);
	}
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
}


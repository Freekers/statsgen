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
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,GroupedConfigItemsPanel::OnTextChange)
END_EVENT_TABLE()

GroupedConfigItemsPanel::GroupedConfigItemsPanel(char *title)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","GroupedConfigItemsPanel")
	mTitle	= title;
	STATSGEN_DEBUG_FUNCTION_END
}

GroupedConfigItemsPanel::~GroupedConfigItemsPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","~GroupedConfigItemsPanel")
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::CreateDisplay(wxWindow *parent,int id)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","CreateDisplay")
	Create(parent,id);

	mTitleBox		= new wxStaticBox(this,wxID_ANY,mTitle);
	mTitleSizer		= new wxStaticBoxSizer(mTitleBox,wxVERTICAL);
	mContentsSizer	= new wxBoxSizer(wxVERTICAL);
	mTitleSizer->Add(mContentsSizer,1,wxEXPAND);
	STATSGEN_DEBUG_FUNCTION_END
}
bool GroupedConfigItemsPanel::UpdateFromTrigger()
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","UpdateFromTrigger")
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
	STATSGEN_DEBUG_FUNCTION_END
	return (panelTriggered);
}

void GroupedConfigItemsPanel::OnTextChange(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","OnTextChange")
	UpdateFromTrigger();
	if (GetParent()!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Popping Event")
		//GetParent()->AddPendingEvent(event);
		GetParent()->GetEventHandler()->AddPendingEvent(event);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::Add(char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","Add")
	wxString	labelStr = label;
	wxString	defaultValueStr = defaultValue;
	Add(labelStr,configKey, defaultValueStr,charWidth,triggerList);
	STATSGEN_DEBUG_FUNCTION_END
}
void GroupedConfigItemsPanel::Add(wxString label,
							wxString &configKey,
							wxString defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","Add")
	TextConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new TextConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr);
	newPanel->SetConfigKey(configKey,defaultStr);
	newPanel->ApplyConfigKeyChange();
	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::AddBoolean(char *label,
							wxString &configKey,
							bool defaultValue,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddBoolean")
	BooleanConfigItemGUI	*newPanel;
	wxString				labelStr=label;
	wxString				defaultStr;

	if(defaultValue)
	{
		defaultStr="Y";
	}
	else
	{
		defaultStr="N";
	}
	newPanel=new BooleanConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr);
	newPanel->SetConfigKey(configKey,defaultStr);
	newPanel->ApplyConfigKeyChange();
	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

FileConfigItemGUI *GroupedConfigItemsPanel::AddFile(wxString label,
							wxString &configKey,
							wxString defaultValue,
							int charWidth,
							DirectoryConfigItemGUI *directoryGUI,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddFile")
	FileConfigItemGUI	*newPanel;
	wxString			defaultStr=defaultValue;

	newPanel=new FileConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,label,directoryGUI);
	newPanel->SetConfigKey(configKey,defaultStr);
	newPanel->ApplyConfigKeyChange();

	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
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
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddRemoteFile")
	RemoteFileConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new RemoteFileConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr,directoryGUI);
	newPanel->SetConfigKey(configKey,defaultStr,FTPID);
	newPanel->ApplyConfigKeyChange();

	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
	return (newPanel);
}

DirectoryConfigItemGUI *GroupedConfigItemsPanel::AddDirectory(wxString label,
							wxString &configKey,
							wxString defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddDirectory")
	DirectoryConfigItemGUI	*newPanel;
	wxString				defaultStr=defaultValue;

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"new DirectoryConfigItemGUI");
	newPanel=new DirectoryConfigItemGUI();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"DirectoryConfigItemGUI Create");
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,label);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"DirectoryConfigItemGUI Set");
	newPanel->SetConfigKey(configKey,defaultStr);
	newPanel->ApplyConfigKeyChange();

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"configPanels Add");
	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"set trigger condition");
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
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
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddRemoteDirectory")
	RemoteDirectoryConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	newPanel=new RemoteDirectoryConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr);
	newPanel->SetConfigKey(configKey,defaultStr,FTPID);
	newPanel->ApplyConfigKeyChange();

	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
	return (newPanel);
}

void GroupedConfigItemsPanel::AddSelection(wxString label,
							wxString &configKey,
							wxString defaultValue,
							wxArrayString &codes,
							wxArrayString &names,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddSelection")
	SelectionConfigItemGUI	*newPanel;
	wxString			labelStr=label;
	wxString			defaultStr=defaultValue;

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"newPanel");
	newPanel=new SelectionConfigItemGUI();
	newPanel->SetSelection(codes,names);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"newPanel create");
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"newPanel set");
	newPanel->SetConfigKey(configKey,defaultStr);
	newPanel->ApplyConfigKeyChange();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"add");
	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"newPanel settriggerlist");
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::AddSchedule(char *label,
							wxString &configKey,
							wxString &defaultValue,
							bool intervalWithBase,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddSchedule")
	ScheduleConfigItemGUI	*newPanel;
	wxString				labelStr=label;

	newPanel=new ScheduleConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr);
	newPanel->SetConfigKey(configKey,defaultValue,intervalWithBase);
	newPanel->ApplyConfigKeyChange();

	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

FileConfigItemGUI *GroupedConfigItemsPanel::AddFile(
							char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							DirectoryConfigItemGUI *directoryGUI,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddFile")
	FileConfigItemGUI	*panel;
	wxString	labelStr = label;
	wxString	defaultStr = defaultValue;

	panel = AddFile(labelStr,configKey,defaultStr,charWidth,directoryGUI,triggerList);
	STATSGEN_DEBUG_FUNCTION_END
	return panel;
}

DirectoryConfigItemGUI *GroupedConfigItemsPanel::AddDirectory(char *label,
							wxString &configKey,
							wxString defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddDirectory")
	DirectoryConfigItemGUI *panel;
	wxString	labelStr = label;
	panel = AddDirectory(labelStr,configKey,defaultValue,charWidth,triggerList);
	STATSGEN_DEBUG_FUNCTION_END
	return (panel);
}
DirectoryConfigItemGUI *GroupedConfigItemsPanel::AddDirectory(char *label,
							wxString &configKey,
							char *defaultValue,
							int charWidth,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddDirectory")
	DirectoryConfigItemGUI *panel;
	wxString	labelStr = label;
	wxString	defaultValueStr = defaultValue;
	panel = AddDirectory(labelStr,configKey,defaultValueStr,charWidth,triggerList);
	STATSGEN_DEBUG_FUNCTION_END
	return panel;
}

void GroupedConfigItemsPanel::AddSelection(char *label,
							wxString &configKey,
							char *defaultValue,
							wxArrayString &codes,
							wxArrayString &names,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddSelection")
	wxString	labelStr = label;
	wxString	defaultValueStr = defaultValue;
	AddSelection(labelStr,configKey, defaultValueStr, codes, names, triggerList);
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::AddSelectionFreeForm(char *label,
							wxString &configKey,
							char *defaultValue,
							wxArrayString &codes,
							wxArrayString &names,
							int charWidth,
							GUITriggerList *triggerList)
{
	STATSGEN_DEBUG_FUNCTION_START("GroupedConfigItemsPanel","AddSelectionFreeForm")
	SelectionFreeFormConfigItemGUI	*newPanel;
	wxString						labelStr=label;
	wxString						defaultStr=defaultValue;

	newPanel=new SelectionFreeFormConfigItemGUI();
	newPanel->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,labelStr);
	newPanel->SetSelection(codes,names);
	newPanel->SetConfigKey(configKey,defaultStr);
	newPanel->ApplyConfigKeyChange();

	configItemPanels.Add((void *)newPanel);
	if (triggerList!=NULL)
	{
		newPanel->SetTriggerCondition(*triggerList);
	}
	mContentsSizer->Add(newPanel,1,wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN);
	ConfigureSizer();
	STATSGEN_DEBUG_FUNCTION_END
}

void GroupedConfigItemsPanel::ConfigureSizer()
{
	mTitleSizer->SetSizeHints(this);
	SetSizer(mTitleSizer);
}

// Statsgen Includes
#include "FTPConnectionPanel.h"
#include "GlobalStatistics.h"
#include "GUITriggerList.h"

FTPConnectionPanel::FTPConnectionPanel() : GroupedConfigItemsPanel((char *)"FTP Connection Settings")
{
}

void FTPConnectionPanel::CreateConfigs(char *group)
{
	wxString	groupStr = group;
	CreateConfigs(groupStr);
}
void FTPConnectionPanel::CreateConfigs(wxString group)
{
	wxString	configKey;
	wxString	triggerKey;

	GUITriggerList	triggerList;
	GUITriggerItem	triggerItem;

	triggerKey.Printf("/%s/FTPEnabled",group);

	triggerItem.SetPositiveDisableTrigger(triggerKey,(char *)"N");
	triggerList.Add(triggerItem);

	AddBoolean((char *)"Enabled",triggerKey,false);

	configKey.Printf("/%s/IPAddress",group);
	Add((char *)"Hostname",configKey,(char *)"",-1);

	configKey.Printf("/%s/FTPUsername",group);
	Add((char *)"Username",configKey,(char *)"",-1,&triggerList);

	configKey.Printf("/%s/FTPPassword",group);
	Add((char *)"Password",configKey,(char *)"",-1,&triggerList);

	configKey.Printf("/%s/FTPPort",group);
	Add((char *)"Port",configKey,(char *)"21",5,&triggerList);

	configKey.Printf("/%s/FTPPassive",group);
	AddBoolean((char *)"Passive",configKey,true,&triggerList);

	configKey.Printf("/%s/FTPRestartDownload",group);
	AddBoolean((char *)"Restart Downloads",configKey,true,&triggerList);
}

FTPConnectionPanel::~FTPConnectionPanel()
{
}


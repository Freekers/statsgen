// Statsgen Includes
#include "FTPConnectionPanel.h"
#include "GlobalStatistics.h"
#include "GUITriggerList.h"

FTPConnectionPanel::FTPConnectionPanel() : GroupedConfigItemsPanel("FTP Connection Settings")
{
}

void FTPConnectionPanel::CreateConfigs(char *group)
{
	wxString	configKey;
	wxString	triggerKey;

	GUITriggerList	triggerList;
	GUITriggerItem	triggerItem;

	triggerKey.Printf("/%s/FTPEnabled",group);

	triggerItem.SetPositiveDisableTrigger(triggerKey,"N");
	triggerList.Add(triggerItem);

	AddBoolean("Enabled",triggerKey,false);

	configKey.Printf("/%s/IPAddress",group);
	Add("Hostname",configKey,"",-1);

	configKey.Printf("/%s/FTPUsername",group);
	Add("Username",configKey,"",-1,&triggerList);

	configKey.Printf("/%s/FTPPassword",group);
	Add("Password",configKey,"",-1,&triggerList);

	configKey.Printf("/%s/FTPPort",group);
	Add("Port",configKey,"21",5,&triggerList);

	configKey.Printf("/%s/FTPPassive",group);
	AddBoolean("Passive",configKey,true,&triggerList);

	configKey.Printf("/%s/FTPRestartDownload",group);
	AddBoolean("Restart Downloads",configKey,true,&triggerList);
}

FTPConnectionPanel::~FTPConnectionPanel()
{
}


// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>
#include <wx/filename.h>
#include <wx/app.h>

// Statsgen includes
#include "ConfigData.h"
#include "ErrorData.h"
#include "StaticFunctions.h"
#include "GlobalStatistics.h"

ConfigData::ConfigData()
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigData","Constructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigData::Initiate()
{
	wxFileName	baseDirectory(wxGetApp().argv[0]);
	wxFileName	tempIniFile;

	STATSGEN_DEBUG_FUNCTION_START("ConfigData","Initiate")

	tempIniFile=baseDirectory;
	tempIniFile.MakeAbsolute();
	tempIniFile.SetFullName("statsgen2.ini");

	Initiate(tempIniFile);

	STATSGEN_DEBUG_FUNCTION_END
}

ConfigData::~ConfigData()
{
	weaponGroupCache.Close();
	Close();
}

void ConfigData::Close()
{
	if (configFile!=NULL)
	{
		delete configFile;
	}
	configFile=NULL;
}

int ConfigData::FindConfigCacheEntry(wxString &key)
{
	int			cacheIndex;
	int			foundIndex;
	int			cacheCount;
	ConfigItem	configItem;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("ConfigData","FindConfigCacheEntry")
	STATSGEN_DEBUG(DEBUG_RARELY,key);
	cacheCount=configKeys.GetCount();
	foundIndex=-1;
	for (cacheIndex=0;cacheIndex<cacheCount;cacheIndex++)
	{
		configItem=configKeys.Item(cacheIndex);
		if (configItem.key.Cmp(key)==0)
		{
			// Found an entry in the cache
			foundIndex=cacheIndex;
			break;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(foundIndex);
}

void ConfigData::RollbackChanges()
{
	int			cacheIndex;
	int			cacheCount;
	ConfigItem	configItem;

	// We need to delete each entry in the Config Cache
	// that has a committed value of false
	cacheCount=configKeys.GetCount();
	cacheIndex=0;
	while (cacheIndex<cacheCount)
	{
		configItem=configKeys.Item(cacheIndex);
		if (!configItem.committed)
		{
			configKeys.RemoveAt(cacheIndex);
			cacheCount--;
		}
		else
		{
			cacheIndex++;
		}
	}

	groupsDeleted.Clear();
}

void ConfigData::CommitChanges()
{
	int			cacheIndex;
	int			cacheCount;
	ConfigItem	configItem;
	wxString	errorMessage;
	wxString	group;
	int			groupCount;
	int			groupIndex;

	STATSGEN_DEBUG_FUNCTION_START("ConfigData","CommitChanges")
	// Find each entry in the cache that has not been
	// committed and write it out to the cache file
	// then set it's own flag to committed
	cacheCount=configKeys.GetCount();
	cacheIndex=0;
	while (cacheIndex<cacheCount)
	{
		configItem=configKeys.Item(cacheIndex);
		if (!configItem.committed)
		{
			configKeys.RemoveAt(cacheIndex);
			configItem.committed=true;
			cacheCount--;
			// Add back in the committed version
			// the lowered cache count will mean
			// we shouldn't bother with reading this
			// far
			configKeys.Add(configItem);
			STATSGEN_DEBUG_CODE(errorMessage="Writing " + configItem.key + " to file";)
			STATSGEN_DEBUG(DEBUG_RARELY,errorMessage)
			// Write the actual entry to the config file
			configFile->Write(configItem.key,configItem.value);
		}
		else
		{
			cacheIndex++;
		}
	}
	
	STATSGEN_DEBUG(DEBUG_SOMETIMES,"Written non-committed data to config")
	// We have stepped through every cache entry and written
	// them to the config file now remove any groups that are 
	// in the deleted groups list
	groupCount=groupsDeleted.GetCount();
	STATSGEN_DEBUG_CODE(errorMessage.Printf("delete groups list=%d",groupCount);)
	STATSGEN_DEBUG(DEBUG_SOMETIMES,errorMessage)
	for (groupIndex=0;groupIndex<groupCount;groupIndex++)
	{
		group="/"+groupsDeleted.Item(groupIndex);
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Deleting group [%s]",group.GetData());)
		STATSGEN_DEBUG(DEBUG_RARELY,errorMessage)
		configFile->DeleteGroup(group);
	}
	groupsDeleted.Clear();
	// - flush the file to ensure
	// it all is actually written
	configFile->Flush();
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigData::EmptyCache()
{
	configKeys.Clear();
}

bool ConfigData::ReadTextValue(wxString &key,wxString *value)
{
	int		cacheIndex;
	bool		retVal=false;
	ConfigItem	configItem;
	wxString	errorMessage;

	// Here we need to read a config item from the
	// config cache. If we cannot find the config item
	// we can then attempt to read it from the config file
	// itself.
	cacheIndex=FindConfigCacheEntry(key);
	if (cacheIndex>=0)
	{
		// Found an entry in the cache
		configItem=configKeys.Item(cacheIndex);
		*value=configItem.value;
		retVal=true;
	}
	else
	{
		// Not found it in the cache
		// search for it in the actual config file
		if (!configFile->Read(key,value))
		{
			// Not in the cache or the real file
			retVal=false;
		}
		else
		{
			// Found it in the real file
			// Add it to the cache now
			configItem.key=key;
			configItem.value=(*value);
			configItem.committed=true;
			configKeys.Add(configItem);
			retVal=true;
		}
	}

	return (retVal);
}

bool ConfigData::ReadTextValue(char *key,wxString *value)
{
	wxString	keyString;

	keyString=key;
	return(ReadTextValue(keyString, value));
}

bool ConfigData::ReadTextValue(wxString &key,wxString *value,char *defaultValue)
{
	bool	retVal;

	retVal=ReadTextValue(key,value);
	if (retVal==false)
	{
		WriteTextValue(key,defaultValue);
		*value=defaultValue;
	}
	else
	{
		if (strlen(defaultValue)>0)
		{
			if (value->Length()==0)
			{
				WriteTextValue(key,defaultValue);
				*value=defaultValue;
			}
		}
	}

	return (retVal);
}

bool ConfigData::ReadTextValue(char *key,wxString *value,char *defaultValue)
{
	wxString	keyString;

	keyString=key;
	return(ReadTextValue(keyString, value, defaultValue));
}

void ConfigData::WriteTextValue(wxString &key,wxString &value)
{
	int			cacheIndex;
	ConfigItem	configItem;

	// First try and find it in the cache
	cacheIndex=FindConfigCacheEntry(key);
	if (cacheIndex>=0)
	{
		// Found a cache entry - just remove it
		// We are gonna add a new cache entry
		// regardless
		configKeys.RemoveAt(cacheIndex);
	}

	configItem.key=key;
	configItem.value=value;
	configItem.committed=false;
	configKeys.Add(configItem);
}

void ConfigData::WriteTextValue(wxString &key,char *value)
{
	wxString	valueString;

	valueString=value;
	WriteTextValue(key,valueString);
}

void ConfigData::WriteTextValue(char *key,wxString &value)
{
	wxString	keyString;

	keyString=key;
	WriteTextValue(keyString,value);
}

void ConfigData::WriteTextValue(char *key,char *value)
{
	wxString	valueString;
	wxString	keyString;

	valueString=value;
	keyString=key;
	WriteTextValue(keyString,valueString);
}

bool ConfigData::ReadGroup(wxString &group,
				wxArrayString &keys,
				wxArrayString &values)
{
	bool		retVal=true;
	long		cookie=-1;
	bool		moreConfigItems;
	wxString	configItem;
	wxString	msg;
	wxString	configPath;
	wxString	configValue;
	wxString	configKey;
	int			index;
	int			count;

	keys.Clear();
	values.Clear();

	STATSGEN_DEBUG_FUNCTION_START("ConfigData","ReadGroup")
	STATSGEN_DEBUG(DEBUG_RARELY,group)
	
	configPath="/"+group+"/";
	configFile->SetPath(configPath);
	moreConfigItems=configFile->GetFirstEntry(configItem,cookie);
	while (moreConfigItems)
	{
		keys.Add(configItem);
		moreConfigItems=configFile->GetNextEntry(configItem,cookie);
	}

	count=keys.GetCount();
	for (index=0;index<count;index++)
	{
		configItem=keys.Item(index);
		configKey=configPath+configItem;
		ReadTextValue(configKey,&configValue);
		values.Add(configValue);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool ConfigData::ReadGroup(char *group,
				wxArrayString &keys,
				wxArrayString &values)
{
	wxString	groupStr=group;

	return (ReadGroup(groupStr,keys,values));
}

bool ConfigData::ReadList(wxString &group, wxArrayString &list)
{
	return (ReadList(group,"",list));
}

bool ConfigData::ReadList(char *group, wxArrayString &list)
{
	return (ReadList(group,"",list));
}

bool ConfigData::ReadList(wxString &group, 
			wxString &subGroup,
			wxArrayString &list)
{
	bool		retVal=true;
	wxString	configKey;
	wxString	configValue;
	int			listCount;
	int			listIndex;

	list.Clear();

	configKey.Printf("/%s/%sLISTCOUNT",
						group.GetData(),
						subGroup.GetData());
	ReadTextValue(configKey,&configValue,"0");
	listCount=atoi(configValue.GetData());
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		configKey.Printf("/%s/%sLISTITEM%03d",
							group.GetData(),
							subGroup.GetData(),
							listIndex+1);
		ReadTextValue(configKey,&configValue,"");
		list.Add(configValue);
	}
	return (retVal);
}

bool ConfigData::ReadList(wxString &group, 
			char *subGroup,
			wxArrayString &list)
{
	wxString	subGroupStr;

	subGroupStr=subGroup;

	return (ReadList(group,subGroupStr,list));
}

bool ConfigData::ReadList(char *group, 
			wxString &subGroup,
			wxArrayString &list)
{
	wxString	groupStr;

	groupStr=group;

	return (ReadList(groupStr,subGroup,list));
}

bool ConfigData::ReadList(char *group, 
			char *subGroup,
			wxArrayString &list)
{
	wxString	groupStr;
	wxString	subGroupStr;

	groupStr=group;
	subGroupStr=subGroup;

	return (ReadList(groupStr,subGroupStr,list));
}

bool ConfigData::WriteList(wxString &group, wxString &subGroup,
							wxArrayString &list)
{
	bool		retVal=true;
	wxString	configKey;
	wxString	configValue;
	int			listCount;
	int			listIndex;

	listCount=list.GetCount();
	configValue.Printf("%d",listCount);
	configKey.Printf("/%s/%sLISTCOUNT",group.GetData(),subGroup.GetData());
	WriteTextValue(configKey,configValue);
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		configKey.Printf("/%s/%sLISTITEM%03d",group.GetData(),
								subGroup.GetData(),listIndex+1);
		configValue=list.Item(listIndex);
		WriteTextValue(configKey,configValue);
	}
	return (retVal);
}

bool ConfigData::WriteList(wxString &group, char *subGroup,
							wxArrayString &list)
{
	wxString	subGroupStr=subGroup;

	return (WriteList(group,subGroupStr,list));
}

bool ConfigData::WriteList(char *group, char *subGroup,
							wxArrayString &list)
{
	wxString	subGroupStr=subGroup;
	wxString	groupStr=group;

	return (WriteList(groupStr,subGroupStr,list));
}

bool ConfigData::WriteList(char *group, wxString &subGroup,
							wxArrayString &list)
{
	wxString	groupStr=group;

	return (WriteList(groupStr,subGroup,list));
}

bool ConfigData::WriteList(wxString &group, wxArrayString &list)
{
	return (WriteList(group,"",list));
}

bool ConfigData::WriteList(char *group, wxArrayString &list)
{
	wxString	groupStr=group;

	return (WriteList(groupStr,"",list));
}

void ConfigData::PurgeDeletedEntry(wxString &configKey)
{
	int			groupCount;
	int			groupIndex;
	wxString	groupPrefix;
	wxString	group;

	// Step through each group and see if this config key
	// is present in the group - if it is we don't wanna delete
	// it (i.e. remove it from the delete list)
	groupCount=groupsDeleted.GetCount();
	for (groupIndex=0;groupIndex<groupCount;groupIndex++)
	{
		group=groupsDeleted.Item(groupIndex);
		groupPrefix="/"+group+"/";
		if (configKey.StartsWith(groupPrefix))
		{
			// this config key matches a deleted group
			// so this group is not to be deleted
			groupsDeleted.RemoveAt(groupIndex);
			break;
		}
	}
}

bool ConfigData::DeleteGroup(wxString &group)
{
	bool		retVal=true;
	int			groupIndex;
	int			groupCount;
	wxString	deletedGroup;
	bool		groupNotDeleted=true;

	groupCount=groupsDeleted.GetCount();
	for (groupIndex=0;groupIndex<groupCount;groupIndex++)
	{
		deletedGroup=groupsDeleted.Item(groupIndex);
		if (deletedGroup.Cmp(group)==0)
		{
			groupNotDeleted=false;
			break;
		}
	}

	if (groupNotDeleted)
	{
			groupsDeleted.Add(group);
	}

	return (retVal);
}

bool ConfigData::DeleteGroup(char *group)
{
	wxString	groupStr=group;

	return (DeleteGroup(groupStr));
}

void ConfigData::Initiate(wxFileName &iniFilenameIn)
{
	wxString	msg;
	long	style=wxCONFIG_USE_NO_ESCAPE_CHARACTERS;
	char	*appName="Statsgen2";
	char	*vendorName="";

	STATSGEN_DEBUG_FUNCTION_START("ConfigData","Initiate")

	iniFilename=iniFilenameIn;
	// Initialise the config file - 
	configFile=new wxFileConfig(appName,
					vendorName,
					iniFilename.GetFullPath(),
					iniFilename.GetFullPath(),
					style);
//	configFile->SetExpandingEnvVars(false);

	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigData::Export(wxArrayString &upgradeList,
			ConfigData &sourceConfig,
			char *description,
			char *group,
			int exportType,
			int exportAnswerType,
			char *recommendation)
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigData","Export")

	wxArrayString	keys;
	wxArrayString	values;
	int		keyCount;
	int		keyIndex;
	wxString	configKey;
	wxString	configValue;
	wxString	outputConfigKey;
	wxString	groupStr;

	groupStr=group;
	upgradeList.Add(groupStr);
	sourceConfig.ReadGroup(group,keys,values);
	keyCount=keys.GetCount();
	outputConfigKey="/";
	outputConfigKey+=group;
	outputConfigKey+="/UPGRADEDESCRIPTION";
	WriteTextValue(outputConfigKey,description);
	outputConfigKey="/";
	outputConfigKey+=group;
	outputConfigKey+="/UPGRADERECOMMENDATION";
	WriteTextValue(outputConfigKey,recommendation);
	outputConfigKey="/";
	outputConfigKey+=group;
	outputConfigKey+="/UPGRADEANSWERTYPE";
	configValue.Printf("%d",exportAnswerType);
	WriteTextValue(outputConfigKey,configValue);
	
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		configKey=keys.Item(keyIndex);
		configValue=values.Item(keyIndex);
		if ((exportType==CONFIG_EXPORT_ALL)||(configValue.Length()>0))
		{
			outputConfigKey="/";
			outputConfigKey+=group;
			outputConfigKey+="/";
			outputConfigKey+=configKey;
			WriteTextValue(outputConfigKey,configValue);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigData::ReadGroupList(wxArrayString &listOfGroups)
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigData","ReadGroupList")
	wxString		groupName;
	long			index=0;
	bool			moreEntries;

	// ReadGroupList does not work properly - for some reason after 2 reads it
	// no longer gets a list
	//
	Close();
	Initiate();

	listOfGroups.Clear();
	moreEntries=configFile->GetFirstGroup(groupName,index);

	while (moreEntries)
	{
		listOfGroups.Add(groupName);
		moreEntries=configFile->GetNextGroup(groupName,index);
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigData::UpgradeDetails(wxString &description,wxString &upgradeType)
{
	wxString		configKeyDescription="/UPGRADELIST/Description";
	wxString		configKeyType="/UPGRADELIST/UpgradeType";
	wxString		configGroup="UPGRADELIST";
	wxArrayString	groups;

	description="";
	upgradeType="";

	ReadList((char *)configGroup.GetData(),groups);
	ReadTextValue(configKeyDescription,&description);
	ReadTextValue(configKeyType,&upgradeType);
	if (groups.GetCount()>0)
	{
		upgradeType=UPGRADE_TYPE_CONFIG;
	}
	else
	{
		if (upgradeType.Length()==0)
		{
			upgradeType=UPGRADE_TYPE_UNKNOWN;
		}
	}

	if (description.Length()==0)
	{
		description=iniFilename.GetFullName();
	}
}

wxArrayString ConfigData::ReadWeaponGroups()
{
	wxArrayString	groups;
	int				groupCount;
	int				groupIndex;
	wxString		group;
	wxArrayString	weaponGroups;

	ReadGroupList(groups);

	groupCount=groups.GetCount();
	for (groupIndex=0;groupIndex<groupCount;groupIndex++)
	{
		group=groups.Item(groupIndex);
		if (IsWeaponGroupKey(group))
		{
			weaponGroups.Add(group);
		}
	}

	return (weaponGroups);
}

wxArrayString ConfigData::WeaponGroupKeys(wxString &weaponGroup)
{
	wxArrayString	weaponGroupKeys;
	int		keyIndex;
	int		keyCount;
	wxString	key;
	wxArrayString	weaponGroupContents;

	if (IsWeaponGroupKey(weaponGroup))
	{
		weaponGroupContents=ReadWeaponGroup(weaponGroup);
		keyCount=weaponGroupContents.GetCount();
		for (keyIndex=0;keyIndex<keyCount;keyIndex++)
		{
			key=weaponGroupContents.Item(keyIndex);
			WX_APPEND_ARRAY(weaponGroupKeys,WeaponGroupKeys(key));
		}
	}
	else
	{
		weaponGroupKeys.Add(weaponGroup);
	}

	return (weaponGroupKeys);
}

bool ConfigData::IsWeaponGroupKey(wxString &key)
{
	return (key.StartsWith(CONFIG_WEAPON_GROUP_PREFIX));
}

wxArrayString ConfigData::ReadWeaponGroup(wxString &weaponGroup)
{
	wxArrayString	weaponGroupKeys;
	wxArrayString	weaponGroupValues;

	ReadGroup(weaponGroup,weaponGroupKeys,weaponGroupValues);

	return (weaponGroupKeys);
}

bool ConfigData::WeaponMatches(wxString &weaponKey,wxString &weaponGroupOrKey)
{
	return (weaponGroupCache.WeaponMatches(weaponKey,weaponGroupOrKey));
}

void ConfigData::OpenWeaponGroupCache()
{
	weaponGroupCache.Open();
}

void ConfigData::CloseWeaponGroupCache()
{
	weaponGroupCache.Close();
}

WeaponGroupCache::WeaponGroupCache()
{
	cacheOpen=false;
}

WeaponGroupCache::~WeaponGroupCache()
{
	Close();
}

void WeaponGroupCache::Open()
{
	int				weaponGroupsCount;
	int				weaponGroupsIndex;
	wxString		weaponGroup;
	wxArrayString	*weaponGroupContents;

	if (cacheOpen)
	{
		return;
	}
	weaponGroups=globalStatistics.configData.ReadWeaponGroups();

	weaponGroupsCount=weaponGroups.GetCount();
	for (weaponGroupsIndex=0;weaponGroupsIndex<weaponGroupsCount;weaponGroupsIndex++)
	{
		weaponGroup=weaponGroups.Item(weaponGroupsIndex);
		weaponGroupContents=new wxArrayString();

		*weaponGroupContents=globalStatistics.configData.ReadWeaponGroup(weaponGroup);
		weaponGroupsContents.Add(weaponGroupContents);
	}

	cacheOpen=true;
}

void WeaponGroupCache::Close()
{
	int				weaponGroupsCount;
	int				weaponGroupsIndex;
	wxString		weaponGroup;
	wxArrayString	*weaponGroupContents;

	if (!cacheOpen)
	{
		return;
	}
	cacheOpen=false;

	weaponGroupsCount=weaponGroups.GetCount();
	for (weaponGroupsIndex=0;weaponGroupsIndex<weaponGroupsCount;weaponGroupsIndex++)
	{
		weaponGroupContents=(wxArrayString *)weaponGroupsContents.Item(weaponGroupsIndex);
		delete weaponGroupContents;
	}
	weaponGroupsContents.Clear();
	weaponGroups.Clear();
}

wxArrayString *WeaponGroupCache::ReadWeaponGroup(wxString &weaponGroup)
{
	int				groupIndex;
	wxArrayString	*weaponGroupContents;

	groupIndex=weaponGroups.Index(weaponGroup);
	if (groupIndex==wxNOT_FOUND)
	{
		weaponGroupContents=NULL;
	}
	else
	{
		weaponGroupContents=(wxArrayString *)weaponGroupsContents.Item(groupIndex);
	}

	return (weaponGroupContents);
}

bool WeaponGroupCache::WeaponMatches(wxString &weaponKey,wxString &weaponGroupOrKey)
{
	wxArrayString	*weaponGroupKeys;
	bool			retVal=false;
	int				groupCount;
	int				groupIndex;
	wxString		group;

	Open();
	if (globalStatistics.configData.IsWeaponGroupKey(weaponGroupOrKey))
	{
		weaponGroupKeys=ReadWeaponGroup(weaponGroupOrKey);
		if (weaponGroupKeys==NULL)
		{
			groupCount=0;
		}
		else
		{
			groupCount=weaponGroupKeys->GetCount();
		}
		for (groupIndex=0;groupIndex<groupCount;groupIndex++)
		{
			group=weaponGroupKeys->Item(groupIndex);
			retVal|=WeaponMatches(weaponKey,group);
			if (retVal)
			{
				break;
			}
		}
	}
	else
	{
		retVal=(weaponKey.CmpNoCase(weaponGroupOrKey)==0);
	}
	return (retVal);
}


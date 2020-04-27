#ifndef __CONFIGDATA
#define __CONFIGDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>

// Statsgen Includes
#include "ConfigItem.h"

#define CONFIG_EXPORT_SET_ONLY		0
#define CONFIG_EXPORT_ALL			1

#define CONFIG_ANSWER_TYPE_ASK		0
#define CONFIG_ANSWER_TYPE_USE_LAST	1

#define UPGRADE_TYPE_CONFIG			"Configuration File"
#define UPGRADE_TYPE_DATABASE		"Database Contents"
#define UPGRADE_TYPE_UNKNOWN		"Unknown"

#define CONFIG_WEAPON_GROUP_PREFIX	"weapongroup_"

class WeaponGroupCache
{
	public:
		WeaponGroupCache();
		~WeaponGroupCache();
		void Open();
		void Close();
		bool WeaponMatches(wxString &weaponKey,wxString &weaponGroupOrKey);
		wxArrayString *ReadWeaponGroup(wxString &weaponGroupOrKey);
	private:
		bool			cacheOpen;
		wxArrayString	weaponGroups;
		wxArrayPtrVoid	weaponGroupsContents;
		
};

class ConfigData
{
	public:
		ConfigData();
		void Initiate();
		void Initiate(wxFileName &iniFilename);
		virtual ~ConfigData();
		void UpgradeDetails(wxString &description,wxString &upgradeType);
		void Export(wxArrayString &upgradeList,
					ConfigData &sourceConfig,
					char *description,
					char *group,
					int exportType,
					int exportAnswerType,
					char *recommendation);

		void CommitChanges();
		void RollbackChanges();
		void EmptyCache();
		bool ReadTextValue(char *key,wxString *value);
		bool ReadTextValue(wxString &key,wxString *value);
		bool ReadTextValue(char *key,wxString *value,char *defaultValue);
		bool ReadTextValue(wxString &key,wxString *value,char *defaultValue);
		void WriteTextValue(wxString &key,wxString &value);
		void WriteTextValue(char *key,wxString &value);
		void WriteTextValue(wxString &key,char *value);
		void WriteTextValue(char *key,char *value);
		bool ReadList(char *group,wxArrayString &list);
		bool ReadList(wxString &group,wxArrayString &list);

		bool ReadList(char *group,wxString &subGroup,wxArrayString &list);
		bool ReadList(wxString &group,wxString &subGroup,wxArrayString &list);
		bool ReadList(char *group,char *subGroup,wxArrayString &list);
		bool ReadList(wxString &group,char *subGroup,wxArrayString &list);

		bool WriteList(char *group,wxArrayString &list);
		bool WriteList(wxString &group,wxArrayString &list);

		bool WriteList(char *group,char *subGroup,wxArrayString &list);
		bool WriteList(wxString &group,char *subGroup,wxArrayString &list);

		bool WriteList(char *group,wxString &subGroup,wxArrayString &list);
		bool WriteList(wxString &group,wxString &subGroup,wxArrayString &list);
		bool ReadGroup(char *group,
						wxArrayString &keys,
						wxArrayString &values);
		bool ReadGroup(wxString &group,
						wxArrayString &keys,
						wxArrayString &values);
		bool DeleteGroup(wxString &group);
		bool DeleteGroup(char *group);
		void ReadGroupList(wxArrayString &listOfGroups);
		wxArrayString ReadWeaponGroups();
		wxArrayString ReadWeaponGroup(wxString &weaponGroup);
		bool WeaponMatches(wxString &weaponKey,wxString &weaponGroupOrKey);
		bool IsWeaponGroupKey(wxString &key);
		wxArrayString WeaponGroupKeys(wxString &key);
		void Close();
		void OpenWeaponGroupCache();
		void CloseWeaponGroupCache();

	private:
		int FindConfigCacheEntry(wxString &key);

	private:
		void PurgeDeletedEntry(wxString &configKey);

		ArrayOfConfigItem	configKeys;
		wxArrayString		groupsDeleted;
		wxFileConfig		*configFile;
		wxFileName			iniFilename;
		WeaponGroupCache	weaponGroupCache;
};

#endif

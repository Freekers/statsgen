#ifndef __AWARDDEFINITION
#define __AWARDDEFINITION

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class AwardDefinition;

WX_DECLARE_OBJARRAY(AwardDefinition,ArrayOfAwardDefinition);
class AwardDefinition
{
	public:
		void ReadConfig();
		void WriteConfig();
		AwardDefinition(wxString &idIn);
//		bool Allowed(wxString &gameType);
		virtual ~AwardDefinition();
		static wxString SQLCreateTable();
		static wxString SQLTableNameComponent(const char *component);
		static wxString SQLCreateTableComponent(const char *component);
		static wxString SQLTableName();
		bool WriteToDatabase(int itemIndex);
		bool WriteToDatabaseComponent(int itemIndex,const char *component,wxArrayString &componentList,const char *prefix);
		int WeaponScore(bool *noScore,wxString &key,bool kill,bool teamkill);
		int XPScore(bool *noScore,wxString &key,float points);
		int LocationScore(bool *noScore,wxString &key,bool kill,bool teamkill);
		int ActionScore(bool *noScore,wxString &key);
		int MiscScore(bool *noScore,char *key,int count);
		void SplitCode(wxString &code,bool *positive,bool *teamkill);

		void AddWeaponComponent(wxString &code,bool kill,bool positive,bool teamkill);
		void AddLocationComponent(wxString &code,bool kill,bool positive,bool teamkill);
		void AddActionComponent(wxString &code,bool positive);
		void AddMiscComponent(wxString &code,bool positive);
		void AddXPComponent(wxString &code,bool positive);

		void RemoveWeaponComponent(wxString &code,bool killList,bool teamkillList);
		void RemoveLocationComponent(wxString &code,bool killList,bool teamkillList);
		void RemoveActionComponent(wxString &code);
		void RemoveMiscComponent(wxString &code);
		void RemoveXPComponent(wxString &code);

		wxArrayString	allowedGameTypes;
		wxString		image;
		wxString		name;
		bool			weighted;
		wxArrayString	weaponKillComponents;
		wxArrayString	weaponDeathComponents;
		wxArrayString	weaponTKComponents;
		wxArrayString	locationKillComponents;
		wxArrayString	locationDeathComponents;
		wxArrayString	locationTKComponents;
		wxArrayString	actionComponents;
		wxArrayString	xpComponents;
		wxArrayString	miscComponents;

		wxString		id;
		
};


#endif

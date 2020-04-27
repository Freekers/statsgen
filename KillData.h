#ifndef __KILLDATA
#define __KILLDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class KillData;

enum KillTypes
{
	KILL_TYPE_KILL=0,
	KILL_TYPE_DEATH,
	KILL_TYPE_TEAMKILL,
	KILL_TYPE_TEAMKILLVICTIM,
	KILL_TYPE_TEAMSWAP,
	KILL_TYPE_SUICIDE
};

#define KILL_TYPE_STRING_UNKNOWN	"UNKNOWN"
#define KILL_TYPE_STRING_KILL		"KILL"
#define KILL_TYPE_STRING_DEATH		"DEATH"
#define KILL_TYPE_STRING_TEAMKILL	"TEAMKILL"
#define KILL_TYPE_STRING_TEAMKILLVICTIM	"TEAMKILLVICTIM"
#define KILL_TYPE_STRING_SUICIDE	"SUICIDE"
#define KILL_TYPE_STRING_TEAMSWAP	"TEAMSWAP"

WX_DECLARE_OBJARRAY(KillData,ArrayOfKillData);
class KillData
{
	public:
		KillData();
		virtual ~KillData();
		KillTypes KillType(int gameType,int playerIndex);
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int roundIndex,int itemIndex);
		char *KillTypeText();

		
		wxDateTime	killTime;
		int			gameType;
		int			mapName;
		int			playerIndex;
		int			playerClass;
		int			playerTeam;
		int			playerWeapon;
		int			playerAmmo;
		int			targetIndex;
		int			targetClass;
		int			targetTeam;
		int			targetDamage;
		int			targetLocation;
		
};


#endif

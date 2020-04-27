#ifndef __STATSGENDATABASE
#define __STATSGENDATABASE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>

// Statsgen Includes
#include "sqlite3.h"
#include "KillTotal.h"
//#include "libsqlitewrapped.h"

// #defines
#define	TABLE_NAME_KEY_AMMO	"ammo"
#define	TABLE_NAME_KEY_ACTION	"action"
#define	TABLE_NAME_KEY_WEAPON	"weapon"
#define	TABLE_NAME_KEY_MAP	"map"
#define	TABLE_NAME_KEY_TEAM	"team"
#define	TABLE_NAME_KEY_CLASS	"class"
#define	TABLE_NAME_KEY_GAMETYPE	"gametype"
#define	TABLE_NAME_KEY_LOCATION	"location"
#define	TABLE_NAME_KEY_XP	"xp"

#define FIELD_WIDTH_MISC_STATISTIC		50
#define FIELD_WIDTH_MISC_VALUE			50
#define FIELD_WIDTH_KEY_ID				32
#define FIELD_WIDTH_KEY_REALNAME		100
#define FIELD_WIDTH_KEY_IMAGE			150
#define FIELD_WIDTH_PLAYER_DATA			150
#define FIELD_WIDTH_PLAYER_NAME			50
#define FIELD_WIDTH_SERVER_TYPE			10
#define FIELD_WIDTH_HOSTNAME			50
#define FIELD_WIDTH_VARIABLE			50
#define FIELD_WIDTH_VALUE				50
#define FIELD_WIDTH_CLAN_TAG			10
#define FIELD_WIDTH_CLAN_NAME			50
#define FIELD_WIDTH_CLAN_WEBADDRESS		150
#define FIELD_WIDTH_CLAN_BANNER			150
#define FIELD_WIDTH_PLAYER_NAME			50
#define FIELD_WIDTH_PLAYER_GUID			50
#define FIELD_WIDTH_ROUND_START_TIME	20
#define FIELD_WIDTH_KILL_TYPE			14
#define FIELD_WIDTH_SPEECH				80
#define FIELD_WIDTH_AWARD_ID			32
#define FIELD_WIDTH_AWARD_NAME			50
#define FIELD_WIDTH_AWARD_IMAGE			150
class StatsgenDatabase
{
	public:
		static wxString StringFieldDefinition(const char *fieldName,const char *key,int defaultFieldWidth);
		StatsgenDatabase();
		virtual ~StatsgenDatabase();
		wxString Filename();
		void Create();
		void Empty();
		bool SimpleExecute(wxString &sql);
		bool OpenDB();
		bool CloseDB();
		void Zap();
		void ApplyAliases(wxArrayInt &changedPlayers);
		static wxString SafeForInsert(wxString &text);
		sqlite3	*DBHandle();
		bool WriteMiscellaneousStatistic(char *variable,wxString &value);
		bool WriteMiscellaneousStatistic(char *variable,long value);
		bool WriteMiscellaneousStatistic(char *variable,int value);
		void GetDatabaseComponent(const char *componentType,
								wxArrayString &componentNames,
								wxArrayString &componentSQL);

		void CreateIndexes();
		void DropIndexes();
		int RetrieveTableCount(const char *tableName,const char *fieldName);
		void UpdatePlayerCount();
		void UpdateRoundCount();
		int NextPlayerIndex();
		int NextRoundIndex();
		void CreateIndexListTable(const char *tableName);
		void CreateChangedPlayersTable();
		void CreateChangedRoundsTable();
		void EmptyIndexListTable(const char *tableName);
		void EmptyChangedPlayersTable();
		void EmptyChangedRoundsTable();
		void WriteIndexListToTable(const char *tableName,wxArrayInt &list);
		void WriteChangedPlayersList(wxArrayInt &listOfPlayerIndexes);
		void WriteChangedRoundsList(wxArrayInt &listOfRoundIndexes);
		void WriteChangedRoundsList();

		void UpdatePlayerKillTotal(int playerIndex,
								int databasePlayerIndex,
								const char *tableNameSuffix,
								ArrayOfKillTotal &killTotals);
		void UpdatePlayerKillTotals(int playerIndex,
								int databasePlayerIndex,
								ArrayOfKillTotal &maps,
								ArrayOfKillTotal &gameTypes,
								ArrayOfKillTotal &weapons,
								ArrayOfKillTotal &locations,
								ArrayOfKillTotal &teams,
								ArrayOfKillTotal &classes);
		void Optimize();
	private:
		void CreateViews();
		void CreateTotalListView(const char *totalSuffix,
					const char *indextable);
		void CreateKillTotalIndexes(const char *totaltype);
		bool CreateGenericKeyTable(char *tableName);
		bool EmptyGenericKeyTable(char *tableName);

		wxString		databaseFilename;
		sqlite3			*sqliteDBHandle;
		int				roundCount;
		int				playerCount;
	
};


#endif

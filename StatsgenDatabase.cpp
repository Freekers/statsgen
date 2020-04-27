#include <stdint.h>

// wxWindows includes
#include <wx/string.h>

// Statsgen includes
#include "ErrorData.h"
#include "StatsgenDatabase.h"
#include "KillData.h"
#include "ActionData.h"
#include "AKAListEntry.h"
#include "Round.h"
#include "TeamWinData.h"
#include "TeamLossData.h"
#include "SpeechData.h"
#include "PlayerInRound.h"
#include "GenericKey.h"
#include "Player.h"
#include "Clan.h"
#include "GlobalStatistics.h"
#include "Progress.h"
#include "TemplateOpenQuery.h"
#include "StaticFunctions.h"


StatsgenDatabase::StatsgenDatabase()
{
	wxString	configKey;
	
	sqliteDBHandle=NULL;
	roundCount=0;
	playerCount=0;

//	configKey="/Database/filename";
//	globalStatistics.configData.ReadTextValue(configKey,
//						&databaseFilename,
//						":memory:");

}

StatsgenDatabase::~StatsgenDatabase()
{
	CloseDB();
}

void StatsgenDatabase::CreateIndexListTable(const char *tableName)
{
	wxString	SQL;

	SQL.Printf("create table %s ( idx integer )",tableName);
	SimpleExecute(SQL);
}
void StatsgenDatabase::CreateChangedPlayersTable()
{
	CreateIndexListTable("changedplayers");
}
void StatsgenDatabase::CreateChangedRoundsTable()
{
	CreateIndexListTable("changedrounds");
}

void StatsgenDatabase::EmptyIndexListTable(const char *tableName)
{
	wxString	SQL;

	SQL.Printf("delete from %s",tableName);
	SimpleExecute(SQL);
}
void StatsgenDatabase::EmptyChangedPlayersTable()
{
	EmptyIndexListTable("changedplayers");
}
void StatsgenDatabase::EmptyChangedRoundsTable()
{
	EmptyIndexListTable("changedrounds");
}

void StatsgenDatabase::WriteIndexListToTable(const char *tableName,wxArrayInt &list)
{
	int 		listCount;
	int			listIndex;
	int			index;
	wxString	SQL;
	wxString	statusStr;
	bool		openCommit=false;

	listCount=list.GetCount();
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		if ((listIndex % 100) == 0)
		{
			if (openCommit)
			{
				SQL="commit";
				SimpleExecute(SQL);
			}
			SQL="begin";
			SimpleExecute(SQL);
			openCommit=true;
			statusStr.Printf("Writing %s Index %d of %d",tableName,listIndex,listCount);
			progress->SetStatus(statusStr);
		}
		index=list.Item(listIndex);
		SQL.Printf("insert into %s "
			"(idx)"
			"values"
			"('%d')",
			tableName,
			index);
		SimpleExecute(SQL);
	}
	if (openCommit)
	{
		SQL="commit";
		SimpleExecute(SQL);
	}
}

void StatsgenDatabase::WriteChangedPlayersList(wxArrayInt &listOfPlayerIndexes)
{
	WriteIndexListToTable("changedplayers",listOfPlayerIndexes);
}

void StatsgenDatabase::WriteChangedRoundsList(wxArrayInt &listOfRoundIndexes)
{
	WriteIndexListToTable("changedrounds",listOfRoundIndexes);
}

void StatsgenDatabase::WriteChangedRoundsList()
{
	wxString	sql="insert into changedrounds select roundindex from playerinround where playerindex in (select idx from changedplayers) group by roundindex";

	SimpleExecute(sql);
}

bool StatsgenDatabase::CreateGenericKeyTable(char *tableName)
{
	wxString	SQL;
	wxString	errorString;
	bool		retVal=true;

	SQL=GenericKey::SQLCreateTable(tableName);
	SimpleExecute(SQL);

	return retVal;
}

bool StatsgenDatabase::CloseDB()
{
	if (sqliteDBHandle!=NULL)
	{
		sqlite3_close(sqliteDBHandle);
		sqliteDBHandle=NULL;
	}

	return (true);
}

bool StatsgenDatabase::OpenDB()
{
	wxString	msg;
	wxString	configKey;
	bool		retVal=true;
	int			status;

	STATSGEN_DEBUG_FUNCTION_START("StatsgenDatabase","OpenDB")
	STATSGEN_DEBUG_CODE(msg.Printf("SQLITE3 Version [%s]",sqlite3_libversion());)
	STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
	if (sqliteDBHandle!=NULL)
	{
	}
	else
	{
		wxFileName	dbFilename;
		wxString	defaultDBFilename;

		dbFilename=BaseDirectoryFileName("statsgen2.db");
		defaultDBFilename=dbFilename.GetFullPath();
		configKey="/Database/filename";
		globalStatistics.configData.ReadTextValue(configKey,
						&databaseFilename,
						defaultDBFilename);
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Opening DB")
		status=sqlite3_open(STRING_TO_CHAR(databaseFilename),
							&sqliteDBHandle);
		if (status!=SQLITE_OK)
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Opening Database [%s]", sqlite3_errmsg(sqliteDBHandle));)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			progress->LogError(msg,SeverityError);
			retVal=false;
		}
		else
		{
//			STATSGEN_DEBUG(DEBUG_SOMETIMES,"Opening Database:Success")
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

wxString StatsgenDatabase::Filename()
{
	return(databaseFilename);
}

void StatsgenDatabase::Create()
{
	wxString	errorString;
	wxString	SQL;
	long		topValue=22;
	long		step=0;
	static bool	alreadyCreated=false;

	progress->Initiate(topValue,
			(char *)" Steps",
			(long)1,
			(char *)" Steps",
			(long)1);
	progress->DisableTimeToGo();
	OpenDB();
	progress->Update(step++);

	if (!alreadyCreated)
	{
		alreadyCreated=true;
		CreateChangedPlayersTable();
		progress->Update(step++);progress->SetStatus((char *)"Changed Players");
		CreateChangedRoundsTable();
		progress->Update(step++);progress->SetStatus((char *)"Changed Rounds");
		// Key Tables (weapons, maps, teams etc)
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_ACTION);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Actions");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_AMMO);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Ammo");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_WEAPON);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Weapons");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_MAP);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Maps");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_TEAM);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Teams");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_CLASS);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Classes");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_GAMETYPE);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Gametypes");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_LOCATION);
		progress->Update(step++);progress->SetStatus((char *)"Keys:Locations");
		CreateGenericKeyTable((char *)TABLE_NAME_KEY_XP);
		progress->Update(step++);progress->SetStatus((char *)"Keys:XP");
	
		// Cheaters
		globalStatistics.cheaters.CreateDatabase();
		progress->Update(step++);progress->SetStatus((char *)"Cheats");

		// Skill Weights
		globalStatistics.skillWeights.CreateDatabase();
		progress->Update(step++);progress->SetStatus((char *)"Skill Weights");
		// Score Weights
		globalStatistics.scoreWeights.CreateDatabase();
		progress->Update(step++);progress->SetStatus((char *)"Score Weights");

		// Avatars / Player Data
		globalStatistics.playerDataAvatars->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Avatars");
		// Avatars / Player Data
		globalStatistics.playerDataPicture->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Picture");
		// Avatars / Player Data
		globalStatistics.playerDataWebsite->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Website");
		// Avatars / Player Data
		globalStatistics.playerDataXfire->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:XFire");
		// Avatars / Player Data
		globalStatistics.playerDataClanRole->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Clan Role");
		// Avatars / Player Data
		globalStatistics.playerDataRealName->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Real Name");
		// Avatars / Player Data
		globalStatistics.playerDataMisc1->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Misc 1");
		// Avatars / Player Data
		globalStatistics.playerDataMisc2->CreateDatabaseTable();
		progress->Update(step++);progress->SetStatus((char *)"Player Data:Misc 2");
	
		// Create progressbar Table
		SQL=ProgressPanel::CreateTableSQL();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"progressbar");

		// Create aka Table
		SQL=AKAListEntry::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"AKA List");
	
		// Create killtotals Table
		SQL=KillTotal::SQLCreateTable("maps");
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kill Totals:Maps");
	
		// Create killtotals Table
		SQL=KillTotal::SQLCreateTable("gametypes");
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kill Totals:Game Types");
	
		// Create killtotals Table
		SQL=KillTotal::SQLCreateTable("weapons");
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kill Totals:Weapons");
	
		// Create killtotals Table
		SQL=KillTotal::SQLCreateTable("locations");
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kill Totals:Locations");
	
		// Create killtotals Table
		SQL=KillTotal::SQLCreateTable("teams");
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kill Totals:Teams");
	
		// Create killtotals Table
		SQL=KillTotal::SQLCreateTable("classes");
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kill Totals:Classes");

		// Create Server Table
		SQL=Server::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Servers");
	
		// Create Server Variables Table
		SQL=Server::SQLCreateVariableTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Server Variables");

		// Create Server Variables Table
		SQL=Server::SQLCreateProgressTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Server Progress");

		// Create Clans Table
		SQL=Clan::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Clans");
	
		// Create Players Table
		SQL=Player::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Players");
	
		// Create appropriate data tables
		SQL=Round::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Rounds");

		// Create appropriate data tables
		SQL=Round::SQLCreateVariableTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Round Variables");

		SQL=KillData::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Kills");

		SQL=TeamWinData::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Team Wins");

		SQL=TeamLossData::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Team Losses");

		SQL=ActionData::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Actions");

		SQL=SpeechData::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Speech");
	
		SQL=PlayerInRound::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Players In Rounds");
	
		SQL=PlayerInRound::SQLCreateTableAward();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Award Points");
	
		SQL=XPPoints::SQLTotalCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Total XP Points");
	
		SQL=XPPoints::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"XP Points");
	
		SQL=StreakData::SQLCreateTable();
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Streaks");
	
		SQL=AwardDefinition::SQLCreateTable();
		SimpleExecute(SQL);
	
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"weaponkill");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"weapondeath");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"weapontk");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"locationkill");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"locationdeath");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"locationtk");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"action");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"misc");SimpleExecute(SQL);
		SQL=AwardDefinition::SQLCreateTableComponent((char *)"xp");SimpleExecute(SQL);

		progress->Update(step++);progress->SetStatus((char *)"Award Definitions");

		// Miscellaneous Statistics
		SQL.Printf("create table miscellaneous "
			"("
			"%s," 
			"%s" 
			")",
			STRING_TO_CHAR(StringFieldDefinition("statistic","miscstatistic",FIELD_WIDTH_MISC_STATISTIC)),
			STRING_TO_CHAR(StringFieldDefinition("value","miscvalue",FIELD_WIDTH_MISC_VALUE)));
		SimpleExecute(SQL);
		progress->Update(step++);progress->SetStatus((char *)"Miscellaneous");
//		CreateIndexes();
//		progress->Update(step++);progress->SetStatus((char *)"Indexes");
		CreateViews();
		progress->Update(step++);progress->SetStatus((char *)"Views");
		CreateIndexes();
	}
}

bool StatsgenDatabase::WriteMiscellaneousStatistic(char *statistic,int value)
{
	wxString	valueStr;

	valueStr.Printf("%d",value);
	return (WriteMiscellaneousStatistic(statistic,valueStr));
}

bool StatsgenDatabase::WriteMiscellaneousStatistic(char *statistic,long value)
{
	wxString	valueStr;

	valueStr.Printf("%ld",value);
	return (WriteMiscellaneousStatistic(statistic,valueStr));
}
bool StatsgenDatabase::WriteMiscellaneousStatistic(char *statistic,wxString &value)
{
	wxString	safeValue;
	wxString	SQL;

	safeValue=SafeForInsert(value);

	SQL="begin";
	SimpleExecute(SQL);

	SQL.Printf("insert into miscellaneous "
		"(statistic,value)"
		"values"
		"('%s','%s')",
		statistic,
		STRING_TO_CHAR(safeValue));
	SimpleExecute(SQL);

	SQL="commit";
	SimpleExecute(SQL);

	return (true);
}

bool StatsgenDatabase::SimpleExecute(wxString &SQL)
{
	wxString	errorString;
	bool		retVal;
	int			status;
	char		*errorMessage;
	wxString	err;
	

	STATSGEN_DEBUG_FUNCTION_START("StatsgenDatabase","SimpleExecute")
	// Create appropriate tables

//	StatsgenDebug(SQL);
	if (sqliteDBHandle==NULL)
	{
		retVal=false;
	}
	else
	{
	status=sqlite3_exec(sqliteDBHandle,
						STRING_TO_CHAR(SQL),
						NULL,
						NULL,
						&errorMessage);
	if (status!=SQLITE_OK)
	{
		err=errorMessage;
		if (!err.EndsWith(" already exists"))
		{
			errorString.Printf("ERROR [%s][%s]",STRING_TO_CHAR(SQL),errorMessage);
			STATSGEN_DEBUG(DEBUG_ALWAYS,errorString)
			progress->LogError(errorString,SeverityError);
		}
		sqlite3_free(errorMessage);
	}
	else
	{
//		STATSGEN_DEBUG(DEBUG_RARELY,"SUCCESS")
	}
	retVal=true;
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

wxString StatsgenDatabase::SafeForInsert(wxString &text)
{
	STATSGEN_DEBUG_FUNCTION_START("StatsgenDatabase","SafeForInsert")
	wxString	retVal;

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"text");
	STATSGEN_DEBUG(DEBUG_RARELY,text);
	retVal=text;

	retVal.Replace((char *)"'",(char *)"''");
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"result");
	STATSGEN_DEBUG(DEBUG_RARELY,retVal);

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void StatsgenDatabase::Empty()
{
	wxString	errorString;
	wxString	SQL;
	long		topValue=20;
	long		step=0;

	progress->Initiate(topValue,
				(char *)" Steps",
				(long)1,
				(char *)" Steps",
				(long)1);
	progress->DisableTimeToGo();
	OpenDB();
	progress->Update(step++);progress->SetStatus((char *)"Starting");

	EmptyChangedPlayersTable();
	progress->Update(step++);progress->SetStatus((char *)"Changed Players");
	EmptyChangedRoundsTable();
	progress->Update(step++);progress->SetStatus((char *)"Changed Rounds");
	// Key Tables (weapons, maps, teams etc)
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_ACTION);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Actions");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_AMMO);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Ammo");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_WEAPON);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Weapons");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_MAP);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Maps");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_TEAM);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Teams");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_CLASS);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Classes");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_GAMETYPE);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Game Types");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_LOCATION);
	progress->Update(step++);progress->SetStatus((char *)"Keys:Locations");
	EmptyGenericKeyTable((char *)TABLE_NAME_KEY_XP);
	progress->Update(step++);progress->SetStatus((char *)"Keys:XP");

	// Empty Total XP
	// SQL.Printf("delete from %s",XPPoints::SQLTotalTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Total XP List");

	// Empty XP
	// SQL.Printf("delete from %s",XPPoints::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("XP List");

	// Empty Cheaters
	// SQL.Printf("delete from %s",Cheaters::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Cheaters List");

	// Empty AKA
	// SQL.Printf("delete from %s",AKAListEntry::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("AKA List");

	// Empty Skill Weights
	globalStatistics.skillWeights.EmptyDatabase();
	progress->Update(step++);progress->SetStatus((char *)"Skill Weights");

	// Empty Score Weights
	globalStatistics.scoreWeights.EmptyDatabase();
	progress->Update(step++);progress->SetStatus((char *)"Score Weights");

	// Empty Avatars / Player Data
	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataAvatars->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Avatars");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataPicture->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Pictures");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataWebsite->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Website");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataXfire->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:XFire");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataClanRole->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Clan Roles");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataRealName->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Real Names");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataMisc1->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Misc 1");

	SQL.Printf("delete from %s",STRING_TO_CHAR(globalStatistics.playerDataMisc2->SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Player Data:Misc 2");

	// Empty kill totals
	// SQL.Printf("delete from %s",KillTotal::SQLTableName("maps").GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kill Totals:Maps");

	// Empty kill totals
	// SQL.Printf("delete from %s",KillTotal::SQLTableName("gametypes").GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kill Totals:Game Types");

	// Empty kill totals
	// SQL.Printf("delete from %s",KillTotal::SQLTableName("weapons").GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kill Totals:Weapons");

	// Empty kill totals
	// SQL.Printf("delete from %s",KillTotal::SQLTableName("locations").GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kill Totals:Locations");

	// Empty kill totals
	// SQL.Printf("delete from %s",KillTotal::SQLTableName("teams").GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kill Totals:Teams");

	// Empty kill totals
	// SQL.Printf("delete from %s",KillTotal::SQLTableName("classes").GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kill Totals:Classes");

	// Empty Server Variables Table
	// SQL.Printf("delete from %s",Server::SQLVariableTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Server Variables");

	// Empty Server Table
	SQL.Printf("delete from %s",STRING_TO_CHAR(Server::SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Servers");

	// Create Clans Table
	SQL.Printf("delete from %s",STRING_TO_CHAR(Clan::SQLTableName()));
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Clans");

	// Create Players Table
	// SQL.Printf("delete from %s",Player::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Players");

	// Create appropriate data tables
	// SQL.Printf("delete from %s",Round::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Rounds");

	// SQL.Printf("delete from %s",Round::SQLVariableTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Round Variables");

	// SQL.Printf("delete from %s",KillData::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Kills");

	// SQL.Printf("delete from %s",TeamWinData::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Team Wins");

	// SQL.Printf("delete from %s",TeamLossData::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Team Losses");

	// SQL.Printf("delete from %s",ActionData::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Actions");

	// SQL.Printf("delete from %s",SpeechData::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Speech");

	// SQL.Printf("delete from %s",PlayerInRound::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Players In Rounds");

	// SQL.Printf("delete from %s",PlayerInRound::SQLTableNameAward().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Award Points");

	// SQL.Printf("delete from %s",StreakData::SQLTableName().GetData());
	// SimpleExecute(SQL);
	// progress->Update(step++);progress->SetStatus("Streaks");

	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableName()));
	SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"weaponkill")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"weapondeath")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"weapontk")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"locationkill")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"locationdeath")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"locationtk")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"action")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"misc")));SimpleExecute(SQL);
	SQL.Printf("delete from %s",STRING_TO_CHAR(AwardDefinition::SQLTableNameComponent((char *)"xp")));SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Award Definition");

	SQL="delete from miscellaneous";
	SimpleExecute(SQL);
	progress->Update(step++);progress->SetStatus((char *)"Miscellaneous");
}

sqlite3 *StatsgenDatabase::DBHandle()
{
	return (sqliteDBHandle);
}

bool StatsgenDatabase::EmptyGenericKeyTable(char *tableName)
{
	wxString	SQL;
	wxString	errorString;
	bool		retVal=true;

	SQL.Printf("delete from %s",tableName);
	SimpleExecute(SQL);

	return retVal;
}

void StatsgenDatabase::CreateViews()
{
	wxString	SQL;
	wxString	errorString;

	SQL="create view VIEW_KILLDATA as"
		" select "
		"killdata.roundindex as roundindex,"
		"killdata.killidx as killidx,"
		"killdata.killtype as killtype,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.playerindex as playerindex,"
		"playerclass.id as playerclassid,"
		"playerclass.realname as playerclassname,"
		"playerclass.image as playerclassimage,"
		"playerteam.id as playerteamid,"
		"playerteam.realname as playerteamname,"
		"playerteam.image as playerteamimage,"
		"playerweapon.id as playerweaponid,"
		"playerweapon.realname as playerweaponname,"
		"playerweapon.image as playerweaponimage,"
		"target.name as targetname,"
		"target.id as targetid,"
		"target.clanindex as targetclanindex,"
		"target.playerindex as targetindex,"
		"targetclass.id as targetclassid,"
		"targetclass.realname as targetclassname,"
		"targetclass.image as targetclassimage,"
		"targetteam.id as targetteamid,"
		"targetteam.realname as targetteamname,"
		"targetteam.image as targetteamimage,"
		"targetlocation.id as targetlocationid,"
		"targetlocation.realname as targetlocationname,"
		"targetlocation.image as targetlocationimage"
		" from "
		" killdata,"
		"player,"
		"class as playerclass,"
		"team as playerteam,"
		"weapon as playerweapon,"
		"player as target,"
		"class as targetclass,"
		"team as targetteam,"
		"location as targetlocation"
		" where "
		"killdata.playerindex=player.playerindex and "
		"killdata.playerclass=playerclass.idx and "
		"killdata.playerteam=playerteam.idx and "
		"killdata.playerweapon=playerweapon.idx and "
		"killdata.targetindex=target.playerindex and "
		"killdata.targetclass=targetclass.idx and "
		"killdata.targetteam=targetteam.idx and "
		"killdata.targetlocation=targetlocation.idx"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_AWARDPOINTS as"
		" select "
		"awardpoints.roundindex as roundindex,"
		"awardpoints.playerindex as playerindex,"
		"awardpoints.awardindex as awardindex,"
		"awardpoints.score as score,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex"
		" from "
		" awardpoints,"
		" player"
		" where "
		"awardpoints.playerindex=player.playerindex"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_PLAYERINROUND as"
		" select "
		"playerinround.roundindex as roundindex,"
		"playerinround.playerindex as playerindex,"
		"playerinround.score as score,"
		"playerinround.skill as skill,"
		"playerinround.kills as kills,"
		"playerinround.deaths as deaths,"
		"playerinround.teamkills as teamkills,"
		"playerinround.suicides as suicides,"
		"playerinround.duration as duration,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.dropped as dropped"
		" from "
		" playerinround,"
		" player"
		" where "
		"playerinround.playerindex=player.playerindex AND "
		"dropped='N'"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_PLAYER as"
		" select "
		" * "
		" from "
		" player"
		" where "
		"player.dropped='N'"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_ROUNDS as"
		" select "
		"rounds.roundidx as roundidx,"
		"rounds.starttime as starttime,"
		"rounds.duration as duration,"
		"rounds.gametype as gametype,"
		"rounds.mapname as mapname,"
		"servers.serveridx as serverindex,"
		"servers.servertype as servertype,"
		"servers.logfilesize as logfilesize,"
		"servers.hostname as hostname,"
		"servers.gameport as gameport,"
		"map.id as mapid,"
		"map.realname as maprealname,"
		"map.image as mapimage,"
		"gametype.id as gametypeid,"
		"gametype.realname as gametyperealname,"
		"gametype.image as gametypeimage"
		" from "
		" map,"
		" gametype,"
		" rounds,"
		" servers"
		" where "
		"servers.serveridx=rounds.serverindex AND "
		"rounds.dropped='N' AND "
		"rounds.gametype=gametype.idx AND "
		"rounds.mapname=map.idx"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_ACTIONDATA as"
		" select "
		"actiondata.roundindex as roundindex,"
		"actiondata.actionidx as actionindex,"
		"actiondata.playerclass as classidx,"
		"actiondata.playerteam as teamidx,"
		"actiondata.action as actionidx,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.playerindex as playerindex,"
		"class.realname as playerclassname,"
		"class.image as playerclassimage,"
		"team.realname as playerteamname,"
		"team.image as playerteamimage,"
		"action.realname as actionname,"
		"action.image as actionimage "
		" from "
		" actiondata,"
		"player,"
		"class as class,"
		"team as team,"
		"action as action "
		" where "
		"actiondata.playerindex=player.playerindex and "
		"actiondata.playerclass=class.idx and "
		"actiondata.playerteam=team.idx and "
		"actiondata.action=action.idx"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_XPTOTALPOINTS as"
		" select "
		"xptotalpoints.pointsindex as pointsindex,"
		"xptotalpoints.points as points,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.playerindex as playerindex,"
		"xp.realname as xpname,"
		"xp.id as xpid,"
		"xp.image as xpimage "
		" from "
		" xptotalpoints,"
		"player,"
		"xp as xp "
		" where "
		"xptotalpoints.playerindex=player.playerindex and "
		"player.dropped='N' and "
		"xptotalpoints.pointsindex=xp.idx"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_XPPOINTS as"
		" select "
		"xppoints.roundindex as roundindex,"
		"xppoints.pointsindex as pointsindex,"
		"xppoints.points as points,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.playerindex as playerindex,"
		"xp.realname as xpname,"
		"xp.id as xpid,"
		"xp.image as xpimage "
		" from "
		" xppoints,"
		"player,"
		"xp as xp "
		" where "
		"xppoints.playerindex=player.playerindex and "
		"player.dropped='N' and "
		"xppoints.pointsindex=xp.idx"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_TEAMWINDATA as"
		" select "
		"teamwindata.roundindex as roundindex,"
		"teamwindata.teamwinidx as teamwinidx,"
		"teamwindata.playerclass as classidx,"
		"teamwindata.playerteam as teamidx,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.playerindex as playerindex,"
		"class.realname as playerclassname,"
		"class.image as playerclassimage,"
		"team.realname as playerteamname,"
		"team.image as playerteamimage "
		" from "
		" teamwindata,"
		"player,"
		"class as class,"
		"team as team "
		" where "
		"teamwindata.playerindex=player.playerindex and "
		"teamwindata.playerclass=class.idx and "
		"teamwindata.playerteam=team.idx"
		";";
	SimpleExecute(SQL);

	SQL="create view VIEW_TEAMLOSSDATA as"
		" select "
		"teamlossdata.roundindex as roundindex,"
		"teamlossdata.teamlossidx as teamlossidx,"
		"teamlossdata.playerclass as classidx,"
		"teamlossdata.playerteam as teamidx,"
		"player.name as playername,"
		"player.id as playerid,"
		"player.clanindex as playerclanindex,"
		"player.playerindex as playerindex,"
		"class.realname as playerclassname,"
		"class.image as playerclassimage,"
		"team.realname as playerteamname,"
		"team.image as playerteamimage "
		" from "
		" teamlossdata,"
		"player,"
		"class as class,"
		"team as team "
		" where "
		"teamlossdata.playerindex=player.playerindex and "
		"teamlossdata.playerclass=class.idx and "
		"teamlossdata.playerteam=team.idx"
		";";
	SimpleExecute(SQL);


	CreateTotalListView("classes","class");
	CreateTotalListView("gametypes","gametype");
	CreateTotalListView("locations","location");
	CreateTotalListView("maps","map");
	CreateTotalListView("teams","team");
	CreateTotalListView("weapons","weapon");
}

void StatsgenDatabase::CreateTotalListView(const char *totalsuffix,const char *indextable)
{
	wxString	SQL;
	wxString	totalTable;

	totalTable=KillTotal::SQLTableName(totalsuffix);

	SQL.Printf("create view VIEW_%s as"
		" select "
		"%s.itemindex as itemindex,"
		"%s.playerindex as playerindex,"
		"%s.kills as kills,"
		"%s.deaths as deaths,"
		"%s.suicides as suicides,"
		"%s.teamkills as teamkills,"
		"player.name as name,"
		"%s.id as id,"
		"%s.realname as realname,"
		"%s.image as image "
		"from "
		"%s,%s,player "
		"where "
		"%s.itemindex=%s.idx and "
		"%s.playerindex=player.playerindex and "
		"player.dropped='N'"
		";",
		STRING_TO_CHAR(totalTable),
		STRING_TO_CHAR(totalTable),
		STRING_TO_CHAR(totalTable),
		STRING_TO_CHAR(totalTable),
		STRING_TO_CHAR(totalTable),
		STRING_TO_CHAR(totalTable),
		STRING_TO_CHAR(totalTable),
		indextable,
		indextable,
		indextable,
		STRING_TO_CHAR(totalTable),
		indextable,
		STRING_TO_CHAR(totalTable),
		indextable,
		STRING_TO_CHAR(totalTable));
		
	SimpleExecute(SQL);
}

void StatsgenDatabase::CreateKillTotalIndexes(const char *totaltype)
{
	wxString SQL;
	progress->SetStatus(totaltype);
	SQL.Printf("create index index_killtotal%s_1 "
			"on killtotal%s (itemindex,kills) "
			";",totaltype,totaltype);
	SimpleExecute(SQL);
}

void StatsgenDatabase::CreateIndexes()
{
	wxString	SQL;
	wxString	errorString;
	wxString	indexesEnabled;

	progress->SetStatus("Create Indexes");
	globalStatistics.configData.ReadTextValue(
									(char *)"/Database/CreateIndexes",
									&indexesEnabled,
									(char *)"y");

	if (indexesEnabled.CmpNoCase("y")==0)
	{
		progress->SetStatus((char *)"changedplayers");
		SQL="create index index_changedplayers_1 "
			"on changedplayers (idx) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"changedrounds");
		SQL="create index index_changedrounds_1 "
			"on changedrounds (idx) "
			";";
		SimpleExecute(SQL);

		CreateKillTotalIndexes((char *)"classes");
		CreateKillTotalIndexes((char *)"gametypes");
		CreateKillTotalIndexes((char *)"locations");
		CreateKillTotalIndexes((char *)"maps");
		CreateKillTotalIndexes((char *)"teams");
		CreateKillTotalIndexes((char *)"weapons");

		progress->SetStatus((char *)"xppoints_1");
		SQL="create index index_xppoints_1 "
			"on xppoints (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_2");
		SQL="create index index_xppoints_2 "
			"on xppoints (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_3");
		SQL="create index index_xppoints_3 "
			"on xppoints (pointsindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_4");
		SQL="create index index_xppoints_4 "
			"on xppoints (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_5");
		SQL="create index index_xppoints_5 "
			"on xppoints (playerindex,roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_6");
		SQL="create index index_xppoints_6 "
			"on xppoints (playerindex,pointsindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_7");
		SQL="create index index_xppoints_7 "
			"on xppoints (roundindex,pointsindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_8");
		SQL="create index index_xppoints_8 "
			"on xppoints (roundindex,playerindex,pointsindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xppoints_9");
		SQL="create index index_xppoints_9 "
			"on xppoints (playerindex,roundindex,pointsindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"xptotalpoints_1");
		SQL="create index index_xptotalpoints_1 "
			"on xptotalpoints (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xptotalpoints_2");
		SQL="create index index_xptotalpoints_2 "
			"on xptotalpoints (pointsindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xptotalpoints_3");
		SQL="create index index_xptotalpoints_3 "
			"on xptotalpoints (playerindex,pointsindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"xptotalpoints_4");
		SQL="create index index_xptotalpoints_4 "
			"on xptotalpoints (pointsindex,playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"roundvariables_1");
		SQL="create index index_roundvariables_1 "
			"on roundvariables (roundidx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"roundvariables_2");
		SQL="create index index_roundvariables_2 "
			"on roundvariables (serverindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"roundvariables_3");
		SQL="create index index_roundvariables_3 "
			"on roundvariables (serverindex,roundidx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"roundvariables_4");
		SQL="create index index_roundvariables_4 "
			"on roundvariables (roundidx,serverindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"action_1");
		SQL="create index index_action_1 "
			"on action (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"action_2");
		SQL="create index index_action_2 "
			"on action (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"class_1");
		SQL="create index index_class_1 "
			"on class (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"class_2");
		SQL="create index index_class_2 "
			"on class (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"gametype_1");
		SQL="create index index_gametype_1 "
			"on gametype (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"gametype_2");
		SQL="create index index_gametype_2 "
			"on gametype (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"location_1");
		SQL="create index index_location_1 "
			"on location (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"location_2");
		SQL="create index index_location_2 "
			"on location (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"map_1");
		SQL="create index index_map_1 "
			"on map (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"map_2");
		SQL="create index index_map_2 "
			"on map (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"team_1");
		SQL="create index index_team_1 "
			"on team (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"team_2");
		SQL="create index index_team_2 "
			"on team (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"weapon_1");
		SQL="create index index_weapon_1 "
			"on weapon (idx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"weapon_2");
		SQL="create index index_weapon_2 "
			"on weapon (id) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"actiondata_1");
		SQL="create index index_actiondata_1 "
			"on actiondata (roundindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"actiondata_2");
		SQL="create index index_actiondata_2 "
			"on actiondata (actionidx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"actiondata_3");
		SQL="create index index_actiondata_3 "
			"on actiondata (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"actiondata_4");
		SQL="create index index_actiondata_4 "
			"on actiondata (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"killdata_1");
		SQL="create index index_killdata_1 "
			"on killdata (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_2");
		SQL="create index index_killdata_2 "
			"on killdata (roundindex,killtype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_3");
		SQL="create index index_killdata_3 "
			"on killdata (playerindex,killtype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_4");
		SQL="create index index_killdata_4 "
			"on killdata (playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"killdata_5");
		SQL="create index index_killdata_5 "
			"on killdata (killtype,playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_6");
		SQL="create index index_killdata_6 "
			"on killdata (killtype,playerweapon) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_7");
		SQL="create index index_killdata_7 "
			"on killdata (playerweapon) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_8");
		SQL="create index index_killdata_8 "
			"on killdata (killtype,roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_9");
		SQL="create index index_killdata_9 "
			"on killdata (targetlocation) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_10");
		SQL="create index index_killdata_10 "
			"on killdata (killtype,targetindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"killdata_11");
		SQL="create index index_killdata_11 "
			"on killdata (targetindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"player_1");
		SQL="create index index_player_1 "
			"on player (playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"player_2");
		SQL="create index index_player_2 "
			"on player (dropped) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"playerinround_1");
		SQL="create index index_playerinround_1 "
			"on playerinround (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_2");
		SQL="create index index_playerinround_2 "
			"on playerinround (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_3");
		SQL="create index index_playerinround_3 "
			"on playerinround (roundindex,score) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_4");
		SQL="create index index_playerinround_4 "
			"on playerinround (roundindex,skill) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_5");
		SQL="create index index_playerinround_5 "
			"on playerinround (roundindex,kills) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_6");
		SQL="create index index_playerinround_6 "
			"on playerinround (roundindex,deaths) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_7");
		SQL="create index index_playerinround_7 "
			"on playerinround (roundindex,teamkills) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_8");
		SQL="create index index_playerinround_8 "
			"on playerinround (roundindex,suicides) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_9");
		SQL="create index index_playerinround_9 "
			"on playerinround (roundindex,duration) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"playerinround_10");
		SQL="create index index_playerinround_10 "
			"on playerinround (playerindex,roundindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"rounds_1");
		SQL="create index index_rounds_1 "
			"on rounds (roundidx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_2");
		SQL="create index index_rounds_2 "
			"on rounds (gametype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_3");
		SQL="create index index_rounds_3 "
			"on rounds (mapname) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_4");
		SQL="create index index_rounds_4 "
			"on rounds (dropped) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"rounds_5");
		SQL="create index index_rounds_5 "
			"on rounds (serverindex,roundidx) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_6");
		SQL="create index index_rounds_6 "
			"on rounds (serverindex,gametype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_7");
		SQL="create index index_rounds_7 "
			"on rounds (serverindex,mapname) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_8");
		SQL="create index index_rounds_8 "
			"on rounds (serverindex,dropped) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"rounds_9");
		SQL="create index index_rounds_9 "
			"on rounds (serverindex,mapname,gametype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_10");
		SQL="create index index_rounds_10 "
			"on rounds (serverindex,gametype,mapname) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"rounds_11");
		SQL="create index index_rounds_11 "
			"on rounds (mapname,gametype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"rounds_12");
		SQL="create index index_rounds_12 "
			"on rounds (gametype,mapname) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"speechdata_1");
		SQL="create index index_speechdata_1 "
			"on speechdata (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"speechdata_2");
		SQL="create index index_speechdata_2 "
			"on speechdata (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"speechdata_3");
		SQL="create index index_speechdata_3 "
			"on speechdata (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"teamlossdata_1");
		SQL="create index index_teamlossdata_1 "
			"on teamlossdata (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"teamlossdata_2");
		SQL="create index index_teamlossdata_2 "
			"on teamlossdata (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"teamlossdata_3");
		SQL="create index index_teamlossdata_3 "
			"on teamlossdata (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"teamwindata_1");
		SQL="create index index_teamwindata_1 "
			"on teamwindata (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"teamwindata_2");
		SQL="create index index_teamwindata_2 "
			"on teamwindata (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"teamwindata_3");
		SQL="create index index_teamwindata_3 "
			"on teamwindata (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"streakdata_1");
		SQL="create index index_streakdata_1 "
			"on streakdata (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_2");
		SQL="create index index_streakdata_2 "
			"on streakdata (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_3");
		SQL="create index index_streakdata_3 "
			"on streakdata (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_4");
		SQL="create index index_streakdata_4 "
			"on streakdata (streaktype) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_5");
		SQL="create index index_streakdata_5 "
			"on streakdata (playerindex,roundindex,streaksize) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_6");
		SQL="create index index_streakdata_6 "
			"on streakdata (roundindex,playerindex,streaksize) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_7");
		SQL="create index index_streakdata_7 "
			"on streakdata (roundindex,streaksize) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_8");
		SQL="create index index_streakdata_8 "
			"on streakdata (playerindex,streaksize) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_9");
		SQL="create index index_streakdata_9 "
			"on streakdata (streaktype,streaksize) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"streakdata_10");
		SQL="create index index_streakdata_10 "
			"on streakdata (playerindex,roundindex,streaksize) "
			";";
		SimpleExecute(SQL);

		progress->SetStatus((char *)"awardpoints_1");
		SQL="create index index_awardpoints_1 "
			"on awardpoints (roundindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_2");
		SQL="create index index_awardpoints_2 "
			"on awardpoints (roundindex,playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_3");
		SQL="create index index_awardpoints_3 "
			"on awardpoints (roundindex,score) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_4");
		SQL="create index index_awardpoints_4 "
			"on awardpoints (roundindex,awardindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_5");
		SQL="create index index_awardpoints_5 "
			"on awardpoints (playerindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_6");
		SQL="create index index_awardpoints_6 "
			"on awardpoints (playerindex,score) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_7");
		SQL="create index index_awardpoints_7 "
			"on awardpoints (playerindex,awardindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_8");
		SQL="create index index_awardpoints_8 "
			"on awardpoints (awardindex) "
			";";
		SimpleExecute(SQL);
		progress->SetStatus((char *)"awardpoints_9");
		SQL="create index index_awardpoints_9 "
			"on awardpoints (awardindex,score) "
			";";
		SimpleExecute(SQL);
	}

}

void StatsgenDatabase::GetDatabaseComponent(
									const char *componentType,
									wxArrayString &componentNames,
									wxArrayString &componentSQLList)
{
	wxString			SQL;
	TemplateOpenQuery	query;
	wxString			componentName;
	wxString			fieldName;
	wxString			componentSQL;

	STATSGEN_DEBUG_FUNCTION_START("StatsgenDatabase","GetDatabaseComponent")
	componentNames.Clear();
	componentSQLList.Clear();

	SQL.Printf("select name,sql from sqlite_master where type='%s' order by name",componentType);
	query.Initiate(SQL,sqliteDBHandle);
	while (query.NextRow())
	{
		fieldName="name";componentName=query.RetrieveProperty(fieldName);
		fieldName="sql";componentSQL=query.RetrieveProperty(fieldName);
		componentNames.Add(componentName);
		componentSQLList.Add(componentSQL);
		STATSGEN_DEBUG(DEBUG_ALWAYS,componentName)
		STATSGEN_DEBUG(DEBUG_ALWAYS,componentSQL)
	}
	
	STATSGEN_DEBUG_FUNCTION_END
}

void StatsgenDatabase::DropIndexes()
{
	wxString			SQL;
	TemplateOpenQuery	*query;
	wxString			indexName;
	wxString			fieldName;
	wxArrayString		indexNames;
	int					indexCount;
	int					indexIndex;
	wxString			msg;

	STATSGEN_DEBUG_FUNCTION_START("StatsgenDatabase","DropIndexes")

	query=new TemplateOpenQuery();
	fieldName="name";
	SQL="select name from sqlite_master where type='index' order by name";
	query->Initiate(SQL,sqliteDBHandle);
	while (query->NextRow())
	{
		indexName=query->RetrieveProperty(fieldName);
		indexNames.Add(indexName);
	}
	delete (query);
	indexCount=indexNames.GetCount();
	for (indexIndex=0;indexIndex<indexCount;indexIndex++)
	{
		indexName=indexNames.Item(indexIndex);
		msg.Printf("Dropping Index %s",STRING_TO_CHAR(indexName));
		progress->SetStatus(msg);
		SQL.Printf("drop index %s",STRING_TO_CHAR(indexName));
		SimpleExecute(SQL);
	}
	
	STATSGEN_DEBUG_FUNCTION_END
}

int StatsgenDatabase::RetrieveTableCount(const char *tableName,const char *fieldName)
{
	wxString		sql;
	TemplateOpenQuery	query;
	int			tableCount=-1;
	wxString		property="counter";

	sql.Printf("select max(%s) as counter from %s",fieldName,tableName);
	query.Initiate(sql,sqliteDBHandle);
	if (query.NextRow())
	{
		tableCount=atoi(STRING_TO_CHAR(query.RetrieveProperty(property)));
	}
	return tableCount;
}

void StatsgenDatabase::UpdateRoundCount()
{
	roundCount=RetrieveTableCount("rounds","roundidx");
}

void StatsgenDatabase::UpdatePlayerCount()
{
	playerCount=RetrieveTableCount("player","playerindex");
}

int StatsgenDatabase::NextPlayerIndex()
{
	return (playerCount+1);
}

int StatsgenDatabase::NextRoundIndex()
{
	return (roundCount+1);
}

void StatsgenDatabase::UpdatePlayerKillTotal(int playerIndex,
										int databasePlayerIndex,
										const char *tableNameSuffix,
										ArrayOfKillTotal &killTotals)
{
	static wxString		propertyItemIndex	="itemindex";
	static wxString		propertyKills		="kills";
	static wxString		propertyDeaths		="deaths";
	static wxString		propertySuicides	="suicides";
	static wxString		propertyTeamKills	="teamkills";
	TemplateOpenQuery	query;
	wxString			sql;

	sql.Printf("select * from killtotal%s where playerindex=\'%d\'",
				tableNameSuffix,
				databasePlayerIndex);
	query.Initiate(sql,sqliteDBHandle);
	while (query.NextRow())
	{
		KillTotal	killTotal;

		killTotal.playerIndex=playerIndex;
		killTotal.itemIndex	=atoi(STRING_TO_CHAR(query.RetrieveProperty(propertyItemIndex)));
		killTotal.kills		=atoi(STRING_TO_CHAR(query.RetrieveProperty(propertyKills)));
		killTotal.deaths	=atoi(STRING_TO_CHAR(query.RetrieveProperty(propertyDeaths)));
		killTotal.suicides	=atoi(STRING_TO_CHAR(query.RetrieveProperty(propertySuicides)));
		killTotal.teamkills	=atoi(STRING_TO_CHAR(query.RetrieveProperty(propertyTeamKills)));

		killTotals.Add(killTotal);
	}
}

void StatsgenDatabase::UpdatePlayerKillTotals(
											int playerIndex,
											int databasePlayerIndex,
											ArrayOfKillTotal &maps,
											ArrayOfKillTotal &gameTypes,
											ArrayOfKillTotal &weapons,
											ArrayOfKillTotal &locations,
											ArrayOfKillTotal &teams,
											ArrayOfKillTotal &classes)
{
	UpdatePlayerKillTotal(playerIndex,databasePlayerIndex,"maps",maps);
	UpdatePlayerKillTotal(playerIndex,databasePlayerIndex,"gametypes",gameTypes);
	UpdatePlayerKillTotal(playerIndex,databasePlayerIndex,"weapons",weapons);
	UpdatePlayerKillTotal(playerIndex,databasePlayerIndex,"locations",locations);
	UpdatePlayerKillTotal(playerIndex,databasePlayerIndex,"teams",teams);
	UpdatePlayerKillTotal(playerIndex,databasePlayerIndex,"classes",classes);
}

void StatsgenDatabase::Zap()
{
	wxArrayString	tableNames;
	wxArrayString	tableSQL;
	wxString		tableName;
	int				tableCount;
	int				tableIndex;
	wxString		sql;
	wxString		statusText;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("StatsgenDatabase","Zap")

	GetDatabaseComponent("table",tableNames,tableSQL);
	tableCount=tableNames.GetCount();
	msg.Printf("Emptying %d tables",tableCount);
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	progress->Initiate((long)tableCount,(char *)" tables",1, (char *)" tables",1);
	for (tableIndex=0;tableIndex<tableCount;tableIndex++)
	{
		progress->Update((long)tableIndex);
		tableName=tableNames.Item(tableIndex);
		msg.Printf("Emptying %d table %s",tableIndex,STRING_TO_CHAR(tableName));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		statusText="Emptying "+tableName;
		progress->SetStatus(statusText);
		sql="delete from "+tableName;
		SimpleExecute(sql);
	}
	progress->Finalise();
	STATSGEN_DEBUG_FUNCTION_END
}

void StatsgenDatabase::ApplyAliases(wxArrayInt	&changedPlayers)
{
	int				aliasCount;
	int				aliasIndex;
	AliasListEntry	aliasEntry;
	int				playerIndex;
	int				updatedIndex;

	
	changedPlayers.Clear();
	aliasCount=globalStatistics.aliasList.GetCount();
	progress->Initiate(aliasCount,(char *)" aliases", 1,(char *) " aliases",1);
	for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
	{
		progress->Update(aliasIndex);
		aliasEntry=globalStatistics.aliasList.Item(aliasIndex);
		updatedIndex=aliasEntry.UpdateInDatabase();
		if (updatedIndex!=-1)
		{
			changedPlayers.Add(updatedIndex);
		}
	}
}

void StatsgenDatabase::Optimize()
{
	wxString	SQL;

	SQL="analyze";
	SimpleExecute(SQL);
}

wxString StatsgenDatabase::StringFieldDefinition(const char *fieldname,const char *key,int defaultFieldWidth)
{
	wxString	configKey;
	wxString	configValue;
	wxString	defaultWidth;

	wxString	fieldDefinition;

	configKey="/Database/UseFixedFieldWidths";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"y");
	if (configValue.CmpNoCase("y")==0)
	{
		configKey="/DatabaseFieldWidths/";
		configKey+=key;
		defaultWidth.Printf("%d",defaultFieldWidth);
		globalStatistics.configData.ReadTextValue(configKey,&configValue,defaultWidth);
		// Using a fixed width field
		fieldDefinition.Printf("%s string (%d)",fieldname,atoi(STRING_TO_CHAR(configValue)));
	}
	else
	{
		// using variable width field
		fieldDefinition.Printf("%s string",fieldname);
	}

	return (fieldDefinition);
}

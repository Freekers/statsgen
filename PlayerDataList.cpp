// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>

// Statsgen includes
#include "PlayerDataList.h"
#include "PlayerDataEntry.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

PlayerDataList::PlayerDataList(const char *filenameConfigKeyIn,
				const char *databaseTableNameIn,
				const char *defaultFilenameIn)
{
	playerDataList.Clear();
	filenameConfigKey=filenameConfigKeyIn;
	databaseTableName=databaseTableNameIn;
	defaultFilename=defaultFilenameIn;
}

PlayerDataList::~PlayerDataList()
{
}

void PlayerDataList::ReadFromFile()
{
	wxString	filename;
	wxTextFile	fp;
	int			lineIndex;

	filename="";

	globalStatistics.configData.ReadTextValue(filenameConfigKey,
												&filename,
												defaultFilename);
	playerDataList.Clear();
	if (wxFileExists(filename))
	{
		if (fp.Open(filename))
		{
			for (lineIndex=0;lineIndex<fp.GetLineCount();lineIndex++)
			{
				PlayerDataEntry		playerDataEntry;
				playerDataEntry.ReadFromFile(&fp,lineIndex);
				playerDataList.Add(playerDataEntry);
			}
		}
	}
}

void PlayerDataList::WriteToFile()
{
	wxString	filename;
	FILE		*fp;
	int			playerIndex;
	int			playerCount;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("PlayerDataList","WriteToFile")
	filename="";

	globalStatistics.configData.ReadTextValue(filenameConfigKey,
												&filename,
												defaultFilename);
	fp=fopen(STRING_TO_CHAR(filename),"w");
	if (fp!=NULL)
	{
		PlayerDataEntry		playerDataEntry;
		
		playerCount=playerDataList.GetCount();
		STATSGEN_DEBUG_CODE(msg.Printf("Writing [%d] player data entries",playerCount);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		for (playerIndex=0;playerIndex<playerCount;playerIndex++)
		{
			playerDataEntry=playerDataList.Item(playerIndex);
			playerDataEntry.WriteToFile(fp);
		}
	}
	if (fp!=NULL)
	{
		fclose(fp);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void PlayerDataList::AddPlayer(wxString &guid,wxString &name,wxString &data)
{
	PlayerDataEntry		playerDataEntry;
	wxString			msg;
	int					oldIndex;

	STATSGEN_DEBUG_FUNCTION_START("PlayerDataList","AddPlayer")
	
	STATSGEN_DEBUG_CODE(msg.Printf("Adding [%s] [%s] [%s]", STRING_TO_CHAR(guid), STRING_TO_CHAR(name), STRING_TO_CHAR(data));)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	oldIndex=DeletePlayer(guid,name);
	playerDataEntry.playerGUID=guid;
	playerDataEntry.playerName=name;
	playerDataEntry.playerData=data;
	if (oldIndex==-1)
	{
		playerDataList.Add(playerDataEntry);
	}
	else
	{
		playerDataList.Insert(playerDataEntry,oldIndex);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

int PlayerDataList::DeletePlayer(wxString &guid,wxString &name)
{
	PlayerDataEntry		playerDataEntry;
	int					playerCount;
	int					playerIndex;
	wxString			msg;
	int					retVal=-1;

	STATSGEN_DEBUG_FUNCTION_START("PlayerDataList","DeletePlayer")
	STATSGEN_DEBUG_CODE(msg.Printf("Deleting [%s] [%s]", STRING_TO_CHAR(guid), STRING_TO_CHAR(name)); STATSGEN_DEBUG(DEBUG_ALWAYS,msg);)
	playerCount=playerDataList.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		playerDataEntry=playerDataList.Item(playerIndex);
		if ((playerDataEntry.playerGUID.Cmp(guid)==0)&&
			(playerDataEntry.playerName.Cmp(name)==0))
		{
			playerDataList.RemoveAt(playerIndex);
			retVal=playerIndex;
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Removing");
			break;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END

	return (retVal);
}

wxString PlayerDataList::SQLTableName()
{
	return (databaseTableName);
}

wxString PlayerDataList::SQLCreateTable()
{
	wxString	SQL;

	SQL.Printf("create table %s"
				"("
					"playerindex integer,"
					"%s"
				")",
				STRING_TO_CHAR(SQLTableName()),
				STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("playerdata","playerdata",FIELD_WIDTH_PLAYER_DATA)));
	return (SQL);
}

bool PlayerDataList::CreateDatabaseTable()
{
	wxString	SQL;
	bool		retVal=true;

	SQL=SQLCreateTable();

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	
	return (retVal);
}

bool PlayerDataList::WriteToDatabase()
{
	int					listCount;
	int					listIndex;
	PlayerDataEntry		listEntry;
	bool				retVal=true;

	listCount=playerDataList.GetCount();
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listEntry=playerDataList.Item(listIndex);
		listEntry.WriteToDatabase(STRING_TO_CHAR(SQLTableName()));
	}

	return (retVal);
}


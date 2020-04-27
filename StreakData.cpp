// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "StreakData.h"
#include "ErrorData.h"
#include "Player.h"
#include "GlobalStatistics.h"

StreakData::StreakData()
{
}

StreakData::~StreakData()
{
}

void StreakData::Debug()
{
	wxString	errorMessage;
	Player		player;

	errorMessage.Printf("Streak=%d ended by ",streakSize);
	if (streakEndedPlayerIndex!=-1)
	{
		player=globalStatistics.GetPlayer(streakEndedPlayerIndex);
		errorMessage+=player.name;
	}
	else
	{
		errorMessage+="nobody";
	}
}

wxString StreakData::SQLTableName()
{
	wxString	retVal="streakdata";

	return (retVal);
}

wxString StreakData::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"playerindex integer,"
				"streakidx integer,"
				"streaktype integer,"
				"streaksize integer,"
				"streakender integer"
			")",
			STRING_TO_CHAR(SQLTableName()));

	return SQL;
}


bool StreakData::WriteToDatabase(int roundIndex,int playerIndex,
									int streakIndex, KillTypes streakType)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;
	Player		streakEnderPlayer;
	int		actualStreakEnder;

	if (streakEndedPlayerIndex>=0)
	{
		streakEnderPlayer=globalStatistics.playerList.Item(streakEndedPlayerIndex);
		actualStreakEnder=streakEnderPlayer.actualPlayerIndex;
	}
	else
	{
		actualStreakEnder=streakEndedPlayerIndex;
	}


	SQL.Printf("insert into %s"
				"(roundindex,playerindex,streakidx,streaktype,"
				"streaksize,streakender)"
				"values"
				"('%d','%d','%d','%d','%d','%d')",
				STRING_TO_CHAR(SQLTableName()),
				roundIndex,playerIndex,streakIndex,streakType,
				streakSize,actualStreakEnder);
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	return retVal;
}


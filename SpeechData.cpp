// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "SpeechData.h"
#include "GlobalStatistics.h"

SpeechData::SpeechData()
{
}

SpeechData::~SpeechData()
{
}

wxString SpeechData::SQLTableName()
{
	wxString	retVal="speechdata";

	return (retVal);
}

wxString SpeechData::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"speechidx integer,"
				"playerindex integer,"
				"%s"
			")",
			STRING_TO_CHAR(SQLTableName()),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("speech","speech",FIELD_WIDTH_SPEECH)));

	return SQL;
}

bool SpeechData::WriteToDatabase(int roundIndex,int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	Player		player;

	player=globalStatistics.playerList.Item(playerIndex);

	SQL.Printf("insert into %s"
				"(roundindex,speechidx,playerindex,speech)"
				"values"
				"('%d','%d','%d','%s')",
				STRING_TO_CHAR(SQLTableName()),
				roundIndex,
				itemIndex,
				player.actualPlayerIndex,
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(speech)));
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}

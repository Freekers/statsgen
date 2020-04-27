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
			SQLTableName().GetData(),
		StatsgenDatabase::StringFieldDefinition("speech","speech",FIELD_WIDTH_SPEECH).GetData());

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
				SQLTableName().GetData(),
				roundIndex,
				itemIndex,
				player.actualPlayerIndex,
				StatsgenDatabase::SafeForInsert(speech).GetData());
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}

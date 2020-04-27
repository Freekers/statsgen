// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "AliasListEntry.h"
#include "GlobalStatistics.h"
#include "Progress.h"

AliasListEntry::AliasListEntry()
{
}

AliasListEntry::~AliasListEntry()
{
}

void AliasListEntry::ReadFromFile(wxTextFile *fp,int index)
{
	wxString	primaryLine;
	wxString	aliasLine;

	primaryLine=fp->GetLine(index*2);
	aliasLine=fp->GetLine((index*2)+1);

	primaryGUID=primaryLine.BeforeFirst(',');
	primaryName=primaryLine.AfterFirst(',');
	aliasGUID=aliasLine.BeforeFirst(',');
	aliasName=aliasLine.AfterFirst(',');
}

void AliasListEntry::WriteToFile(FILE *fp)
{
	fprintf(fp,"%s,%s\n%s,%s\n",
		primaryGUID.GetData(),
		primaryName.GetData(),
		aliasGUID.GetData(),
		aliasName.GetData());
}

int AliasListEntry::UpdateInDatabase()
{
	Player					primaryPlayer;
	Player					secondaryPlayer;
	int						primaryIndex=0;
	int						secondaryIndex=0;
	bool					primaryFound;
	bool					secondaryFound;
	int						updatedIndex=-1;
	static wxArrayString	tablesNeedingUpdate;
	static wxArrayString	fieldsNeedingUpdate;
	wxString				tableName;
	wxString				fieldName;
	static int				tableCount=0;
	int						tableIndex;
	wxString				sql;
	bool					cheat;

	if (tableCount==0)
	{
		tableName="actiondata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="akadata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="awardpoints";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="killdata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="killdata";
		fieldName="targetindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="playerinround";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="speechdata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="streakdata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="streakdata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="teamlossdata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="teamwindata";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableName="xppoints";
		fieldName="playerindex";
		tablesNeedingUpdate.Add(tableName);
		fieldsNeedingUpdate.Add(fieldName);

		tableCount=tablesNeedingUpdate.GetCount();
	}

	primaryPlayer.name=primaryName;
	secondaryPlayer.name=aliasName;

	if (primaryName.Cmp(aliasName)!=0)
	{
		primaryFound=primaryPlayer.UpdateFromDatabase(primaryIndex);
		secondaryFound=secondaryPlayer.UpdateFromDatabase(secondaryIndex);

		if (primaryFound && secondaryFound)
		{
			// Both players are in the database - so we need to join them together
			primaryPlayer.Add(secondaryPlayer);
			updatedIndex=primaryPlayer.actualPlayerIndex;
			for (tableIndex=0;tableIndex<tableCount;tableIndex++)
			{
				tableName=tablesNeedingUpdate.Item(tableIndex);
				fieldName=fieldsNeedingUpdate.Item(tableIndex);
				sql.Printf("update %s set %s=%d where %s=%d",
						tableName.GetData(),
						fieldName.GetData(),
						primaryPlayer.actualPlayerIndex,
						fieldName.GetData(),
						secondaryPlayer.actualPlayerIndex);
				globalStatistics.statsgenDatabase.SimpleExecute(sql);
			}
			// now we can delete the secondary players
			sql.Printf("delete from player where playerindex=%d",
				secondaryPlayer.actualPlayerIndex);
			globalStatistics.statsgenDatabase.SimpleExecute(sql);

			// now we need to write back the current changed player
			primaryPlayer.dropped=globalStatistics.dropList.IsDropped(primaryName,&cheat);
			primaryPlayer.WriteToDatabaseUpdate(primaryPlayer.actualPlayerIndex,true);
		}
		if (!primaryFound && secondaryFound)
		{
			// The secondary name is in the database - but no primary
			// so all we have to do is update the name
			sql.Printf("update player set name='%s' where playerindex='%d'",
					StatsgenDatabase::SafeForInsert(primaryName).GetData(),
					secondaryPlayer.actualPlayerIndex);
			globalStatistics.statsgenDatabase.SimpleExecute(sql);
		}
	}

	return (updatedIndex);
}

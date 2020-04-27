// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "COD5LogFileReader.h"
#include "GlobalStatistics.h"
#include "ErrorData.h"

void COD5LogFileReader::AddDefaultNonAction(const char *action)
{
	wxString	key;

	key=action;

	if (nonActions.Index(key)==wxNOT_FOUND)
	{
		nonActions.Add(key);
	}
}

COD5LogFileReader::COD5LogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn) : LogFileReader(filenameIn,serverTypeIn)
{
	STATSGEN_DEBUG_FUNCTION_START("COD5LogFileReader","Constructor");
	wxString		configGroup;

	configGroup="COD5NonActions";

	globalStatistics.configData.ReadList(configGroup,nonActions);

	AddDefaultNonAction("J");	// Join
	AddDefaultNonAction("Q");	// Quit
	AddDefaultNonAction("W");	// Team Win
	AddDefaultNonAction("L");	// Team Loss
	AddDefaultNonAction("T");	// Team Tie

	globalStatistics.configData.WriteList(configGroup,nonActions);
	STATSGEN_DEBUG_FUNCTION_END
}

bool COD5LogFileReader::DecodeAction(wxString &line,DecodedAction &decode)
{
	bool		retVal=false;
	int			tokenCount;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("COD5LogFileReader","DecodeAction")
	decode.action=GetLineToken(line,";",1,&tokenCount).Upper();
	if (tokenCount == 4)
	{
		decode.playerID=GetLineToken(line,";",2,&tokenCount);
		decode.playerRoundID=GetLineToken(line,";",3,&tokenCount);
		decode.playerName=GetLineToken(line,";",4,&tokenCount);
		decode.playerTeam="";
		UpdatePlayerRoundInfo2(decode.playerID,
								playerIDs,
								playerTeamList,
								decode.playerTeam);
		retVal=(nonActions.Index(decode.action)==wxNOT_FOUND);

		
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool COD5LogFileReader::DecodeTeamWin(wxString &line,DecodedTeamWin &decode)
{
	bool		retVal=false;
	int			tokenCount;
	int			tokenIndex=0;
	wxString	name;
	wxString	id;
	wxString	roundID;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodeTeamWin");
	roundID="";

	retVal=DecodeTeamWinLoss("W;",line,id,roundID,name,decode.teamName);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Normal Team Win Found")
		decode.Clear();
		decode.playerIDs.Add(id);
		decode.playerRoundIDs.Add(roundID);
		decode.playerNames.Add(name);
	}
	
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}


bool COD5LogFileReader::DecodeTeamWinLoss(const char *key,wxString &line,wxString &id, wxString &roundID,wxString &name, wxString &team)
{
	bool		retVal=false;
	int			tokenCount;

	STATSGEN_DEBUG_FUNCTION_START("COD5LogFileReader","DecodeTeamWinLoss");

	if (line.StartsWith(key))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Team Win or Loss Found")
		id=GetLineToken(line,";",2,&tokenCount);
		if (tokenCount == 4)
		{
			roundID=GetLineToken(line,";",3,&tokenCount);
			name=GetLineToken(line,";",4,&tokenCount);
			UpdatePlayerRoundInfo2(id,
								playerIDs,
								playerTeamList,
								team);
			retVal=true;
		
		}
	}
	
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool COD5LogFileReader::DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode)
{
	bool		retVal=false;
	int			tokenCount;
	int			tokenIndex=0;
	wxString	name;
	wxString	id;
	wxString	roundID;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodeTeamLoss");
	roundID="";

	retVal=DecodeTeamWinLoss("L;",line,id,roundID,name,decode.teamName);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Normal Team Loss Found")
		decode.Clear();
		decode.playerIDs.Add(id);
		decode.playerRoundIDs.Add(roundID);
		decode.playerNames.Add(name);
	}
	
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

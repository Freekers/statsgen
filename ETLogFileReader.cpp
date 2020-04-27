// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "GlobalStatistics.h"
#include "ETLogFileReader.h"
#include "LogFileReader.h"
#include "ErrorData.h"
#include "Round.h"
#include "ConfigData.h"
#include "StaticFunctions.h"
#include "Progress.h"
#include "Server.h"
#include "Round.h"


ETLogFileReader::ETLogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn): MOHAALogFileReader(filenameIn,serverTypeIn)
{
	STATSGEN_DEBUG_FUNCTION_START("ETLogFileReader","Constructor");
	STATSGEN_DEBUG_FUNCTION_END
}

bool ETLogFileReader::DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode)
{
	bool		retVal=false;

	STATSGEN_DEBUG_FUNCTION_START("ETLogFileReader","DecodeTeamLoss");
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool ETLogFileReader::DecodePlayerJoin(wxString &line,DecodedPlayerJoin &decode)
{
	bool		retVal=false;
	wxString 	playerInRoundID;

	STATSGEN_DEBUG_FUNCTION_START("ETLogFileReader","DecodePlayerJoin");
	retVal=DecodeClientUserinfoChanged(line,
									playerInRoundID,
									decode.playerName,
									decode.playerTeam,
									decode.playerClass);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,decode.playerName);
		STATSGEN_DEBUG(DEBUG_RARELY,decode.playerTeam);
		STATSGEN_DEBUG(DEBUG_RARELY,decode.playerClass);
		UpdatePlayerRoundInfo(playerInRoundID,
							playerIDListNames,
							playerIDList,
							decode.playerName);
		UpdatePlayerRoundInfo(decode.playerName,
							playerTeamListNames,
							playerTeamList,
							decode.playerTeam);
		UpdatePlayerRoundInfo(decode.playerName,
							playerClassListNames,
							playerClassList,
							decode.playerClass);
	}
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool ETLogFileReader::DecodeTeamWin(wxString &line,DecodedTeamWin &decode)
{
	bool		retVal=false;

	STATSGEN_DEBUG_FUNCTION_START("ETLogFileReader","DecodeTeamWin");
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool ETLogFileReader::IsWorldPlayer(wxString &id,wxString &team,wxString &name)
{
	bool	retVal=false;

	if ((name.Length()==0) ||
		(name.Cmp("<world>")==0))
	{
		retVal=true;
	}

	return (retVal);
}

bool ETLogFileReader::DecodeRoundStart(wxString &line,
					DecodedRoundStart &decode,
					Round *round)
{
	return (DecodeInitGame(line,decode,round));
}

bool ETLogFileReader::IsRoundStart(wxString &line)
{
	bool	retVal;

	retVal=(line.StartsWith("InitGame:"));

	return (retVal);
}

bool ETLogFileReader::IsRoundEnd(wxString &line)
{
	bool	retVal=false;

	retVal=false;
	if ((line.StartsWith("ShutdownGame:")))
	{
		retVal=true;
	}
	return (retVal);
}

bool ETLogFileReader::StripTimeFromLine(wxString &lineRead,wxDateTime &lineTime)
{
	bool		retVal=false;
	wxString	timeString;
	wxString	remainder;
	wxString	minuteString;
	wxString	secondString;
	wxString	hourString;
	int			hours=0;
	int			minutes=0;
	int			seconds=0;
	time_t		since1970;
	

	timeString=lineRead.BeforeFirst(' ');
	remainder=lineRead.AfterFirst(' ');
	lineRead=remainder;

	timeString.Trim(true);
	timeString.Trim(false);

	timeString.Replace(".",":");

	
	hourString=timeString.BeforeFirst(':');
	minuteString=timeString.AfterFirst(':');
	secondString=minuteString.AfterFirst(':');
	minuteString=minuteString.BeforeFirst(':');
	if (secondString.Length()>0)
	{
		// we are in format HH:MM:SS
	}
	else
	{
		// we are in format MM:SS
		secondString=minuteString;
		minuteString=hourString;
		hourString="0";
	}
	if (minuteString.IsNumber() &&
		secondString.IsNumber() &&
		hourString.IsNumber())
	{
		retVal=true;
		hours=atoi(hourString.GetData());
		minutes=atoi(minuteString.GetData());
		seconds=atoi(secondString.GetData());
		since1970=(3600*hours)+(60*minutes)+seconds;
		lineTime.Set(since1970);
	}
	else
	{
		retVal=false;
	}
	return(retVal);

}

ETLogFileReader::~ETLogFileReader()
{
}


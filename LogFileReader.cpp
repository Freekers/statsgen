// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "GlobalStatistics.h"
#include "LogFileReader.h"
#include "ErrorData.h"
#include "Round.h"
#include "ConfigData.h"
#include "StaticFunctions.h"
#include "Progress.h"
#include "Server.h"
#include "Round.h"
#include "MultiFile.h"

bool EndsWith(wxString &line,const char *match)
{
	int			matchLength;
	wxString	matchString;

	matchLength=strlen(match);
	matchString=line.Right(matchLength);

	return (matchString.Cmp(match)==0);
}

wxString GetLineToken(wxString &line,
						char *sep,
						int tokenNumber,
						int *tokenCount)
{
	wxStringTokenizer	tokens(line,sep);
	wxString		retVal;
	int			tokenIndex;

	*tokenCount=tokens.CountTokens();

	retVal="";
	if (tokenNumber<=*tokenCount)
	{
		for (tokenIndex=1;tokenIndex<=tokenNumber;tokenIndex++)
		{
			retVal=tokens.GetNextToken();
		}
	}

	return retVal;
}

LogFileReader::LogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn)
{
	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","Constructor");
	filenames			=filenameIn;
	serverType			=serverTypeIn;
	lastRoundEndedAt	=0;
	fakeTeamIDs			=true;

	STATSGEN_DEBUG_FUNCTION_END
}

void LogFileReader::FakeTeamIDs(bool value)
{
	fakeTeamIDs	= value;
}

wxString LogFileReader::GetServerType()
{
	return (serverType);
}

bool LogFileReader::DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode)
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

	if (line.StartsWith("L;"))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Team Loss Found")
		decode.Clear();
		decode.teamName=GetLineToken(line,(char *)";",2,&tokenCount);
		for (tokenIndex=4;tokenIndex<=tokenCount;tokenIndex+=2)
		{
			id=GetLineToken(line,(char *)";",tokenIndex-1,&tokenCount);
			name=GetLineToken(line,(char *)";",tokenIndex,&tokenCount);
			decode.playerIDs.Add(id);
			decode.playerRoundIDs.Add(roundID);
			decode.playerNames.Add(name);
		}
		retVal=true;
	}
	else
	{
		if (line.StartsWith("RL;"))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"COD4 PAM Team Loss Found")
			decode.Clear();
			decode.teamName=GetLineToken(line,(char *)";",1,&tokenCount);
			decode.teamName="";
			for(tokenIndex=3;tokenIndex<=tokenCount;tokenIndex+=2)
			{
				id=GetLineToken(line,(char *)";",tokenIndex-1,&tokenCount);
				name=GetLineToken(line,(char *)";",tokenIndex,&tokenCount);
				decode.playerIDs.Add(id);
				decode.playerRoundIDs.Add(roundID);
				decode.playerNames.Add(name);
				STATSGEN_DEBUG_CODE(
				msg.Printf("id[%s] name[%s]",STRING_TO_CHAR(id),STRING_TO_CHAR(name));
									)
				
				STATSGEN_DEBUG(DEBUG_RARELY,msg);
			}
			retVal=true;
		}
	}
	
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::DecodeTeamWin(wxString &line,DecodedTeamWin &decode)
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

	if (line.StartsWith("W;"))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Normal Team Win Found")
		decode.Clear();
		decode.teamName=GetLineToken(line,(char *)";",2,&tokenCount);
		for (tokenIndex=4;tokenIndex<=tokenCount;tokenIndex+=2)
		{
			id=GetLineToken(line,(char *)";",tokenIndex-1,&tokenCount);
			name=GetLineToken(line,(char *)";",tokenIndex,&tokenCount);
			decode.playerIDs.Add(id);
			decode.playerRoundIDs.Add(roundID);
			decode.playerNames.Add(name);
		}
		retVal=true;
	}
	else
	{
		if (line.StartsWith("RW;"))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"COD4 PAM Team Win Found")
			decode.Clear();
			decode.teamName=GetLineToken(line,(char *)";",1,&tokenCount);
			decode.teamName="";
			for(tokenIndex=3;tokenIndex<=tokenCount;tokenIndex+=2)
			{
				id=GetLineToken(line,(char *)";",tokenIndex-1,&tokenCount);
				name=GetLineToken(line,(char *)";",tokenIndex,&tokenCount);
				decode.playerIDs.Add(id);
				decode.playerRoundIDs.Add(roundID);
				decode.playerNames.Add(name);
				STATSGEN_DEBUG_CODE(
				msg.Printf("id[%s] name[%s]",STRING_TO_CHAR(id),STRING_TO_CHAR(name));
									)
				
				STATSGEN_DEBUG(DEBUG_RARELY,msg);
			}
			retVal=true;
		}
	}
	
	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::IsWorldPlayer(wxString &id,wxString &team,wxString &name)
{
	bool	retVal=false;

	if ((id.Length()==0) &&
		(name.Length()==0) &&
		(team.Cmp("world")==0))
	{
		retVal=true;
	}

	return (retVal);
}

bool LogFileReader::DecodeKill(wxString &line,DecodedKill &decode)
{
	bool	retVal=false;
	int		tokenCount;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodeKill");
	if ((line.StartsWith("D;")) || (line.StartsWith("AD;")) || (line.StartsWith("VD;")))
	{
		decode.targetID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.targetTeam=GetLineToken(line,(char *)";",4,&tokenCount);
		decode.playerID=GetLineToken(line,(char *)";",6,&tokenCount);
		decode.playerTeam=GetLineToken(line,(char *)";",8,&tokenCount);
		if (fakeTeamIDs)
		{
			UpdatePlayerRoundInfo2(decode.targetID,
								playerIDs,
								playerTeamList,
								decode.targetTeam);
			UpdatePlayerRoundInfo2(decode.playerID,
								playerIDs,
								playerTeamList,
								decode.playerTeam);
		}
	}
	if (line.StartsWith("K;"))
	{
		decode.targetID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.targetRoundID=GetLineToken(line,(char *)";",3,&tokenCount);
		decode.targetTeam=GetLineToken(line,(char *)";",4,&tokenCount);
		decode.targetName=GetLineToken(line,(char *)";",5,&tokenCount);
		decode.targetClass="Soldier";
		decode.playerID=GetLineToken(line,(char *)";",6,&tokenCount);
		decode.playerRoundID=GetLineToken(line,(char *)";",7,&tokenCount);
		decode.playerTeam=GetLineToken(line,(char *)";",8,&tokenCount);
		decode.playerName=GetLineToken(line,(char *)";",9,&tokenCount);
		decode.playerClass="Soldier";
		decode.playerWeapon=GetLineToken(line,(char *)";",10,&tokenCount);
		decode.targetDamage=GetLineToken(line,(char *)";",11,&tokenCount);
		decode.playerAmmo=GetLineToken(line,(char *)";",12,&tokenCount);
		decode.targetLocation=GetLineToken(line,(char *)";",13,&tokenCount);
		if ((decode.playerWeapon.Length()==0)||
			(decode.playerWeapon.Cmp("none")==0))
		{
			decode.playerWeapon=decode.playerAmmo;
		}
		if (tokenCount==13)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Kill Found")
			retVal=true;
			if (decode.targetTeam.Length()==0)
			{
				decode.targetTeam=FindPlayerRoundInfo2(
							decode.targetID,
							playerIDs,
							playerTeamList,
							(char *)"");
			}
			if (decode.playerTeam.Length()==0)
			{
				decode.playerTeam=FindPlayerRoundInfo2(
							decode.playerID,
							playerIDs,
							playerTeamList,
							(char *)"");
			}
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::DecodeAction(wxString &line,DecodedAction &decode)
{
	bool		retVal=false;
	int			tokenCount;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodeAction");
	if (line.StartsWith("A;"))
	{
		decode.playerID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.playerRoundID=GetLineToken(line,(char *)";",3,&tokenCount);
		decode.playerTeam=GetLineToken(line,(char *)";",4,&tokenCount);
		decode.playerName=GetLineToken(line,(char *)";",5,&tokenCount);
		decode.action=GetLineToken(line,(char *)";",6,&tokenCount);
		if (tokenCount==6)
		{
			// COD4 Kill Assist is differently organised *sigh*
			// Why can't they stick to the rules
			// Kill Assist
			// playerid;playername;targetid;targetname;action
			// playerid = playerid
			// playername = playerroundid
			// targetid = playerteam
			// targetname = playername
			// action = action
			// if targetid and player id are both hex then this really is
			// a cod4 kill assist
			if (IsHexString(decode.playerID) && IsHexString(decode.playerTeam))
			{
				decode.playerName		=decode.playerRoundID;
				decode.playerRoundID	="";
				decode.playerTeam		="";
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"COD4 Kill Assist Action Found")
			}
			else
			{
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Normal Action Found")
			}
			retVal=true;
		}
		if (tokenCount==4)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"COD4 PAM MOD activity")
			decode.action=decode.playerID;
			decode.playerID=decode.playerRoundID;
			decode.playerName=decode.playerTeam;

			decode.playerRoundID="";
			decode.playerTeam="";
			STATSGEN_DEBUG_CODE(
			msg.Printf("id[%s] name[%s] action[%s]",
						STRING_TO_CHAR(decode.playerID),
						STRING_TO_CHAR(decode.playerName),
						STRING_TO_CHAR(decode.action));
								)
			retVal=true;
			STATSGEN_DEBUG(DEBUG_RARELY,msg)
		}
		if (tokenCount==5)
		{
			if ((decode.playerTeam.IsNumber()) &&
				(decode.playerName.StartsWith("shots_")))
			{
				// COD4 PAM MOD
				// 4th token a numeric, 5th token shots_fired or shots_hit
				// not really a action
				retVal=false;
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"COD4 PAM MOD Action Found")
			}
			else
			{
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Extreme MOD Action Found")
				// This is a (extreme) mod with no team line
				decode.action=decode.playerName;
				decode.playerName=decode.playerTeam;
				decode.playerTeam="";
				retVal=true;
			}
		}
	}
	if (line.StartsWith("heal;"))
	{
		decode.playerID="";
		decode.playerRoundID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.playerTeam="";
		decode.playerName=GetLineToken(line,(char *)";",3,&tokenCount);
		decode.action=GetLineToken(line,(char *)";",1,&tokenCount);
		if (tokenCount==5)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Admiral Action Found")
			retVal=true;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::DecodeSpeech(wxString &line,DecodedSpeech &decode)
{
	bool				retVal=false;
	int					tokenCount;
	Player				player;
	PlayerInRound		playerInRound;
	int					playerInRoundCount;
	int					playerInRoundIndex;
	wxString			decolouredName;
	bool				found;
	wxString			tempGUID;
	
	wxString			msg;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodeSpeech");

	if ((line.StartsWith("say;"))||
		(line.StartsWith("sayteam;")))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Speech Found")
		decode.playerID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.playerRoundID=GetLineToken(line,(char *)";",3,&tokenCount);
		decode.playerName=GetLineToken(line,(char *)";",4,&tokenCount);
		decode.speech=GetLineToken(line,(char *)";",5,&tokenCount);

		tempGUID=serverType+"_"+decode.playerID;
		
		// Speech does not bother with the colour codes
		// Search current round for a decoloured name that matches this one
		found = false;
		playerInRoundCount=currentRound.playersInRound.GetCount();
		for (playerInRoundIndex=0;
			playerInRoundIndex<playerInRoundCount;
			playerInRoundIndex++)
		{
			playerInRound=currentRound.playersInRound.Item(playerInRoundIndex);
			player=globalStatistics.playerList.Item(playerInRound.playerIndex);
			decolouredName=DecolouriseName(player.name);
			if (decolouredName.Cmp(decode.playerName)==0)
			{
				// Matched it - swap
				decode.playerName=player.name;
				found=true;
				break;
			}
		}
		if (!found)
		{
			// not found so far - lets try using GUID
			for (playerInRoundIndex=0;
				playerInRoundIndex<playerInRoundCount;
				playerInRoundIndex++)
			{
				playerInRound=currentRound.playersInRound.Item(playerInRoundIndex);
				player=globalStatistics.playerList.Item(playerInRound.playerIndex);
				if (player.ID.Cmp(tempGUID)==0)
				{
					// Matched it - swap
					decode.playerName=player.name;
					found=true;
					break;
				}
			}
		}
		retVal=found;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::DecodePlayerJoin(wxString &line,DecodedPlayerJoin &decode)
{
	bool	retVal=false;
	int		tokenCount;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodePlayerJoin");
	if (line.StartsWith("J;"))
	{
		decode.playerID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.playerRoundID=GetLineToken(line,(char *)";",3,&tokenCount);
		decode.playerName=GetLineToken(line,(char *)";",4,&tokenCount);
		decode.playerClass="";
		decode.playerTeam="";
		if (tokenCount==4)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Join Found")
			retVal=true;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::DecodePlayerQuit(wxString &line,DecodedPlayerQuit &decode)
{
	bool	retVal=false;
	int		tokenCount;
	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodePlayerQuit");

	if (line.StartsWith("Q;"))
	{
		decode.playerID=GetLineToken(line,(char *)";",2,&tokenCount);
		decode.playerRoundID=GetLineToken(line,(char *)";",3,&tokenCount);
		decode.playerName=GetLineToken(line,(char *)";",4,&tokenCount);
		if (tokenCount==4)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Quit Found")
			retVal=true;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::DecodeRoundStart(wxString &line,
									DecodedRoundStart &decode,
									Round *round)
{
	bool	retVal;
	retVal=DecodeInitGame(line,decode,round);
	if (retVal)
	{
		playerIDs.Clear();
		playerTeamList.Clear();
	}

	return (retVal);
}

bool LogFileReader::DecodeInitGame(wxString &line,
									DecodedRoundStart &decode,
									Round *round)
{
	bool		retVal=false;
	int			tokenCount;
	int			tokenIndex;
	wxString	remainder;
	wxString	key;
	wxString	value;
	bool		mapFound=false;
	bool		gameTypeFound=false;
	wxString	msg;
	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","DecodeRoundStart");

	if (IsRoundStart(line))
	{
		round->logfileVariableKeys.Clear();
		round->logfileVariableValues.Clear();
		remainder=line.AfterFirst(' ');
		key=GetLineToken(remainder,(char *)"\\",1,&tokenCount);
		for (tokenIndex=1;tokenIndex<=tokenCount;tokenIndex++)
		{
			if ((tokenIndex % 2)!=0)
			{
				// odd numbers are the values
				value=GetLineToken(remainder,(char *)"\\",tokenIndex,&tokenCount);
				// We have decoded a value - the key should represent what
				// it means
				if (key.CmpNoCase("g_gametype")==0)
				{
					decode.gameType=value;
					gameTypeFound=true;
				}
				else
				if (key.CmpNoCase("mapname")==0)
				{
					decode.mapName=value;
					mapFound=true;
				}
				else
				{
					if (tokenIndex>1)
					{
						thisServer->AddVariable(key,value);
						round->AddVariable(key,value);
					}
				}

			}
			else
			{
				// even numbers are the keys
				key=GetLineToken(remainder,(char *)"\\",tokenIndex,&tokenCount);
			}
		}
		if (gameTypeFound && mapFound)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Round Start Found")
			retVal=true;
		}
	}
	else
	{
		// Not a Start Round
		retVal=false;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool LogFileReader::IsRoundStart(wxString &line)
{
	bool	retVal;

	retVal=false;
	if (line.StartsWith("InitGame:"))
	{
		retVal=true;
	}

	return (retVal);
}

bool LogFileReader::IsRoundEnd(wxString &line)
{
	bool	retVal;

	retVal=false;
	if ((line.StartsWith("ShutdownGame:"))||
		(line.StartsWith("ExitLevel:"))||
		(line.StartsWith("RestartGame:"))
		)

	{
		retVal=true;
	}

	return (retVal);
}

bool LogFileReader::StripTimeFromLine(wxString &lineRead,wxDateTime &lineTime)
{
	bool		retVal=false;
	wxString	timeString;
	wxString	remainder;
	wxString	minuteString;
	wxString	secondString;
	int			minutes=0;
	int			seconds=0;
	time_t		since1970;
	

	timeString=lineRead.BeforeFirst(' ');
	remainder=lineRead.AfterFirst(' ');
	lineRead=remainder;

	timeString.Trim(true);
	timeString.Trim(false);

	if (timeString.IsNumber())
	{
		since1970=atoi(STRING_TO_CHAR(timeString));
		lineTime.Set(since1970);
		retVal=true;
	}
	else
	{
		minuteString=timeString.BeforeFirst(':');
		secondString=timeString.AfterFirst(':');
		if (minuteString.IsNumber() &&
			secondString.IsNumber())
		{
			retVal=true;
			minutes=atoi(STRING_TO_CHAR(minuteString));
			seconds=atoi(STRING_TO_CHAR(secondString));
			since1970=(60*minutes)+seconds;
			lineTime.Set(since1970);
		}
		else
		{
			retVal=false;
		}
	}
	return(retVal);
}

wxString LogFileReader::ReadLine(MultiFile &logfile,
				ErrorData &errorData)
{
	unsigned char		charRead;
	bool		endOfLine=false;
	wxString	lineRead;

	lineRead="";
	while (!endOfLine && !logfile.Eof())
	{
		charRead=logfile.GetC();
		if (charRead=='\n')
		{
			endOfLine=true;
		}
		else
		if (charRead=='\r')
		{
			endOfLine=true;
		}
		else
		if (charRead>=' ')
		{
			// Printable character
			lineRead+=charRead;
		}
		else
		{
			// Corrupted character - don't add to line
		}
	}

	return(lineRead);
}

void LogFileReader::ProcessLogFile(Server *server,int serverIndex,long maxLogfileSize)
{
	MultiFile			logfile(filenames);
	wxString			errorMessage;
	wxFileOffset		logfileSize=0;
	bool				inRound=false;
	wxString			lineRead;
	bool				result;
	wxDateTime			lineTime;
	int					roundCount;
	int					roundIndex;
	off_t				currentFilePosition;
	wxString			msg;
	ErrorData			errorData;
	int					lineNumber=0;
	long				maxLogfilePosition=0;
	long				lastRoundEndedAtLong;

	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","ProcessLogFile")
	thisServer=server;
	if (logfile.Ok())
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 1");
		// File appears to be open

		logfileSize=logfile.Length();
		if (logfileSize<lastRoundEndedAt)
		{
			lastRoundEndedAt=0;
			secondaryLastRoundEndedAt=0;
		}
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 2");
		/*
		STATSGEN_DEBUG_CODE(errorMessage.Printf("File [%s] opened OK %d bytes",
					 filename.GetData(),
					(int)logfileSize);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
		*/
		inRound=false;

		// Seek to the last bit of the logfile read
		logfile.SeekI(lastRoundEndedAt);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 3");
		if (maxLogfileSize>0)
		{
			maxLogfilePosition=lastRoundEndedAt+(1024*1024*maxLogfileSize);
		}
		else
		{
			maxLogfilePosition=0;
		}
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 4");
		lastRoundEndedAtLong = lastRoundEndedAt;
		STATSGEN_DEBUG_CODE(msg.Printf("Last Round Ended At %ld",lastRoundEndedAtLong);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 5");
		while (!logfile.Eof() && errorData.IsOK())
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 6.1");
			// Read each line in turn
			// Stripping leading and trailing spaces
			currentFilePosition=logfile.TellI();
			progress->Update(currentFilePosition);
			lineRead=ReadLine(logfile,errorData);
			lineRead.Trim(true);
			lineRead.Trim(false);
			lineNumber++;
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 6.2");
			STATSGEN_DEBUG_CODE(msg.Printf("%9d %s",lineNumber,STRING_TO_CHAR(lineRead));)
			STATSGEN_DEBUG(DEBUG_RARELY,msg)
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 6.3");
			if (lineRead.Length()>0)
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 6.4");
				// Line contains something
				result=StripTimeFromLine(lineRead,lineTime);
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 6.5");
				if (result)
				{
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 6.6");
					// Line has a valid time field
					if (IsRoundStart(lineRead))
					{
						STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Round Start Encountered")
						// Start of round encountered
						if (inRound)
						{
							STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Old Round Ended.Starting new round")
							// We were already in a round
							// Strange situation - new round started
							// without old round ending
							// Just store current round and start again
							currentRound.Process(errorData,serverIndex);
							//currentRound.Debug();
							globalStatistics.rounds.Add(currentRound);
							currentRound.Initiate(this,
											lineTime,
											lineRead);
							lastRoundEndedAt=currentFilePosition;
						}
						else
						{
							STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Starting new round")
							// We were not in a previous round
							// so we just need to create a new round
							currentRound.Initiate(this,
											lineTime,
											lineRead);
						}
						inRound=true;
					}
					else
					if (IsRoundEnd(lineRead))
					{
						
						STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Round End encountered")
						// Reached end of a round
						if (inRound)
						{
							// Reached end of an active round
							currentRound.AddLogLine(lineTime,lineRead);
							currentRound.Process(errorData,serverIndex);
							//currentRound.Debug();
							globalStatistics.rounds.Add(currentRound);
							roundCount=globalStatistics.rounds.GetCount();
							msg.Printf("Processing: Rounds %d",roundCount);
							progress->SetStatus(msg);
							lastRoundEndedAt=logfile.TellI();
						}
						else
						{
							STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Not In Round:Line Discarded")
							// End of an inactive round (start of logfile)
							// or Start Of Round got corrupted/not recorded.
							// Just throw away this round
						}
						inRound=false;
					}
					else
					{
						STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"not round start or end");
						// Some other sort of line - just add it
						currentRound.AddLogLine(lineTime,lineRead);
					}
				}
				else
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Bad Time Field")
					// Line does not have a valid time field
					// Ignore
				}
			}
			else
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"zero length line");
				// Line is zero length - ignore
			}
			if ((maxLogfilePosition>0) && (lastRoundEndedAt>maxLogfilePosition))
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"reached limit for this process run");
				// Reached the limit for this process run
				break;
			}
		}
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 7");
		// End of file processed
		// Search through removing all the dropped rounds
		roundCount=globalStatistics.rounds.GetCount();
		roundIndex=0;
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Dropping Inactive Rounds")
		while (roundIndex<roundCount)
		{
			STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Dropping round 1")
			currentRound=globalStatistics.rounds.Item(roundIndex);
			if (currentRound.IsDropped())
			{
				STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Dropping round 2")
				globalStatistics.rounds.RemoveAt(roundIndex);
				roundCount--;
			}
			else
			{
				roundIndex++;
			}
		}
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 8");
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Step 9");
		// Failed to open the logfile
		errorMessage="Failed to open " /*+ filename + " for reading."*/;
		errorMessage+=logfile.Filename();
		errorMessage+=" for reading.";
		errorData.SetError(ERROR_CODE_FILE_OPEN, errorMessage);
		progress->LogError(errorMessage,SeverityError);
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	}
	STATSGEN_DEBUG_FUNCTION_END
}

LogFileReader::~LogFileReader()
{
}

void LogFileReader::StartFromBeginning()
{
	STATSGEN_DEBUG_FUNCTION_START("LogFileReader","StartFromBeginning")
	lastRoundEndedAt=0;
	STATSGEN_DEBUG_FUNCTION_END
}

void LogFileReader::RetrieveLastRoundPositions(long *lastRoundEndedAtOut,
										long *secondaryLastRoundEndedAtOut)
{
	*lastRoundEndedAtOut=lastRoundEndedAt;
	*secondaryLastRoundEndedAtOut=lastRoundEndedAt;
}
void LogFileReader::SetLastRoundPositions(long lastRoundEndedAtIn,
										long secondaryLastRoundEndedAtIn)
{
	
	lastRoundEndedAt=lastRoundEndedAtIn;
	secondaryLastRoundEndedAt=lastRoundEndedAtIn;
}

wxString LogFileReader::FindPlayerRoundInfo2(wxString &name,
												wxArrayString &listnames,
												wxArrayString &listIDs,
												char *defaultValue)
{
	wxString	retVal=defaultValue;
	int			listCount;
	int			listIndex;
	wxString	listname;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","FindPlayerRoundInfo")
	listCount=listnames.GetCount();

	STATSGEN_DEBUG(DEBUG_RARELY,name)
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listname=listnames.Item(listIndex);
		if (listname.Cmp(name)==0)
		{
			retVal=listIDs.Item(listIndex);
			STATSGEN_DEBUG(DEBUG_RARELY,retVal)
			break;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void LogFileReader::UpdatePlayerRoundInfo2(wxString &name,
								wxArrayString &listnames,
								wxArrayString &listIDs,
								wxString &id)
{
	int			listCount;
	int			listIndex;
	wxString	listname;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("MOHAALogFileReader","UpdatePlayerRoundInfo")
	if (id.Length()==0)
	{
		return;
	}
	listCount=listnames.GetCount();
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listname=listnames.Item(listIndex);
		if (listname.Cmp(name)==0)
		{
			listnames.RemoveAt(listIndex);
			listIDs.RemoveAt(listIndex);
			break;
		}
	}
	STATSGEN_DEBUG_CODE(msg.Printf("[%s] = [%s]",STRING_TO_CHAR(name),STRING_TO_CHAR(id));)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	listnames.Add(name);
	listIDs.Add(id);

	STATSGEN_DEBUG_FUNCTION_END
}

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "GlobalStatistics.h"
#include "QuakeWarsLogFileReader.h"
#include "ErrorData.h"
#include "Round.h"
#include "ConfigData.h"
#include "StaticFunctions.h"
#include "Progress.h"
#include "Server.h"
#include "Round.h"
#include "MultiFile.h"


QuakeWarsLogFileReader::QuakeWarsLogFileReader(wxArrayString &filenameIn,wxArrayString &secondaryFilenameIn,wxString &serverTypeIn): LogFileReader(filenameIn,serverTypeIn)
{
	STATSGEN_DEBUG_FUNCTION_START("QuakeWarsLogFileReader","Constructor");
	secondaryFilenames			=secondaryFilenameIn;
	secondaryLastRoundEndedAt	=0;

	STATSGEN_DEBUG_FUNCTION_END
}

time_t QuakeWarsLogFileReader::StripTimeFromLine(wxString &lineRead)
{
	time_t		lineTime;
	wxString	lineTimeString;

	lineTimeString=lineRead.Left(19);
	lineTime=DecodeTime(lineTimeString);
	if (lineTime>0)
	{
		lineRead=lineRead.Mid(22);
	}

	return (lineTime);
}

time_t QuakeWarsLogFileReader::DecodeTime(wxString &timeString)
{
	wxDateTime	decodedTime;
	bool		parseResult;
	time_t		retVal;
	//char		*dateFormat="%Y-%m-%d %H:%M:%S";
	wxString	dateFormat="%Y-%m-%d %H:%M:%S";
	//char		*timeStringChar;
	wxString	timeStringChar;
	wxString::const_iterator	end;

	//timeStringChar=(char *)timeString.GetData();
	timeStringChar=timeString;
	if (timeString.Length()!=19)
	{
		retVal=0;
	}
	else
	{
		//parseResult=(wxChar *)decodedTime.ParseFormat(timeStringChar,dateFormat);
		parseResult=decodedTime.ParseFormat(timeString,dateFormat,&end);
		//if (parseResult=NULL)
		if (parseResult == FALSE)
		{
			retVal=0;
		}
		else
		{
			retVal=decodedTime.GetTicks();
		}
	}

	return (retVal);
}

time_t QuakeWarsLogFileReader::DecodeMapStartTime(wxString &lineRead,
								const char *prefix)
{
	wxString	timeLine;
	time_t		timeExtracted;

	timeLine=lineRead.Mid(strlen(prefix));
	timeLine=timeLine.Left(19);
	timeExtracted=DecodeTime(timeLine);

	return (timeExtracted);
}

void QuakeWarsLogFileReader::TrimQuotes(wxString &line)
{
	int	lineLength;

	if (line[0]=='\'')
	{
		line=line.Mid(1);
	}
	lineLength=line.Length();
	if (lineLength>0)
	{
		if (line[lineLength-1]=='\'')
		{
			line=line.Left(lineLength-1);
		}
	}
}

void QuakeWarsLogFileReader::SortPlayersByLength(
		wxArrayString &playerNames,
		wxArrayString &playerTeams,
		wxArrayString &playerClasses)
{
	wxString		playerName;
	wxString		playerTeam;
	wxString		playerClass;
	wxArrayString	outplayerNames;
	wxArrayString	outplayerTeams;
	wxArrayString	outplayerClasses;
	int				nameCount;
	int				nameIndex;
	int				longestNameIndex;
	int				longestNameLength;
	int				nameLength;

	// Need the longest filename first
	nameCount=playerNames.GetCount();
	while (nameCount>0)
	{
		longestNameLength=0;
		for (nameIndex=0;nameIndex<nameCount;nameIndex++)
		{
			playerName=playerNames.Item(nameIndex);
			nameLength=playerName.Length();
			if (nameLength>=longestNameLength)
			{
				longestNameLength=nameLength;
				longestNameIndex=nameIndex;
			}
		}
		playerName=playerNames.Item(longestNameIndex);
		playerClass=playerClasses.Item(longestNameIndex);
		playerTeam=playerTeams.Item(longestNameIndex);

		playerNames.RemoveAt(longestNameIndex);
		playerClasses.RemoveAt(longestNameIndex);
		playerTeams.RemoveAt(longestNameIndex);

		outplayerNames.Add(playerName);
		outplayerClasses.Add(playerClass);
		outplayerTeams.Add(playerTeam);
		nameCount=playerNames.GetCount();
	}

	playerNames=outplayerNames;
	playerClasses=outplayerClasses;
	playerTeams=outplayerTeams;
}

wxString QuakeWarsLogFileReader::SubstitutePlayerNamesInLine(
			wxString &lineRead,wxArrayString &playerNames)
{
	int			playerCount;
	int			playerIndex;
	wxString	playerName;
	wxString	updatedLine;

	updatedLine=lineRead;
	playerCount=playerNames.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		playerName=playerNames.Item(playerIndex);
		updatedLine.Replace(playerName,QUAKEWARS_LOGLINE_PLAYER_SUBSTITUTE);
	}

	return (updatedLine);
}

wxString QuakeWarsLogFileReader::PlayerGUID(wxString &playerName,
											wxArrayString &pbGUIDs,
											wxArrayString &pbNames)
{
	wxString	guid="0";
	int			playerIndex;

	playerIndex=pbNames.Index(playerName);
	if (playerIndex!=wxNOT_FOUND)
	{
		guid=pbGUIDs.Item(playerIndex);
	}

	return (guid);
}
bool QuakeWarsLogFileReader::DecodeRound(
			MultiFile &logfileBuffer,
			MultiFile &secondaryLogfileBuffer,
			int serverIndex,
			wxArrayString &pbGUIDs,
			wxArrayString &pbNames)
{
	// Secondary logfile contains the round details
	// primary logfile contains kills

	// We need to find the round details in the secondary logfile
	// then find any associated kills in the secondary logfile

	wxArrayString	playerNames;
	wxArrayString	playerTeams;
	wxArrayString	playerClasses;
	wxString		mapName;
	wxString		gameType;
	wxString		roundLength;
	wxString		winningTeam;
	time_t			mapStartTime=0;
	time_t			mapEndTime=0;
	time_t			lineTime=0;
	wxString		serverName;
	bool			roundFinished=false;
	bool			roundNearlyFinished=false;
	bool			roundStarted=false;
	bool			inRound=false;
	bool			useThisLine=false;
	wxString		lineRead;
	wxString		decodedLine;
	ErrorData		errorData;
	wxString		currentTeam="unknown";
	off_t			secondaryFilePosition;
	wxString		playerLine;
	wxString		playerName;
	wxString		playerTeam;
	wxString		playerClass;
	wxString		targetName;
	wxString		targetTeam;
	wxString		targetClass;
	wxString		weapon;
	wxString		weaponString;
	int				weaponIndex;
	int				playerIndex;
	int				targetIndex;
	bool			noMoreRounds;
	int				playerCount;
	wxString		playerGUID;
	wxString		playerAmmo;
	wxString		targetGUID;
	wxString		targetLocation;
	wxString		lastPlayerName;
	long			currentPosition;
	wxArrayString	xpLines;
	int				xpIndex;
	int				xpLineIndex;
	int				xpLineCount;
	wxString		xpLine;
	static wxArrayString	serverVariables;
	static wxArrayString	serverValues;
	int				variableIndex;
	wxString		variable;
	wxString		value;
		

	STATSGEN_DEBUG_FUNCTION_START("QuakeWarsLogFileReader","DecodeRound")
	// Apart from xp points (which i have no, current, interest in)
	// All the obective file usefully logs is
	// Round Start Time
	// Round End Time
	// Round Map
	// Round Gametype (ruleset)
	// What players are present and in what teams

	noMoreRounds=false;
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Decoding Objective File")
	secondaryFilePosition=secondaryLogfileBuffer.TellI();
	while (!secondaryLogfileBuffer.Eof() && (!roundFinished))
	{
		currentPosition=secondaryLogfileBuffer.TellI();
		currentPosition+=logfileBuffer.TellI();
		progress->Update(currentPosition);
		lineRead=ReadLine(secondaryLogfileBuffer,errorData);
		STATSGEN_DEBUG(DEBUG_RARELY,lineRead)
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_PLAYER_XP))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"XP Found")
			xpLine=lineRead+" ";
			xpLine+=lastPlayerName;
			xpIndex=xpLines.Index(xpLine);
			if (xpIndex!=wxNOT_FOUND)
			{
				xpLines.RemoveAt(xpIndex);
			}
			xpLines.Add(xpLine);
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_MAPSTARTED))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Round Start Found")
			xpLines.Clear();
			roundStarted=true;
			mapStartTime=DecodeMapStartTime(lineRead,
								QUAKEWARS_LOGLINEPREFIX_MAPSTARTED);
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_SERVER))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Server Found")
			serverName=lineRead.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_SERVER));
			STATSGEN_DEBUG(DEBUG_RARELY,serverName)
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_MAP))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Map Found")
			mapName=lineRead.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_MAP));
			TrimQuotes(mapName);
			mapName=mapName.AfterLast('/');
			mapName=mapName.BeforeFirst('.');
			mapName=mapName.Lower();
			STATSGEN_DEBUG(DEBUG_RARELY,mapName)
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_RULESET))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"gametype Found")
			gameType=lineRead.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_RULESET));
			gameType=gameType.Lower();
			STATSGEN_DEBUG(DEBUG_RARELY,gameType)
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_TEAM))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"team Found")
			currentTeam=lineRead.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_TEAM));
			currentTeam=currentTeam.Lower();
			STATSGEN_DEBUG(DEBUG_RARELY,currentTeam)
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_PLAYER))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"player Found")
			playerLine=lineRead.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_PLAYER));
			// Player Line contains
			// 'playername' Class 'class'
			TrimQuotes(playerLine);
			// Player Line contains
			// playername' Class 'class
			playerClass=playerLine.AfterLast('\'');
			playerLine=playerLine.BeforeLast(' ');
			// Player Line contains
			// playername' Class
			playerLine=playerLine.BeforeLast(' ');
			// Player Line contains
			// playername'
			playerName=playerLine.BeforeLast('\'');
			// At this point we have 
			// player Name
			// Player Class
			// and hopefully the current player team
			playerIndex=playerNames.Index(playerName);
			if (playerIndex!=wxNOT_FOUND)
			{
				// Existing player - update its values
				// just remove it and we will add it at end
				playerNames.RemoveAt(playerIndex);
				playerTeams.RemoveAt(playerIndex);
				playerClasses.RemoveAt(playerIndex);
			}
			playerNames.Add(playerName);
			playerClasses.Add(playerClass);
			playerTeams.Add(currentTeam);
			lastPlayerName=playerName;
			wxString msg;
			msg.Printf("name [%s] class=[%s] team=[%s]",
				STRING_TO_CHAR(playerName),
				STRING_TO_CHAR(playerClass),
				STRING_TO_CHAR(currentTeam));
			STATSGEN_DEBUG(DEBUG_RARELY,msg)
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_MAPFINISHED))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"map finished Found")
			mapEndTime=DecodeMapStartTime(lineRead,
								QUAKEWARS_LOGLINEPREFIX_MAPFINISHED);
		} else
		if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_WINNINGTEAM))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"team win Found")
			winningTeam=lineRead.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_WINNINGTEAM));
			winningTeam=winningTeam.Lower();
			roundNearlyFinished=true;
			STATSGEN_DEBUG(DEBUG_RARELY,winningTeam)
		} else
		{
			// Dropped through
			if (roundNearlyFinished)
			{
				roundFinished=true;
			}
		}
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Finished Decoding Objective File")
	if ((roundFinished) && 
			(gameType.Length()>0) &&
			(mapName.Length()>0) &&
			(mapStartTime!=0) &&
			(mapEndTime!=0) &&
			(playerNames.GetCount()!=0)
		)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Decoding Kill File")
		Round	currentRound;


		// Located round details - time to search for kills
		// in the primary logfile
		// First lets sort the player names into length order
		SortPlayersByLength(playerNames,playerTeams,playerClasses);
		roundFinished=false;
		roundStarted=false;
		inRound=false;
		wxDateTime	roundStartTime(mapStartTime);
		// set up the current round values
		currentRound.Initiate(this,roundStartTime,lineRead);
		currentRound.SetStartingCondition(serverIndex,
							mapName,
							mapEndTime-mapStartTime,
							gameType);
		while (!logfileBuffer.Eof() && (!roundFinished))
		{
			currentPosition=secondaryLogfileBuffer.TellI();
			currentPosition+=logfileBuffer.TellI();
			progress->Update(currentPosition);
			lastRoundEndedAt=logfileBuffer.TellI();
			lineRead=ReadLine(logfileBuffer,errorData);
			STATSGEN_DEBUG(DEBUG_RARELY,lineRead)
			lineTime=StripTimeFromLine(lineRead);
			if (lineRead.StartsWith(QUAKEWARS_LOGLINEPREFIX_PLAYER_PBGUID))
			{
					wxString	workingLine;
					wxString	pbGUID;
					wxString	playerName;
					int			playerIndex;
					workingLine=lineRead.AfterFirst('[');
					pbGUID=workingLine.BeforeFirst(']');
					workingLine=workingLine.AfterFirst(']');
					playerName=workingLine.AfterFirst('"');
					playerName=playerName.BeforeLast('"');
					playerIndex=pbNames.Index(playerName);
					if (playerIndex!=wxNOT_FOUND)
					{
						pbGUIDs.RemoveAt(playerIndex);
						pbNames.RemoveAt(playerIndex);
					}
					pbGUIDs.Add(pbGUID);
					pbNames.Add(playerName);
			}
			else if (lineRead.StartsWith("si_version"))
			{
				wxString	variable;
				wxString	value;

				variable=lineRead.BeforeFirst(':');
				value=lineRead.AfterFirst(' ');

				serverVariables.Add(variable);
				serverValues.Add(value);
			}
			else if (lineRead.StartsWith("g_version"))
			{
				variable=lineRead.BeforeFirst(':');
				value=lineRead.AfterFirst(' ');

				serverVariables.Add(variable);
				serverValues.Add(value);
			}

			useThisLine=true;
			// At this point i should have a line and it's time
			if (lineTime>0)
			{
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"line time found")
				// we have decoded a line time
				// we can use this to identify if this kill
				// is within a round or not
				if ((lineTime>=mapStartTime) && (lineTime<=mapEndTime))
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"line time within range")
					inRound=true;
				}
				if (lineTime>mapEndTime)
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"line time greater than range")
					// After end of round
					inRound=false;
					roundFinished=true;
					// This line is actually part of next round so rewind
					logfileBuffer.SeekI(lastRoundEndedAt);
				}
				if (lineTime<mapStartTime)
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"line time less than range")
					// before beginning of round
					inRound=false;
					roundFinished=false;
				}
			}
			else
			{
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"no line time found")
				// We do not have line times to guide us with
				// we can only use the log entries to determine status
				// round start is
				// "All Stats Cleared" to mark the beginning of a new round
				if (lineRead.Cmp(QUAKEWARS_LOGLINEPREFIX_ALLSTATSCLEARED)==0)
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"all stats cleared found")
					useThisLine=false;
					// Encountered start of new round
					if (inRound)
					{
						roundFinished=true;
						roundStarted=false;
						inRound=false;
						// rewinding back to beginning of this line
						logfileBuffer.SeekI(lastRoundEndedAt);
					}
					else
					{
						roundStarted=true;
						roundFinished=false;
						inRound=true;
					}
				}
				// Lets give this line a default time 
				lineTime=mapStartTime;
			}

			// ok - if we have decided we are in a round we can look
			// at the line and decide if it is a kill or not
			if (inRound && useThisLine)
			{
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"checking this line for kills")
				// Ok = we have a line to check
				// replace any player names in it with the word
				// PLAYER
				decodedLine=SubstitutePlayerNamesInLine(lineRead,playerNames);
				if (decodedLine.StartsWith(QUAKEWARS_LOGLINEPREFIX_PLAYER_JOIN))
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"this is a join line - ignore it")
					// Ignore loglines that are join lines
					wxString	teamName;
					// ends with a kill - does it start with a kill?
					teamName=decodedLine.Mid(strlen(QUAKEWARS_LOGLINEPREFIX_PLAYER_JOIN));
					playerName=lineRead.Left(lineRead.Length()-
												(teamName.Length()+
												strlen(QUAKEWARS_LOGLINEPREFIX_PLAYER_JOIN2)));
					playerIndex=playerNames.Index(playerName);
					if (playerIndex!=wxNOT_FOUND)
					{
						playerClass=playerClasses.Item(playerIndex);

						playerNames.RemoveAt(playerIndex);
						playerClasses.RemoveAt(playerIndex);
						playerTeams.RemoveAt(playerIndex);

						playerNames.Add(playerName);
						//playerTeams.Add(playerTeam);
						playerTeams.Add(teamName);
						playerClasses.Add(playerClass);
					}
				}
				else
				if (decodedLine.StartsWith(QUAKEWARS_LOGLINEPREFIX_PLAYER_SPEECH))
				{
					DecodedSpeech	speech;
					wxString		speechString;
					int				speechIndex;
					wxDateTime		speechTime;

					speechTime.Set(mapEndTime);
					speechString=decodedLine.AfterFirst(' ');
					speechIndex=lineRead.Length()-speechString.Length();
					speech.playerName=lineRead.Left(speechIndex-2);
					speech.speech=lineRead.Mid(speech.playerName.Length()+2);
					playerGUID=PlayerGUID(speech.playerName,pbGUIDs,pbNames);
					speech.playerID=playerGUID;
					currentRound.AddSpeech(speech,speechTime);
				}
				else
				if (EndsWith(decodedLine,QUAKEWARS_LOGLINESUFFIX_PLAYER_KILL))
				{
					STATSGEN_DEBUG(DEBUG_RARELY,(char *)"line ends with kill suffix")
					// ends with a kill - does it start with a kill?
					if (decodedLine.StartsWith("[") ||
						decodedLine.StartsWith(QUAKEWARS_LOGLINEPREFIX_PLAYER_KILL))
					{
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"so far this appears to be a kill line")
						// OK - this appears to be a kill line
						// i.e. player [weapon] player
						// or 
						// [weapon] player
						// once we work out the weapon - we can work out
						// who comes before the weapon (the player)
						// and who comes out after the weapon (the target)
						weapon=decodedLine.AfterFirst('[');
						weapon=weapon.BeforeLast(']');
						weaponString="[" + weapon + "] ";
						weaponIndex=lineRead.Find(weaponString);
						playerName="";
						targetName="";
						if (weaponIndex>=0)
						{
							if (weaponIndex>0)
							{
								playerName=lineRead.Left(weaponIndex-1);
							}
							else
							{
								// No player name
								playerName="";
							}
							targetName=lineRead.Mid(weaponIndex+weaponString.Length());
							// At this point we have a weapon, player name,
							// and target name
							playerIndex=playerNames.Index(playerName);
							targetIndex=playerNames.Index(targetName);
							if (playerName.Length()==0)
							{
								playerName="";
								playerTeam="";
								playerClass="";
								playerIndex=0;
							}
							else
							{
								if (playerIndex!=wxNOT_FOUND)
								{
									playerTeam=playerTeams.Item(playerIndex);
									playerClass=playerClasses.Item(playerIndex);
								}
							}
							if (targetIndex!=wxNOT_FOUND)
							{
								targetTeam=playerTeams.Item(targetIndex);
								targetClass=playerClasses.Item(targetIndex);
							}
							if ((targetIndex!=wxNOT_FOUND) &&
								((playerIndex!=wxNOT_FOUND) || (playerName.Length()==0))
								)
							{
								DecodedKill		decodedKill;
								wxString msg;
								msg.Printf("player [%s] class [%s] team [%s] "
											"weapon [%s] "
											"target [%s] class [%s] team [%s]",
									STRING_TO_CHAR(playerName),
									STRING_TO_CHAR(playerClass),
									STRING_TO_CHAR(playerTeam),
									STRING_TO_CHAR(weapon),
									STRING_TO_CHAR(targetName),
									STRING_TO_CHAR(targetClass),
									STRING_TO_CHAR(targetTeam)
									);
								STATSGEN_DEBUG(DEBUG_RARELY,msg)

								// we have a valid
								// player name
								// target name
								// weapon
								// This does appear to be a kill
								if (playerName.Length()==0)
								{
									playerName=WORLD_PLAYER_NAME;
									playerTeam=WORLD_PLAYER_TEAM;
									playerGUID=WORLD_PLAYER_ID;
									playerClass=WORLD_PLAYER_CLASS;
								}
								else
								{
									playerGUID=PlayerGUID(playerName,pbGUIDs,pbNames);
								}
								targetGUID=PlayerGUID(targetName,pbGUIDs,pbNames);
								targetLocation=targetTeam;
								playerAmmo="none";
								decodedKill.playerName=playerName;
								decodedKill.playerTeam=playerTeam;
								decodedKill.playerClass=playerClass;
								decodedKill.playerID=playerGUID;
								decodedKill.playerAmmo=playerAmmo;
								decodedKill.playerWeapon=weapon;
				
								decodedKill.targetName=targetName;
								decodedKill.targetTeam=targetTeam;
								decodedKill.targetClass=targetClass;
								decodedKill.targetID=targetGUID;
								decodedKill.targetLocation=targetLocation;
								currentRound.AddKill(decodedKill);
							}
						}
					}
				}
			} 
		}
		// Need to add in the team wins / losses;
		for (variableIndex=0;variableIndex<serverVariables.GetCount();variableIndex++)
		{
			wxString	variable;
			wxString	value;

			variable=serverVariables.Item(variableIndex);
			value=serverValues.Item(variableIndex);
			currentRound.AddVariable(variable,value);
			thisServer->AddVariable(variable,value);
		}
		variable="hostname";
		currentRound.AddVariable(variable,serverName);
		thisServer->AddVariable(variable,serverName);
		playerCount=playerNames.GetCount();
		for (playerIndex=0;playerIndex<playerCount;playerIndex++)
		{
			playerName=playerNames.Item(playerIndex);
			playerTeam=playerTeams.Item(playerIndex);
			playerClass=playerClasses.Item(playerIndex);
			playerGUID=PlayerGUID(playerName,pbGUIDs,pbNames);
			if (playerTeam.Cmp(winningTeam)==0)
			{
				currentRound.AddTeamWin(playerGUID,playerName,playerTeam,mapStartTime,mapEndTime);
			}
			else
			{
				currentRound.AddTeamLoss(playerGUID,playerName,playerTeam,mapStartTime,mapEndTime);
			}
		}
		xpLineCount=xpLines.GetCount();
		for (xpLineIndex=0;xpLineIndex<xpLineCount;xpLineIndex++)
		{
			wxString	xpLine;
			wxString	xpKey;
			wxString	xpPointsStr;
			float		xpPoints;

			xpLine=xpLines.Item(xpLineIndex);
			// xpLine = XP: fieldops 0.000 playername
			xpKey=xpLine.AfterFirst(' ');
			// xpKey = fieldops 0.000 playername
			xpPointsStr=xpKey.AfterFirst(' ');
			// xpPointsStr = 0.000 playername
			xpKey=xpKey.BeforeFirst(' ');
			// xpKey = fieldops
			playerName=xpPointsStr.AfterFirst(' ');
			xpPointsStr=xpPointsStr.BeforeFirst(' ');
			xpPoints=atof(STRING_TO_CHAR(xpPointsStr));
			if (xpPoints > 0.000000001)
			{
				// Only add if we actually have points
				playerIndex=playerNames.Index(playerName);
				if (playerIndex!=wxNOT_FOUND)
				{
					playerGUID=PlayerGUID(playerName,pbGUIDs,pbNames);
					// ok - got enough info
					// can work out 
					currentRound.AddXPPoints(playerName,
											playerGUID,
											xpKey,
											xpPoints);
				}
			}
		}
							
		currentRound.roundDuration=wxTimeSpan::Seconds(mapEndTime-mapStartTime);
		currentRound.serverIndex=serverIndex;

		if (!currentRound.IsDropped())
		{
			globalStatistics.rounds.Add(currentRound);
			int			xpIndex;
			int			xpCount;
			XPPoints	xp;

			xpCount=currentRound.xpPoints.GetCount();
			for (xpIndex=0;xpIndex<xpCount;xpIndex++)
			{
				xp=currentRound.xpPoints.Item(xpIndex);
				globalStatistics.totalXP.Add(xp);
			}
		}
		lastRoundEndedAt=logfileBuffer.TellI();
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Did not find end of logfile")
		// reached end of file without identifying all the component parts of the round
		// if the round finished = it means we can continue reading from here
		// in the future
		// if the round did not finished = we need to rewind the file ready
		// for next time when hopefully it will be finished
		if (roundFinished)
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"but round finished")
		}
		else
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"but round not finished")
			// rewind to the beginning of the objective file round
			secondaryLogfileBuffer.SeekI(secondaryFilePosition);
			noMoreRounds=true;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (noMoreRounds);
}

void QuakeWarsLogFileReader::ProcessLogFile(Server *server,int serverIndex)
{
	MultiFile			logfile(filenames);
	MultiFile			secondaryLogfile(secondaryFilenames);
	wxString			errorMessage;
	wxFileOffset		logfileSize=0;
	wxFileOffset		secondaryLogfileSize=0;
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
	bool				finished;
	wxArrayString		pbGUIDs;
	wxArrayString		pbNames;

	STATSGEN_DEBUG_FUNCTION_START("QuakeWarsLogFileReader","ProcessLogFile")
	thisServer=server;
	if (logfile.Ok() && secondaryLogfile.Ok())
	{
		// File appears to be open
		logfileSize=logfile.Length();
		secondaryLogfileSize=secondaryLogfile.Length();
		/*
		STATSGEN_DEBUG_CODE(errorMessage.Printf(
					"File [%s] opened OK %d bytes "
					"Secondary File [%s] opened OK %d bytes",
					 filename.GetData(),
					(int)logfileSize,
					 secondaryFilename.GetData(),
					(int)secondaryLogfileSize);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
		*/
		inRound=false;

		// Seek to the last bit of the logfile read
		logfile.SeekI(lastRoundEndedAt);
		secondaryLogfile.SeekI(secondaryLastRoundEndedAt);
		STATSGEN_DEBUG_CODE(msg.Printf("Last Round Ended At %ld",lastRoundEndedAt);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		finished=false;
		while (!secondaryLogfile.Eof() && (!finished))
		{
			finished=DecodeRound(logfile,secondaryLogfile,serverIndex,pbGUIDs,pbNames);
		}
		// End of file processed
	}
	else
	{
		// Failed to open the logfile
		errorMessage="Failed to open "/* + filename + " for reading."*/;
		errorData.SetError(ERROR_CODE_FILE_OPEN, errorMessage);
		progress->LogError(errorMessage,SeverityError);
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	}
	STATSGEN_DEBUG_FUNCTION_END
}

QuakeWarsLogFileReader::~QuakeWarsLogFileReader()
{
}

void QuakeWarsLogFileReader::StartFromBeginning()
{
	STATSGEN_DEBUG_FUNCTION_START("QuakeWarsLogFileReader","StartFromBeginning")
	secondaryLastRoundEndedAt=0;
	LogFileReader::StartFromBeginning();
	STATSGEN_DEBUG_FUNCTION_END
}

bool QuakeWarsLogFileReader::IsWorldPlayer(wxString &guid,wxString &playerTeam,wxString &playerName)
{
	return ((guid.Length()==0) && (playerName.Length()==0));
}


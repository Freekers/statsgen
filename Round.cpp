// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>

// Statsgen includes
#include "GlobalStatistics.h"
#include "Round.h"
#include "LogFileReader.h"
#include "ErrorData.h"
#include "PlayerInRound.h"
#include "ConfigData.h"
#include "StaticFunctions.h"
#include "Progress.h"
#include "XPPoints.h"

void DecodedTeamWin::Clear()
{
	teamName="";
	playerIDs.Clear();
	playerRoundIDs.Clear();
	playerNames.Clear();
}

void DecodedTeamLoss::Clear()
{
	teamName="";
	playerIDs.Clear();
	playerRoundIDs.Clear();
	playerNames.Clear();
}

void RoundLine::Set(wxDateTime &lineTimeIn,wxString &lineIn)
{
	lineTime=lineTimeIn;
	line=lineIn;
}

Round::Round()
{
	logfileReader=NULL;
	dropped=false;
}

Round::~Round()
{
}

void Round::Debug()
{
	wxString	errorMessage;
	KillData	kill;
	ActionData	action;
	SpeechData	speech;
	TeamWinData	teamWin;
	TeamLossData	teamLoss;
	PlayerInRound	player;
	int		killCount;
	int		killIndex;
	int		actionCount;
	int		actionIndex;
	int		speechCount;
	int		speechIndex;
	int		playerCount;
	int		playerIndex;
	int		teamWinCount;
	int		teamWinIndex;
	int		teamLossCount;
	int		teamLossIndex;
	wxString	timeString;
	wxString	durationString;

	STATSGEN_DEBUG_FUNCTION_START("Round","Debug")
	timeString=roundStartTime.Format();
	durationString=roundDuration.Format("%H:%M:%S");
	STATSGEN_DEBUG_CODE(errorMessage.Printf("Round:[%s] Duration[%s]", timeString.GetData(), durationString.GetData());)
	STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	killCount=kills.GetCount();
	for (killIndex=0;killIndex<killCount;killIndex++)
	{
		kill=kills.Item(killIndex);
		timeString=kill.killTime.Format();
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Kill %d:[%s]GT[%s]MAP[%s]PLIDX[%d]CL[%s]TM[%s]WP[%s]AM[%s]"
				"TRIDX[%d]CL[%s]TM[%s]DM[%d]LO[%s]",
				killIndex,
				timeString.GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysGametype,kill.gameType).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysMap,kill.mapName).GetData(),
				kill.playerIndex,
				globalStatistics.GenericKeyID(globalStatistics.keysClass,kill.playerClass).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysTeam,kill.playerTeam).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysWeapon,kill.playerWeapon).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysAmmo,kill.playerAmmo).GetData(),
				kill.targetIndex,
				globalStatistics.GenericKeyID(globalStatistics.keysClass,kill.targetClass).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysTeam,kill.targetTeam).GetData(),
				kill.targetDamage,
				globalStatistics.GenericKeyID(globalStatistics.keysLocation,kill.targetLocation).GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	}
	actionCount=actions.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,"actions");
	for (actionIndex=0;actionIndex<actionCount;actionIndex++)
	{
		action=actions.Item(actionIndex);
		timeString=action.actionTime.Format();
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Action %d:[%s]GT[%s]MAP[%s]PLIDX[%d]CL[%s]TM[%s]AC[%s]",
				actionIndex,
				timeString.GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysGametype,action.gameType).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysMap,action.mapName).GetData(),
				action.playerIndex,
				globalStatistics.GenericKeyID(globalStatistics.keysClass,action.playerClass).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysTeam,action.playerTeam).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysAction,action.action).GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,"teamwins");
	teamWinCount=teamWins.GetCount();
	for (teamWinIndex=0;teamWinIndex<teamWinCount;teamWinIndex++)
	{
		teamWin=teamWins.Item(teamWinIndex);
		timeString=teamWin.winTime.Format();
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Team Win %d:[%s]GT[%s]MAP[%s]PLIDX[%d]CL[%s]TM[%s]",
				teamWinIndex,
				timeString.GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysGametype,teamWin.gameType).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysMap,teamWin.mapName).GetData(),
				teamWin.playerIndex,
				globalStatistics.GenericKeyID(globalStatistics.keysClass,teamWin.playerClass).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysTeam,teamWin.playerTeam).GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	}
	teamLossCount=teamLosses.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,"teamloss");
	for (teamLossIndex=0;teamLossIndex<teamLossCount;teamLossIndex++)
	{
		teamLoss=teamLosses.Item(teamLossIndex);
		timeString=teamLoss.lossTime.Format();
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Team Loss %d:[%s]GT[%s]MAP[%s]PLIDX[%d]CL[%s]TM[%s]",
				teamLossIndex,
				timeString.GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysGametype,teamLoss.gameType).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysMap,teamLoss.mapName).GetData(),
				teamLoss.playerIndex,
				globalStatistics.GenericKeyID(globalStatistics.keysClass,teamLoss.playerClass).GetData(),
				globalStatistics.GenericKeyID(globalStatistics.keysTeam,teamLoss.playerTeam).GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,"speech");
	speechCount=speeches.GetCount();
	for (speechIndex=0;speechIndex<speechCount;speechIndex++)
	{
		speech=speeches.Item(speechIndex);
		timeString=speech.speechTime.Format();
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Speech %d:[%s]PLIDX[%d] [%s]",
				speechIndex,
				timeString.GetData(),
				speech.playerIndex,
				speech.speech.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	}

	STATSGEN_DEBUG(DEBUG_ALWAYS,"players");
	playerCount=playersInRound.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=playersInRound.Item(playerIndex);
		player.Debug();
	}
	
	STATSGEN_DEBUG_FUNCTION_END
}

void Round::Initiate(LogFileReader *reader,
		wxDateTime &lineTime,
		wxString &line)
{
	STATSGEN_DEBUG_FUNCTION_START("Round","Initiate")
	RoundLine	roundLine;
	wxString	timeString;
	logfileReader=reader;
	roundStartTime=lineTime;

	roundLine.Set(lineTime,line);

	roundLines.Clear();
	roundLines.Add(roundLine);
	kills.Clear();
	actions.Clear();
	speeches.Clear();
	playersInRound.Clear();
	teamWins.Clear();
	teamLosses.Clear();

	serverType=logfileReader->GetServerType();
	teamWinAlreadyFound=false;
	teamLossAlreadyFound=false;
	STATSGEN_DEBUG_FUNCTION_END
}

bool Round::IsDropped()
{
	bool		retVal=true;
	wxString	minPlayersInRoundString;
	int			minPlayersInRound;
	wxString	msg;

	// Minimum players in round is a "Hard" drop
	// i.e. we don't want to count any of these stats
	// at all
	STATSGEN_DEBUG_FUNCTION_START("Round","IsDropped")

	globalStatistics.configData.ReadTextValue(
									"/General/MinPlayersInRound",
									&minPlayersInRoundString,
									"1");

	minPlayersInRound=atoi(minPlayersInRoundString.GetData());

	STATSGEN_DEBUG_CODE(msg.Printf("kills=%d, actions=%d, playersinround=%d, minPlayersInRound=%d",kills.GetCount(),actions.GetCount(),playersInRound.GetCount(),minPlayersInRound);)
	STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
	retVal=false;
	if (((kills.GetCount()==0) &&
		 (actions.GetCount()==0)) ||
		(playersInRound.GetCount()<minPlayersInRound)
		)
	{
		retVal=true;
	}
		
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void Round::UpdatePlayerInRoundTime(int playerIndex,wxDateTime &timeStamp,bool resetTime)
{
	int		playerCount;
	int		playerInRoundIndex;
	PlayerInRound	player;
	PlayerInRound	newPlayer;
	bool		playerFound=false;

	newPlayer.playerIndex=playerIndex;
	newPlayer.LogTime(timeStamp,resetTime);

	playerCount=playersInRound.GetCount();
	for (playerInRoundIndex=0;playerInRoundIndex<playerCount;playerInRoundIndex++)
	{
		player=playersInRound.Item(playerInRoundIndex);
		if (player.playerIndex==playerIndex)
		{
			// found an existing player
			// Update it's timestamp
			playerFound=true;
			player.LogTime(timeStamp,resetTime);
			// Remove it
			playersInRound.RemoveAt(playerInRoundIndex);
			// Add it again
			playersInRound.Add(player);
			break;
		}
	}

	if (!playerFound)
	{
		playersInRound.Add(newPlayer);
	}
}

void Round::AddLogLine(wxDateTime &lineTime, wxString &line)
{
	RoundLine	roundLine;

	roundLine.Set(lineTime,line);
	roundLines.Add(roundLine);
}

void Round::AddKill(DecodedKill &decode,
			wxDateTime &killTime,
			int gameType,
			int mapName)
{
	KillData	kill;
	wxString	playerID;
	wxString	playerClass;
	wxString	playerTeam;
	wxString	playerWeapon;
	wxString	playerAmmo;
	wxString	targetID;
	wxString	targetClass;
	wxString	targetTeam;
	wxString	targetLocation;
	wxString	weaponAndAmmo;
	wxString	playerName;
	wxString	targetName;
	bool		cheat;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("Round","AddKill")
	
	playerClass=serverType+"_"+decode.playerClass.Lower();
	playerWeapon=serverType+"_"+decode.playerWeapon.Lower();
	playerAmmo=serverType+"_"+decode.playerAmmo;
	targetClass=serverType+"_"+decode.targetClass.Lower();
	targetTeam=serverType+"_"+decode.targetTeam.Lower();
	targetLocation=serverType+"_"+decode.targetLocation.Lower();

	SafeString(playerClass);
	SafeString(playerWeapon);
	SafeString(playerAmmo);
	SafeString(targetClass);
	SafeString(targetTeam);
	SafeString(targetLocation);

	if (logfileReader->IsWorldPlayer(decode.playerID,decode.playerTeam,decode.playerName))
	{
		playerID=WORLD_PLAYER_ID;
		playerName=WORLD_PLAYER_NAME;
		playerTeam=WORLD_PLAYER_TEAM;
	}
	else
	{
		playerID=serverType+"_"+decode.playerID;
		playerName=decode.playerName;
		playerTeam=serverType+"_"+decode.playerTeam.Lower();
		cheat=false;
		if (globalStatistics.dropList.IsDropped(playerName,&cheat))
		{
			if (cheat)
			{
				// Cheats get no kills
				globalStatistics.cheaters.Add(playerName);
				STATSGEN_DEBUG_FUNCTION_END
				return;
			}
		}
	}

	if (logfileReader->IsWorldPlayer(decode.targetID,decode.targetTeam,decode.targetName))
	{
		targetID=WORLD_PLAYER_ID;
		targetName=WORLD_PLAYER_NAME;
		targetTeam=WORLD_PLAYER_TEAM;
	}
	else
	{
			cheat=false;
			globalStatistics.dropList.IsDropped(decode.targetName,&cheat);
			if (cheat)
			{
				// If the target is a cheat - make it appear as the 
				// world player - then it will be counted as a kill
				// by the player but will get dropped along with the
				// other stats
				globalStatistics.cheaters.Add(decode.targetName);
				targetID=WORLD_PLAYER_ID;
				targetName=WORLD_PLAYER_NAME;
				targetTeam=WORLD_PLAYER_TEAM;
			}
			else
			{
				targetID=serverType+"_"+decode.targetID;
				targetTeam=serverType+"_"+decode.targetTeam.Lower();
				targetName=decode.targetName;
			}
	}

	weaponAndAmmo=playerWeapon+playerAmmo;
	SubstituteLogEntry("CLASS",playerClass);
	SubstituteLogEntry("TEAM",playerTeam);
	SubstituteLogEntry("WEAPON",playerWeapon);

	if (SubstituteLogEntry("AMMO",playerAmmo))
	{
		// we made an ammo substitution
		playerWeapon=playerAmmo;
	}
	if (SubstituteLogEntry("WEAPONAMMO",weaponAndAmmo))
	{
		// we made an ammo substitution
		playerWeapon=weaponAndAmmo;
	}


	SubstituteLogEntry("CLASS",targetClass);
	SubstituteLogEntry("TEAM",targetTeam);
	SubstituteLogEntry("LOCATION",targetLocation);

	StoreLogEntry("WEAPON",playerWeapon);
	StoreLogEntry("LOCATION",targetLocation);

	kill.killTime=killTime;
	kill.gameType=gameType;
	kill.mapName=mapName;
	kill.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
	kill.playerClass=globalStatistics.GenericKeyAdd(globalStatistics.keysClass,playerClass);
	kill.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,playerTeam);
	kill.playerWeapon=globalStatistics.GenericKeyAdd(globalStatistics.keysWeapon,playerWeapon);
	kill.playerAmmo=globalStatistics.GenericKeyAdd(globalStatistics.keysAmmo,playerAmmo);
	kill.targetIndex=globalStatistics.AddPlayer(targetName,targetID);
	kill.targetClass=globalStatistics.GenericKeyAdd(globalStatistics.keysClass,targetClass);
	kill.targetTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,targetTeam);
	kill.targetDamage=atoi(decode.targetDamage);
	kill.targetLocation=globalStatistics.GenericKeyAdd(globalStatistics.keysLocation,targetLocation);
	kills.Add(kill);
	STATSGEN_DEBUG_CODE(
	msg.Printf("player:Name[%s],id[%s],Class[%s],Team[%s],Weapon[%s] target:Name[%s],id[%s],class[%s],team[%s],location[%s]",
		playerName.GetData(),
		playerID.GetData(),
		playerClass.GetData(),
		playerTeam.GetData(),
		playerWeapon.GetData(),
		targetName.GetData(),
		targetID.GetData(),
		targetClass.GetData(),
		targetTeam.GetData(),
		targetLocation.GetData());
						)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	UpdatePlayerInRoundTime(kill.playerIndex,killTime,false);
	UpdatePlayerInRoundTime(kill.targetIndex,killTime,false);
	STATSGEN_DEBUG_FUNCTION_END
}

bool Round::WordAtTextPosition(wxString &speechLine,
								int speechStartIndex,
								int speechLength,
								wxString &bannedWord,
								int *replacementLength)
{
	bool 		retVal;
	wxString	speech;
	int			bannedWordLength;

	bannedWordLength=bannedWord.Length();
	speech=speechLine.Mid(speechStartIndex).Left(bannedWordLength);
	retVal=(speech.CmpNoCase(bannedWord)==0);
	if (retVal)
	{
		(*replacementLength)=bannedWordLength;
	}
	return (retVal);
}

void Round::AddSpeech(DecodedSpeech &decode,
			wxDateTime &speechTime)
{
	SpeechData	speech;
	wxString	playerID=serverType+"_"+decode.playerID;
	bool		cheat=false;
	int		bannedSpeechCount;
	int		bannedSpeechIndex;
	wxString	bannedWord;
	int		speechLength;
	int		speechIndex;
	bool	speechFound;
	int		replacementLength;
	wxString	msg;
	wxString	newSpeech;

	STATSGEN_DEBUG_FUNCTION_START("Round","AddSpeech")

	speech.speechTime=speechTime;
	cheat=false;
	globalStatistics.WriteSpeechFile(speechTime,playerID,decode.playerName,decode.speech);
	if (globalStatistics.dropList.IsDropped(decode.playerName,&cheat))
	{
		if (cheat)
		{
			globalStatistics.cheaters.Add(decode.playerName);
			// no cheats get speech
			STATSGEN_DEBUG_FUNCTION_END
			return;
		}
	}
	speech.playerIndex=globalStatistics.AddPlayer(decode.playerName,playerID);
	speechLength=decode.speech.Length();

	bannedSpeechCount=globalStatistics.bannedSpeech.GetCount();
	if (bannedSpeechCount>0)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Checking Banned Speech")
		speechIndex=0;
		while (speechIndex<speechLength)
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Speech Index=%d",speechIndex);)
			STATSGEN_DEBUG(DEBUG_RARELY,msg)
			speechFound=false;
			for (bannedSpeechIndex=0;
				bannedSpeechIndex<bannedSpeechCount;
				bannedSpeechIndex++)
			{
				bannedWord=globalStatistics.bannedSpeech.Item(bannedSpeechIndex);
				STATSGEN_DEBUG(DEBUG_RARELY,bannedWord)
				if (WordAtTextPosition(
						decode.speech,
						speechIndex,
						speechLength,
						bannedWord,
						&replacementLength))
				{
					STATSGEN_DEBUG(DEBUG_RARELY,"Found it")
					speechFound=true;
					break;
				}
			}
			if (speechFound)
			{
				// We have found banned speech at this position
				// replace it and advance past it
				newSpeech=decode.speech.Left(speechIndex)+
							"****" +
							decode.speech.Mid(speechIndex+replacementLength);
				decode.speech=newSpeech;
				speechLength=decode.speech.Length();
				speechIndex+=replacementLength;
			}
			else
			{
				speechIndex++;
			}
		}
	}
	speech.speech=decode.speech;
	
	speeches.Add(speech);
	UpdatePlayerInRoundTime(speech.playerIndex,speechTime,false);
	STATSGEN_DEBUG_FUNCTION_END
}

void Round::AddJoin(DecodedPlayerJoin &decode,
			wxDateTime &joinTime)
{
	int			playerIndex;
	bool		cheat;
	wxString	playerID=serverType+"_"+decode.playerID;

	cheat=false;
	if (globalStatistics.dropList.IsDropped(decode.playerName,&cheat))
	{
		if (cheat)
		{
			globalStatistics.cheaters.Add(decode.playerName);
			return;
		}
	}
	playerIndex=globalStatistics.AddPlayer(decode.playerName,playerID);
	UpdatePlayerInRoundTime(playerIndex,joinTime,true);
}

void Round::AddQuit(DecodedPlayerQuit &decode,
			wxDateTime &quitTime)
{
	int			playerIndex;
	bool		cheat;
	wxString	playerID=serverType+"_"+decode.playerID;

	cheat=false;
	if (globalStatistics.dropList.IsDropped(decode.playerName,&cheat))
	{
		if (cheat)
		{
			globalStatistics.cheaters.Add(decode.playerName);
			return;
		}
	}
	playerIndex=globalStatistics.AddPlayer(decode.playerName,playerID);
	UpdatePlayerInRoundTime(playerIndex,quitTime,false);
}

bool Round::SubstituteLogEntry(const char *logEntryType,
						wxString &logEntry)
{
	wxString	configKey;
	wxString	configValue;
	bool		retVal;

	configKey.Printf("/LogSubstitute%s/%s",logEntryType,logEntry.GetData());
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
	if (configValue.Length()>0)
	{
		// entry found - substitute
		logEntry=configValue;
		retVal=true;
	}
	else
	{
		// not found - so no change
		retVal=false;
	}

	return (retVal);
}

void Round::StoreLogEntry(const char *logEntryType,
						wxString &logEntry)
{
	wxString	configKey;

	configKey.Printf("/LogEntries%s/%s",logEntryType,logEntry.GetData());
	globalStatistics.configData.WriteTextValue(configKey,"");
}

void Round::AddAction(DecodedAction &decode,
			wxDateTime &actionTime,
			int gameType,
			int mapName)
{
	ActionData	action;
	bool		cheat;
	wxString	msg;
	wxString	playerID=serverType+"_"+decode.playerID;
	wxString	playerClass=serverType+"_"+decode.playerClass.Lower();
	wxString	playerTeam=serverType+"_"+decode.playerTeam.Lower();
	wxString	playerAction=serverType+"_"+decode.action.Lower();

	STATSGEN_DEBUG_FUNCTION_START("Round","AddAction")

	STATSGEN_DEBUG_CODE(
	msg.Printf("playerName[%s] playerID[%s] playerClass[%s] playerTeam[%s] action[%s]",
						decode.playerName.GetData(),
						decode.playerID.GetData(),
						decode.playerClass.GetData(),
						decode.playerTeam.GetData(),
						decode.action.GetData());
						)
	STATSGEN_DEBUG(DEBUG_RARELY,msg);
	SafeString(playerClass);
	SafeString(playerTeam);
	SafeString(playerAction);
	cheat=false;
	if (globalStatistics.dropList.IsDropped(decode.playerName,&cheat))
	{
		if (cheat)
		{
			globalStatistics.cheaters.Add(decode.playerName);
			STATSGEN_DEBUG_FUNCTION_END
			return;
		}
	}
	SubstituteLogEntry("CLASS",playerClass);
	SubstituteLogEntry("TEAM",playerTeam);
	SubstituteLogEntry("ACTION",playerAction);

	StoreLogEntry("ACTION",playerAction);

	action.actionTime=actionTime;
	action.gameType=gameType;
	action.mapName=mapName;
	action.playerIndex=globalStatistics.AddPlayer(decode.playerName,playerID);
	action.playerClass=globalStatistics.GenericKeyAdd(globalStatistics.keysClass,playerClass);
	action.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,playerTeam);
	action.action=globalStatistics.GenericKeyAdd(globalStatistics.keysAction,playerAction);
	actions.Add(action);
	UpdatePlayerInRoundTime(action.playerIndex,actionTime,false);
	STATSGEN_DEBUG_FUNCTION_END
}

void Round::AddTeamWin(DecodedTeamWin &decode,
			wxDateTime &winTime,
			int gameType,
			int mapName)
{
	TeamWinData	playerData;
	bool		cheat;
	int			playerCount;
	int			playerIndex;
	wxString	playerName;
	wxString	playerID;
	wxString	msg;
	wxString	teamName=serverType+"_"+decode.teamName.Lower();

	STATSGEN_DEBUG_FUNCTION_START("Round","AddTeamWin")
	
	SubstituteLogEntry("TEAM",teamName);

	playerCount=decode.playerIDs.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		playerName=decode.playerNames.Item(playerIndex);
		STATSGEN_DEBUG_CODE(
		msg.Printf("Name[%s],Team[%s]",
					playerName.GetData(),
					teamName.GetData());
							)
		
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
		cheat=false;
		if (globalStatistics.dropList.IsDropped(playerName,&cheat))
		{
			if (cheat)
			{
				globalStatistics.cheaters.Add(playerName);
				continue;
			}
		}
		playerID=serverType+"_"+decode.playerIDs.Item(playerIndex);
		playerData.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
		playerData.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,teamName);
		playerData.gameType=gameType;
		playerData.mapName=mapName;
		playerData.winTime=winTime;
		playerData.playerClass=0;
		teamWins.Add(playerData);
		UpdatePlayerInRoundTime(playerData.playerIndex,winTime,false);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void Round::AddTeamLoss(DecodedTeamLoss &decode,
			wxDateTime &lossTime,
			int gameType,
			int mapName)
{
	TeamLossData	playerData;
	bool			cheat;
	int				playerCount;
	int				playerIndex;
	wxString		playerName;
	wxString		playerID;
	wxString		msg;
	wxString		teamName=serverType+"_"+decode.teamName.Lower();
	
	STATSGEN_DEBUG_FUNCTION_START("Round","AddTeamLoss")
	playerCount=decode.playerIDs.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		playerName=decode.playerNames.Item(playerIndex);
		STATSGEN_DEBUG_CODE(
		msg.Printf("Name[%s],Team[%s]",
					playerName.GetData(),
					teamName.GetData());
							)
		
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
		cheat=false;
		if (globalStatistics.dropList.IsDropped(playerName,&cheat))
		{
			if (cheat)
			{
				globalStatistics.cheaters.Add(playerName);
				continue;
			}
		}
		playerID=serverType+"_"+decode.playerIDs.Item(playerIndex);
		playerData.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
		playerData.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,teamName);
		playerData.gameType=gameType;
		playerData.mapName=mapName;
		playerData.playerClass=0;
		playerData.lossTime=lossTime;
		teamLosses.Add(playerData);
		UpdatePlayerInRoundTime(playerData.playerIndex,lossTime,false);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void Round::Process(ErrorData &errorData,int serverIndexIn)
{
	int					roundLinesCount;
	int					roundLinesIndex;
	RoundLine			roundLine;
	DecodedRoundStart	decodeRoundStart;
	DecodedPlayerJoin	decodePlayerJoin;
	DecodedPlayerQuit	decodePlayerQuit;
	DecodedKill			decodeKill;
	DecodedTeamWin		decodeTeamWin;
	DecodedTeamLoss		decodeTeamLoss;
	DecodedAction		decodeAction;
	DecodedSpeech		decodeSpeech;
	wxString			playerID;
	wxString			playerRoundID;
	wxString			playerName;
	wxString			mapNameString;
	wxString			gameTypeString;
	wxString			msg;

	STATSGEN_DEBUG_FUNCTION_START("Round","Process")
	serverIndex=serverIndexIn;
	roundLinesCount=roundLines.GetCount();
	roundDuration=roundStartTime.Subtract(roundStartTime);
	for (roundLinesIndex=0;
		roundLinesIndex<roundLinesCount;
		roundLinesIndex++)
	{
		roundLine=roundLines.Item(roundLinesIndex);
		progress->WakeUp();
		STATSGEN_DEBUG(DEBUG_RARELY,roundLine.line)
		//if (roundLinesIndex==(roundLinesCount-1))
		//{
			roundDuration=roundLine.lineTime.Subtract(roundStartTime);
		//}
		if (roundDuration<0)
		{
			roundStartTime=roundLine.lineTime;
			roundDuration=roundStartTime.Subtract(roundStartTime);
		}
		if (logfileReader->DecodeRoundStart(roundLine.line,
					decodeRoundStart,
					this))
		{
			roundStartTime=roundLine.lineTime;
			roundDuration=roundStartTime.Subtract(roundStartTime);
			if (decodeRoundStart.mapName.Length()>0)
			{
				mapNameString=serverType+"_"+decodeRoundStart.mapName.Lower();
				SubstituteLogEntry("MAP",mapNameString);
				mapName=globalStatistics.GenericKeyAdd(globalStatistics.keysMap,mapNameString);
			}
			if (decodeRoundStart.gameType.Length()>0)
			{
				gameTypeString=serverType+"_"+decodeRoundStart.gameType.Lower();
				SubstituteLogEntry("GAMETYPE",gameTypeString);
				gameType=globalStatistics.GenericKeyAdd(globalStatistics.keysGametype,gameTypeString);
			}

		}
		else
		if (logfileReader->DecodeSpeech(roundLine.line,
					decodeSpeech))
		{
			AddSpeech(decodeSpeech,roundLine.lineTime);
		}
		else
		if (logfileReader->DecodeAction(roundLine.line,
					decodeAction))
		{
			AddAction(decodeAction,roundLine.lineTime,gameType,mapName);
		}
		else
		if (logfileReader->DecodePlayerJoin(roundLine.line,
					decodePlayerJoin))
		{
			AddJoin(decodePlayerJoin,roundLine.lineTime);
		}
		else
		if (logfileReader->DecodePlayerQuit(roundLine.line,
					decodePlayerQuit))
		{
			AddQuit(decodePlayerQuit,roundLine.lineTime);
		}
		else
		if (logfileReader->DecodeKill(roundLine.line,
					decodeKill))
		{
			if (decodeKill.targetTeam.Length()==0)
			{
				decodeKill.targetTeam="targetteam";
			}
			if (decodeKill.playerTeam.Length()==0)
			{
				decodeKill.playerTeam="playerteam";
			}
			AddKill(decodeKill,roundLine.lineTime,gameType,mapName);
		}
		else
		if ((!teamWinAlreadyFound) && (logfileReader->DecodeTeamWin(roundLine.line,
					decodeTeamWin)))
		{
			AddTeamWin(decodeTeamWin,roundLine.lineTime,gameType,mapName);
			if (serverType.CmpNoCase(SERVER_TYPE_COD5)!=0)
			{
				teamWinAlreadyFound=true;
			}
		}
		else
		if ((!teamLossAlreadyFound) && (logfileReader->DecodeTeamLoss(roundLine.line,
					decodeTeamLoss)))
		{
			AddTeamLoss(decodeTeamLoss,roundLine.lineTime,gameType,mapName);
			if (serverType.CmpNoCase(SERVER_TYPE_COD5)!=0)
			{
				teamLossAlreadyFound=true;
			}
		}
	}


	
	// End of round - can remove round lines
	roundLines.Clear();

	STATSGEN_DEBUG_FUNCTION_END
}

void Round::CalculateScores(int roundIndex)
{
	int				playerCount;
	int				playerInRoundIndex;
	int				killCount;
	int				actionCount;
	int				winCount;
	int				lossCount;
	int				killIndex;
	int				actionIndex;
	int				winIndex;
	int				lossIndex;
	wxString		configKey;
	wxString		configValue;

	StreakData		streak;
	int				streakIndex;
	int				streakCount;

	PlayerInRound	playerInRound;
	Player			player;
	ActionData		action;
	KillData		kill;
	TeamWinData		teamWin;
	TeamLossData	teamLoss;

	float			score;
	float			skill;
	float			weaponSkill;
	float			locationSkill;

	KillTypes		killType;
	wxString		weaponStr;
	wxString		locationStr;
	wxString		actionStr;

	float			teamLossScoreWeight;
	float			teamWinScoreWeight;
	float			killScoreWeight;
	float			deathScoreWeight;
	float			teamKillScoreWeight;
	float			teamSwapScoreWeight;
	float			suicideScoreWeight;
	float			gametypeScoreWeight;
	float			gametypeSkillWeight;
	int				speechCount;
	int				speechPoints;
	int				speechIndex;
	bool			playerInRoundUseful;
	int				xpCount;
	int				xpIndex;
	XPPoints		xp;
	wxString		xpStr;

	if (dropped)
	{
		// don't calculate if round dropped
		return;
	}

	teamWinScoreWeight=globalStatistics.ScoreWeight("TeamWin",1.0);

	teamLossScoreWeight=globalStatistics.ScoreWeight("TeamLoss",1.0);

	killScoreWeight=globalStatistics.ScoreWeight("Kill",1.0);

	deathScoreWeight=globalStatistics.ScoreWeight("Death",-1.0);

	suicideScoreWeight=globalStatistics.ScoreWeight("Suicide",-3.0);

	teamKillScoreWeight=globalStatistics.ScoreWeight("TeamKill",-2.0);
	teamSwapScoreWeight=globalStatistics.ScoreWeight("TeamSwap",0.0);
	gametypeScoreWeight=globalStatistics.GametypeScoreWeight(gameType);
	gametypeSkillWeight=globalStatistics.GametypeSkillWeight(gameType);

	playerCount=playersInRound.GetCount();
	killCount=kills.GetCount();
	actionCount=actions.GetCount();
	winCount=teamWins.GetCount();
	lossCount=teamLosses.GetCount();

	speechCount=speeches.GetCount();
	playerInRoundIndex=0;
	while (playerInRoundIndex<playerCount)
	{
		playerInRoundUseful=false;
		playerInRound=playersInRound.Item(0);
		playerInRound.ZeroAwardPoints();
		player=globalStatistics.playerList.Item(playerInRound.playerIndex);
		player.lastRoundPlayed=roundIndex;
		if (player.firstRoundPlayed==-1)
		{
			player.firstRoundPlayed=roundIndex;
		}
		//if (!player.dropped)
		{
			speechPoints=0;
			for (speechIndex=0;speechIndex<speechCount;speechIndex++)
			{
				SpeechData	speech=speeches.Item(speechIndex);
				if (speech.playerIndex==playerInRound.playerIndex)
				{
					speechPoints++;
					playerInRoundUseful=true;
				}
			}
			if (speechPoints>0)
			{
				playerInRound.UpdateMiscAwardPoints("Speech",speechPoints);
			}
			// don't calculate players score if they are dropped

			score=0.0;
			skill=0.0;
			// keep getting the first item as we are gonna remove it and move it to the end
			playerInRound.CalculateStreaks(&kills,gameType);
			// XP Scores
			xpCount=xpPoints.GetCount();
			for (xpIndex=0;xpIndex<xpCount;xpIndex++)
			{
				xp=xpPoints.Item(xpIndex);
				if (xp.playerIndex==playerInRound.playerIndex)
				{
					playerInRoundUseful=true;
					// This xp belongs to this person
					xpStr=globalStatistics.GenericKeyID(globalStatistics.keysXP,xp.pointsIndex);
					skill+=(globalStatistics.SkillWeight(xpStr,"xp",0.0) * xp.points);
					score+=(globalStatistics.ScoreWeight(xpStr,"xp",0.0) * xp.points);
					playerInRound.UpdateXPAwardPoints(xpStr,xp.points);
				}
			}
			// Action Scores
			for (actionIndex=0;actionIndex<actionCount;actionIndex++)
			{
				action=actions.Item(actionIndex);
				if (action.playerIndex==playerInRound.playerIndex)
				{
					playerInRoundUseful=true;
					// This action belongs to this person
					actionStr=globalStatistics.GenericKeyID(globalStatistics.keysAction,action.action);
					score+=globalStatistics.ScoreWeight(actionStr,"action",1.0);
					playerInRound.UpdateActionAwardPoints(actionStr);
				}
			}
			// Team Wins
			for (winIndex=0;winIndex<winCount;winIndex++)
			{
				teamWin=teamWins.Item(winIndex);
				if (teamWin.playerIndex==playerInRound.playerIndex)
				{
					playerInRoundUseful=true;
					// This win belongs to this person
					score+=teamWinScoreWeight;
					playerInRound.UpdateMiscAwardPoints("Team Win",1);
					playerInRound.teamWins=1;
				}
			}
			// Team Losses
			for (lossIndex=0;lossIndex<lossCount;lossIndex++)
			{
				teamLoss=teamLosses.Item(lossIndex);
				if (teamLoss.playerIndex==playerInRound.playerIndex)
				{
					playerInRoundUseful=true;
					// This loss belongs to this person
					score+=teamLossScoreWeight;
					playerInRound.UpdateMiscAwardPoints("Team Loss",1);
					playerInRound.teamLosses=1;
				}
			}
			// Kills / Deaths
			for (killIndex=0;killIndex<killCount;killIndex++)
			{
				kill=kills.Item(killIndex);
				if ((kill.playerIndex==playerInRound.playerIndex)||
					(kill.targetIndex==playerInRound.playerIndex))
				{
					playerInRoundUseful=true;
					weaponStr=globalStatistics.GenericKeyID(globalStatistics.keysWeapon,
															kill.playerWeapon);
					locationStr=globalStatistics.GenericKeyID(globalStatistics.keysLocation,
															kill.targetLocation);
					weaponSkill=globalStatistics.SkillWeight(weaponStr,"weapon",1.0);
					locationSkill=globalStatistics.SkillWeight(locationStr,"location",1.0);

					killType=kill.KillType(gameType,
											playerInRound.playerIndex);
					switch (killType)
					{
						case KILL_TYPE_KILL:
							// Kill
							playerInRound.kills++;
							score+=killScoreWeight;
							skill+=weaponSkill;
							skill+=locationSkill;
							playerInRound.UpdateWeaponAwardPoints(weaponStr,true,false);
							playerInRound.UpdateLocationAwardPoints(locationStr,true,false);
							break;
						case KILL_TYPE_DEATH:
							playerInRound.UpdateWeaponAwardPoints(weaponStr,false,false);
							playerInRound.UpdateLocationAwardPoints(locationStr,false,false);
							// Death
							playerInRound.deaths++;
							score+=deathScoreWeight;
							// Skills are negative as it is a death
							if (globalStatistics.AllowNegativeSkill())
							{
								weaponSkill*=(-1);
								locationSkill*=(-1);
								skill+=weaponSkill;
								skill+=locationSkill;
							}
							break;
						case KILL_TYPE_TEAMKILL:
							// Only affects player (no death for target)
							if (kill.playerIndex==playerInRound.playerIndex)
							{
								playerInRound.UpdateWeaponAwardPoints(weaponStr,false,true);
								playerInRound.UpdateLocationAwardPoints(locationStr,false,true);
								score+=teamKillScoreWeight;
								playerInRound.teamKills++;
							}
							break;
						case KILL_TYPE_TEAMSWAP:
							// Only affects player (target is same as player)
							if (kill.playerIndex==playerInRound.playerIndex)
							{
								score+=teamSwapScoreWeight;
							}
							break;
						case KILL_TYPE_SUICIDE:
							// Only affects player (target is same as player)
							if (kill.playerIndex==playerInRound.playerIndex)
							{
								score+=suicideScoreWeight;
								playerInRound.suicides++;
							}
							break;
						case KILL_TYPE_TEAMKILLVICTIM:
							break;
					}
					// End of Switch KillType
				}
				else
				{
					// This kill does not affect either player or target
				}
			}
			score=score * gametypeScoreWeight;
			skill=skill * gametypeSkillWeight;
			// Looped through every kill
			if (playerInRoundUseful)
			{
				// Only need to do the following if the player has done something
				playerInRound.score+=score;
				playerInRound.skill+=skill;
	
				// Update the score for this player
				globalStatistics.playerList.RemoveAt(playerInRound.playerIndex);
	
				player.accumulatedDuration+=	playerInRound.accumulatedDuration;
				player.kills+=					playerInRound.kills;
				player.deaths+=					playerInRound.deaths;
				player.suicides+=				playerInRound.suicides;
				player.teamKills+=				playerInRound.teamKills;
				player.teamWins+=				playerInRound.teamWins;
				player.teamLosses+=				playerInRound.teamLosses;
				player.score+=					playerInRound.score;
				player.skill+=					playerInRound.skill;
				player.lastActiveRound=			roundIndex;
	
				playerInRound.UpdateMiscAwardPoints("Team Kills",playerInRound.teamKills);
				playerInRound.UpdateMiscAwardPoints("Kills",playerInRound.kills);
				playerInRound.UpdateMiscAwardPoints("Deaths",playerInRound.deaths);
				playerInRound.UpdateMiscAwardPoints("Suicides",playerInRound.suicides);
				streakCount=playerInRound.teamKillStreak.GetCount();
				for (streakIndex=0;streakIndex<streakCount;streakIndex++)
				{
					streak=playerInRound.teamKillStreak.Item(streakIndex);
					if (streak.streakSize>player.longestTeamKillStreak)
					{
						player.longestTeamKillStreak=streak.streakSize;
					}
				}
				// Kills
				streakCount=playerInRound.killStreak.GetCount();
				for (streakIndex=0;streakIndex<streakCount;streakIndex++)
				{
					streak=playerInRound.killStreak.Item(streakIndex);
					if (streak.streakSize>player.longestKillStreak)
					{
						player.longestKillStreak=streak.streakSize;
					}
				}
				// Death Streaks
				streakCount=playerInRound.deathStreak.GetCount();
				for (streakIndex=0;streakIndex<streakCount;streakIndex++)
				{
					streak=playerInRound.deathStreak.Item(streakIndex);
					if (streak.streakSize>player.longestDeathStreak)
					{
						player.longestDeathStreak=streak.streakSize;
					}
				}
	
				globalStatistics.playerList.Insert(player,playerInRound.playerIndex);
			}
		}
		playersInRound.RemoveAt(0);
		//
		playerInRoundUseful=true;
		//
		if (playerInRoundUseful)
		{
			playersInRound.Add(playerInRound);
		}
		else
		{
			// This player did not do a thing in this round
			// no talking - no killing - no dying - no team winning / losing
			// so we are gonna delete them - they are probably just a
			// spectator
			playerCount--;
		}
		playerInRoundIndex++;
	}
	// Looped through every player
	// replace the current player with the newly modified one
}

void Round::WriteToDatabase(int roundIndex)
{
	wxString		SQL;
	int				count;
	int				subIndex;
	KillData		kill;
	ActionData		action;
	TeamWinData		teamWin;
	TeamLossData	teamLoss;
	SpeechData		speech;
	PlayerInRound	player;
	char			droppedChar;
	int				variableCount;
	int				variableIndex;
	wxString		variable;
	wxString		value;
	XPPoints		xp;
	int				xpCount;
	int				xpIndex;
	
	STATSGEN_DEBUG_FUNCTION_START("Round","WriteToDatabase")

	variableCount=logfileVariableKeys.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		variable=StatsgenDatabase::SafeForInsert(logfileVariableKeys.Item(variableIndex));
		value=StatsgenDatabase::SafeForInsert(logfileVariableValues.Item(variableIndex));
		SQL.Printf("insert into %s "
				"(roundidx,serverindex,variable,value) "
				"values ("
				"'%d',"
				"'%d',"
				"'%s',"
				"'%s'"
				")",
				SQLVariableTableName().GetData(),
				roundIndex,
				serverIndex,
				variable.GetData(),
				value.GetData());

		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Variables")
	if (dropped)
	{
		droppedChar='Y';
	}
	else
	{
		droppedChar='N';
	}
	SQL.Printf("insert into %s "
				"(roundidx,serverindex,starttime,duration,gametype,mapname,dropped) "
				"values ("
				"'%d',"
				"'%d',"
				"'%s',"
				"'%d',"
				"'%d',"
				"'%d',"
				"'%c'"
				")",
				SQLTableName().GetData(),
				roundIndex,
				serverIndex,
				roundStartTime.Format("%Y%C%m%d%H%M%S").GetData(),
				(int)roundDuration.GetSeconds().ToLong(),
				gameType,
				mapName,
				droppedChar);

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Round")
	count=kills.GetCount();
	for (subIndex=0;subIndex<count;subIndex++)
	{
		kill=kills.Item(subIndex);
		kill.WriteToDatabase(roundIndex,subIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Kills")
	count=actions.GetCount();
	for (subIndex=0;subIndex<count;subIndex++)
	{
		action=actions.Item(subIndex);
		action.WriteToDatabase(roundIndex,subIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Actions")
	count=teamWins.GetCount();
	for (subIndex=0;subIndex<count;subIndex++)
	{
		teamWin=teamWins.Item(subIndex);
		teamWin.WriteToDatabase(roundIndex,subIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Team Wins")
	count=teamLosses.GetCount();
	for (subIndex=0;subIndex<count;subIndex++)
	{
		teamLoss=teamLosses.Item(subIndex);
		teamLoss.WriteToDatabase(roundIndex,subIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Team Losses")
	count=speeches.GetCount();
	for (subIndex=0;subIndex<count;subIndex++)
	{
		speech=speeches.Item(subIndex);
		speech.WriteToDatabase(roundIndex,subIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Speech")
	count=playersInRound.GetCount();
	for (subIndex=0;subIndex<count;subIndex++)
	{
		player=playersInRound.Item(subIndex);
		player.WriteToDatabase(roundIndex,subIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written Players In Round")
	xpCount=xpPoints.GetCount();	
	for (xpIndex=0;xpIndex<xpCount;xpIndex++)
	{
		xp=xpPoints.Item(xpIndex);
		xp.WriteToDatabase(roundIndex);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,"Written XP Points")
	STATSGEN_DEBUG_FUNCTION_END
}


wxString Round::SQLTableName()
{
	wxString	retVal="rounds";

	return (retVal);
}

wxString Round::SQLVariableTableName()
{
	wxString	retVal="roundvariables";

	return (retVal);
}

wxString Round::SQLCreateVariableTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundidx integer,"
				"serverindex integer,"
				"%s,"
				"%s"
			")",
			SQLVariableTableName().GetData(),
			StatsgenDatabase::StringFieldDefinition("variable","roundvariable",FIELD_WIDTH_VARIABLE).GetData(),
			StatsgenDatabase::StringFieldDefinition("value","roundvalue",FIELD_WIDTH_VALUE).GetData()
			);

	return SQL;

}

wxString Round::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundidx integer,"
				"serverindex integer,"
				"%s,"
				"duration integer,"
				"gametype int,"
				"mapname int,"
				"dropped varchar(1)"
			")",
			SQLTableName().GetData(),
			StatsgenDatabase::StringFieldDefinition("starttime","roundstarttime",FIELD_WIDTH_ROUND_START_TIME).GetData()
			);

	return SQL;

}

void Round::AddVariable(wxString &variable,wxString &value)
{
	logfileVariableKeys.Add(variable);
	logfileVariableValues.Add(value);
}

void Round::SetStartingCondition(int serverIndexIn,
								wxString &mapNameIn,
								time_t durationIn,
								wxString &gameTypeIn)
{
	wxString	mapNameString;
	wxString	gameTypeString;

	serverType=logfileReader->GetServerType();
	kills.Clear();
	actions.Clear();
	speeches.Clear();
	playersInRound.Clear();
	teamWins.Clear();
	teamLosses.Clear();

	mapNameString=serverType+"_"+mapNameIn.Lower();
	gameTypeString=serverType+"_"+gameTypeIn.Lower();

	SubstituteLogEntry("GAMETYPE",gameTypeString);
	SubstituteLogEntry("MAP",mapNameString);

	mapName=globalStatistics.GenericKeyAdd(globalStatistics.keysMap,mapNameString);
	gameType=globalStatistics.GenericKeyAdd(globalStatistics.keysGametype,gameTypeString);
}

void Round::AddTeamWin(wxString &playerGUID,wxString &playerName,wxString &playerTeam,
			time_t mapStartTime,time_t mapEndTime)
{
	TeamWinData	playerData;
	bool		cheat;
	wxString	playerID;
	wxString	teamName=serverType+"_"+playerTeam.Lower();
	wxDateTime	playerTime;
	
	SubstituteLogEntry("TEAM",teamName);

	if (globalStatistics.dropList.IsDropped(playerName,&cheat))
	{
		if (cheat)
		{
			globalStatistics.cheaters.Add(playerName);
		}
	}
	if (!cheat)
	{
		playerID=serverType+"_"+playerGUID;
		playerData.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
		playerData.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,teamName);
		playerData.gameType=gameType;
		playerData.mapName=mapName;
		playerData.playerClass=0;
		teamWins.Add(playerData);
		playerTime.Set(mapStartTime);
		UpdatePlayerInRoundTime(playerData.playerIndex,playerTime,true);
		playerTime.Set(mapEndTime);
		UpdatePlayerInRoundTime(playerData.playerIndex,playerTime,false);
	}
}

void Round::AddTeamLoss(wxString &playerGUID,wxString &playerName,wxString &playerTeam,
			time_t mapStartTime,time_t mapEndTime)
{
	TeamLossData	playerData;
	bool			cheat;
	wxString		playerID;
	wxString		teamName=serverType+"_"+playerTeam.Lower();
	wxDateTime		playerTime;
	
	SubstituteLogEntry("TEAM",teamName);

	if (globalStatistics.dropList.IsDropped(playerName,&cheat))
	{
		if (cheat)
		{
			globalStatistics.cheaters.Add(playerName);
		}
	}
	if (!cheat)
	{
		playerID=serverType+"_"+playerGUID;
		playerData.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
		playerData.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,teamName);
		playerData.gameType=gameType;
		playerData.mapName=mapName;
		playerData.playerClass=0;
		teamLosses.Add(playerData);
		playerTime.Set(mapStartTime);
		UpdatePlayerInRoundTime(playerData.playerIndex,playerTime,true);
		playerTime.Set(mapEndTime);
		UpdatePlayerInRoundTime(playerData.playerIndex,playerTime,false);
	}
}

void Round::AddKill(DecodedKill &decode)
{
	KillData	kill;
	wxString	playerID;
	wxString	playerClass;
	wxString	playerTeam;
	wxString	playerWeapon;
	wxString	playerAmmo;
	wxString	targetID;
	wxString	targetClass;
	wxString	targetTeam;
	wxString	targetLocation;
	wxString	weaponAndAmmo;
	wxString	playerName;
	wxString	targetName;
	bool		cheat;
	wxDateTime	killTime;
	
	STATSGEN_DEBUG_FUNCTION_START("Round","AddKill")
	playerClass=serverType+"_"+decode.playerClass.Lower();
	playerWeapon=serverType+"_"+decode.playerWeapon.Lower();
	playerAmmo=serverType+"_"+decode.playerAmmo;
	targetClass=serverType+"_"+decode.targetClass.Lower();
	targetTeam=serverType+"_"+decode.targetTeam.Lower();
	targetLocation=serverType+"_"+decode.targetLocation.Lower();

	SafeString(playerClass);
	SafeString(playerWeapon);
	SafeString(playerAmmo);
	SafeString(targetClass);
	SafeString(targetTeam);
	SafeString(targetLocation);

	if (logfileReader->IsWorldPlayer(decode.playerID,decode.playerTeam,decode.playerName))
	{
		playerID=WORLD_PLAYER_ID;
		playerName=WORLD_PLAYER_NAME;
		playerTeam=WORLD_PLAYER_TEAM;
	}
	else
	{
		playerID=serverType+"_"+decode.playerID;
		playerName=decode.playerName;
		playerTeam=serverType+"_"+decode.playerTeam.Lower();
		cheat=false;
		if (globalStatistics.dropList.IsDropped(playerName,&cheat))
		{
			if (cheat)
			{
				// Cheats get no kills
				globalStatistics.cheaters.Add(playerName);
				return;
			}
		}
	}

	if (logfileReader->IsWorldPlayer(decode.targetID,decode.targetTeam,decode.targetName))
	{
		targetID=WORLD_PLAYER_ID;
		targetName=WORLD_PLAYER_NAME;
		targetTeam=WORLD_PLAYER_TEAM;
	}
	else
	{
			cheat=false;
			globalStatistics.dropList.IsDropped(decode.targetName,&cheat);
			if (cheat)
			{
				// If the target is a cheat - make it appear as the 
				// world player - then it will be counted as a kill
				// by the player but will get dropped along with the
				// other stats
				globalStatistics.cheaters.Add(decode.targetName);
				targetID=WORLD_PLAYER_ID;
				targetName=WORLD_PLAYER_NAME;
				targetTeam=WORLD_PLAYER_TEAM;
			}
			else
			{
				targetID=serverType+"_"+decode.targetID;
				targetTeam=serverType+"_"+decode.targetTeam.Lower();
				targetName=decode.targetName;
			}
	}

	weaponAndAmmo=playerWeapon+playerAmmo;
	SubstituteLogEntry("CLASS",playerClass);
	SubstituteLogEntry("TEAM",playerTeam);
	SubstituteLogEntry("WEAPON",playerWeapon);

	if (SubstituteLogEntry("AMMO",playerAmmo))
	{
		// we made an ammo substitution
		playerWeapon=playerAmmo;
	}
	if (SubstituteLogEntry("WEAPONAMMO",weaponAndAmmo))
	{
		// we made an ammo substitution
		playerWeapon=weaponAndAmmo;
	}


	SubstituteLogEntry("CLASS",targetClass);
	SubstituteLogEntry("TEAM",targetTeam);
	SubstituteLogEntry("LOCATION",targetLocation);

	StoreLogEntry("WEAPON",playerWeapon);
	StoreLogEntry("LOCATION",targetLocation);

	kill.gameType=gameType;
	kill.mapName=mapName;
	kill.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
	kill.playerClass=globalStatistics.GenericKeyAdd(globalStatistics.keysClass,playerClass);
	kill.playerTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,playerTeam);
	kill.playerWeapon=globalStatistics.GenericKeyAdd(globalStatistics.keysWeapon,playerWeapon);
	kill.playerAmmo=globalStatistics.GenericKeyAdd(globalStatistics.keysAmmo,playerAmmo);
	kill.targetIndex=globalStatistics.AddPlayer(targetName,targetID);
	kill.targetClass=globalStatistics.GenericKeyAdd(globalStatistics.keysClass,targetClass);
	kill.targetTeam=globalStatistics.GenericKeyAdd(globalStatistics.keysTeam,targetTeam);
	kill.targetDamage=0;
	kill.targetLocation=globalStatistics.GenericKeyAdd(globalStatistics.keysLocation,targetLocation);
	kills.Add(kill);
	UpdatePlayerInRoundTime(kill.playerIndex,killTime,false);
	UpdatePlayerInRoundTime(kill.targetIndex,killTime,false);
}

void Round::AddXPPoints(wxString &playerName,
						wxString &playerGUID,
						wxString &xpKey,
						float points)
{
	XPPoints	xp;
	XPPoints	xpFound;
	wxString	xpID;
	wxString	playerID;
	int			xpIndex;
	int			xpCount;
	bool		cheat;

	xpID=serverType+"_"+xpKey.Lower();
	SafeString(xpID);
	playerID=serverType+"_"+playerGUID;
	xp.pointsIndex=globalStatistics.GenericKeyAdd(globalStatistics.keysXP,xpID);
	xp.points=points;

	if (globalStatistics.dropList.IsDropped(playerName,&cheat))
	{
		if (cheat)
		{
			// Cheats get no kills
			globalStatistics.cheaters.Add(playerName);
			return;
		}
	}
	xp.playerIndex=globalStatistics.AddPlayer(playerName,playerID);
	// First we have to remove any existing entry and replace it with
	// this one
	xpCount=xpPoints.GetCount();
	for (xpIndex=0;xpIndex<xpCount;xpIndex++)
	{
		xpFound=xpPoints.Item(xpIndex);
		if ((xpFound.playerIndex==xp.playerIndex) &&
			(xpFound.pointsIndex==xp.pointsIndex))
		{
			xpPoints.RemoveAt(xpIndex);
			break;
		}
	}
	xpPoints.Add(xp);
	StoreLogEntry("XP",xpID);
}


#ifndef __ROUND
#define __ROUND

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen Includes
#include "ErrorData.h"
#include "KillData.h"
#include "ActionData.h"
#include "SpeechData.h"
#include "TeamWinData.h"
#include "TeamLossData.h"
#include "PlayerInRound.h"
#include "XPPoints.h"

class LogFileReader;

class DecodedTeamLoss
{
	public:
		void Clear();
		wxString		teamName;
		wxArrayString	playerIDs;
		wxArrayString	playerRoundIDs;
		wxArrayString	playerNames;
};

class DecodedTeamWin
{
	public:
		void Clear();
		wxString		teamName;
		wxArrayString	playerIDs;
		wxArrayString	playerRoundIDs;
		wxArrayString	playerNames;
};

class DecodedKill
{
	public:
		wxString	playerID;
		wxString	playerRoundID;
		wxString	playerName;
		wxString	playerClass;
		wxString	playerTeam;
		wxString	playerWeapon;
		wxString	playerAmmo;
		wxString	targetID;
		wxString	targetRoundID;
		wxString	targetName;
		wxString	targetClass;
		wxString	targetTeam;
		wxString	targetDamage;
		wxString	targetLocation;
};
class DecodedPlayerJoin
{
	public:
		wxString	playerID;
		wxString	playerRoundID;
		wxString	playerName;
		wxString	playerClass;
		wxString	playerTeam;
};

class DecodedPlayerQuit
{
	public:
		wxString	playerID;
		wxString	playerRoundID;
		wxString	playerName;
};

class DecodedAction
{
	public:
		wxString	playerID;
		wxString	playerRoundID;
		wxString	playerTeam;
		wxString	playerClass;
		wxString	playerName;
		wxString	action;
};

class DecodedSpeech
{
	public:
		wxString	playerID;
		wxString	playerRoundID;
		wxString	playerName;
		wxString	speech;
};

class DecodedRoundStart
{
	public:
		wxString	admin;
		wxString	location;
		wxString	website;
		wxString	gameType;
		wxString	mapName;
		wxString	maxClients;
		wxString	privateClients;
};

class RoundLine
{
	public:
		void Set(wxDateTime &lineTimeIn,wxString &lineIn);
		wxDateTime	lineTime;
		wxString	line;
};

WX_DECLARE_OBJARRAY(RoundLine,ArrayOfRoundLines);

class Round;

WX_DECLARE_OBJARRAY(Round,ArrayOfRounds);
class Round
{
	public:
		Round();
		virtual ~Round();
		void Debug();
		void Initiate(LogFileReader *reader,
						wxDateTime &lineTime,
						wxString &line);
		void AddLogLine(wxDateTime &lineTime,wxString &line);
		void Process(ErrorData &errorData,int serverIndexIn);
		bool IsDropped();
		void CalculateScores(int roundIndex);
		void WriteToDatabase(int index);
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		static wxString SQLCreateVariableTable();
		static wxString SQLVariableTableName();
		void AddVariable(wxString &variable,wxString &value);
		void SetStartingCondition(int serverIndexIn,
					wxString &mapNameIn,
					time_t durationIn,
					wxString &gameTypeIn);
		void AddTeamLoss(wxString &guid, 
						wxString &playerName, 
						wxString &playerTeam,
						time_t mapStartTime,
						time_t mapEndTime);
		void AddTeamWin(wxString &guid, 
						wxString &playerName, 
						wxString &playerTeam,
						time_t mapStartTime,
						time_t mapEndTime);
		void AddKill(DecodedKill &decoded);
		void AddSpeech(DecodedSpeech &decoded,
					wxDateTime &speechTime);
		void AddXPPoints(wxString &playerName,
						wxString &playerGUID,
						wxString &xpKey,
						float points);

	public:
		int						serverIndex;
		ArrayOfKillData			kills;
		ArrayOfActionData		actions;
		ArrayOfTeamWinData		teamWins;
		ArrayOfTeamLossData		teamLosses;
		ArrayOfSpeechData		speeches;
		ArrayOfPlayersInRound	playersInRound;
		ArrayOfXPPoints			xpPoints;
		int						activePlayers;
		
		wxArrayString	logfileVariableKeys;
		wxArrayString	logfileVariableValues;
		bool					dropped;
		wxTimeSpan			roundDuration;

	private:
		void AddKill(DecodedKill &decoded,
					wxDateTime &killTime,
					int gameType,
					int mapName);
		void AddTeamLoss(DecodedTeamLoss &decoded,
					wxDateTime &lossTime,
					int gameType,
					int mapName);
		void AddTeamWin(DecodedTeamWin &decoded,
					wxDateTime &winTime,
					int gameType,
					int mapName);
		void AddAction(DecodedAction &decoded,
					wxDateTime &actionTime,
					int gameType,
					int mapName);
		void AddJoin(DecodedPlayerJoin &decoded,
					wxDateTime &joinTime);
		void AddQuit(DecodedPlayerQuit &decoded,
					wxDateTime &quitTime);

		void UpdatePlayerInRoundTime(int playerIndex, wxDateTime &timeStamp,bool resetTime);
		bool SubstituteLogEntry(const char *logEntryType,
								wxString &logEntry);
		void StoreLogEntry(const char *logEntryType,
								wxString &logEntry);

	private:
		bool				WordAtTextPosition(
									wxString &speechLine,
									int speechIndex,
									int speechLength,
									wxString &bannedWord,
									int *replacementLength);

		wxDateTime			roundStartTime;
		int					gameType;
		int					mapName;
		LogFileReader		*logfileReader;
		ArrayOfRoundLines	roundLines;
		wxString			serverType;

		bool				teamWinAlreadyFound;
		bool				teamLossAlreadyFound;
		
};

#endif

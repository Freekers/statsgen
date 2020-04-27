#ifndef __GLOBALSTATISTICS
#define __GLOBALSTATISTICS

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/filename.h>

// statsgen includes
#include "Cheater.h"
#include "Player.h"
#include "Round.h"
#include "ConfigData.h"
#include "StatsgenDatabase.h"
#include "GenericKey.h"
#include "PlayerCacheEntry.h"
#include "AKAListEntry.h"
#include "AliasListEntry.h"
#include "AwardDefinition.h"
#include "Server.h"
#include "Clan.h"
#include "DropList.h"
#include "ScheduleItem.h"
#include "KillTotalLists.h"
#include "PlayerDataList.h"
#include "WeightList.h"
#include "TemplateFile.h"
#include "PlayerList.h"
#include "AliasCacheEntry.h"
#include "XPPoints.h"
#include "XPTotals.h"
#include "WebServer.h"
#include "TemplateFile.h"

#define CONFIG_KEY_SPEECHFILE	"/General/Speechfile"

#define STEP_APPLY_ALIASES						"ApplyAliases"
#define STEP_DATABASE_READ_STATIC				"DatabaseReadStatic"
#define STEP_DATABASE_LOCATE_EXISTING_PLAYERS	"DatabaseLocateExistingPlayers"
#define STEP_DOWNLOAD_LOGS						"DownloadLogFile"
#define STEP_CREATE_LOG_FILES					"CreateLogfileObjects"
#define STEP_PROCESS_LOG_FILES					"ProcessLogfiles"
#define STEP_DROP_PLAYERS						"DropPlayersAndRounds"
#define STEP_CALCULATE_SCORES					"CalculateScores"
#define STEP_CREATE_DATABASE					"CreateDatabase"
#define STEP_EMPTY_DATABASE						"EmptyDatabase"
#define STEP_WRITE_DATABASE						"WriteDatabase"
#define STEP_TRANSFER_DATABASE					"TransferDatabase"
#define STEP_READ_TEMPLATE						"ReadTemplate"
#define STEP_PROCESS_TEMPLATE					"ProcessTemplate"
#define STEP_POST_PROCESS_TEMPLATE				"PostProcessTemplate"
#define STEP_WEBSITE							"Website"

class TemplateFile;

class GlobalStatistics
{
	public:
		GlobalStatistics();
		~GlobalStatistics();
		void CalculateScores();
		void InitiateSkillCalculationType();
		wxString SkillCalculationTypeConfigKey();
		bool AllowNegativeSkill();
		bool ProcessStepAllowed(int step,wxArrayInt &allowedSteps);
		void ProduceStatsInitiate();
		void ProduceStatsDownload();
		void ProduceStatsProcess();
		void ProduceStatsTransfer();
		void ProduceStatsFinalise();
		void ProduceStats(
				bool fullRun,
				wxArrayInt &allowedSteps,
				TemplateFile &templateFile);
		void DropPlayersAndRounds();
		void DropPlayers();
		void DropRounds();
		void DropInactiveRounds();
		int FindPlayerIndex(wxString &name);
		int FindGUIDIndex(wxString &guid);
		int AddPlayer(wxString &name,wxString &id);
		Player GetPlayer(int index);
		int GenericKeyIndex(ArrayOfGenericKey &keyArray,
							wxString &id);
		wxString GenericKeyID(ArrayOfGenericKey &keyArray,
							int index);
		int GenericKeyAdd(ArrayOfGenericKey &keyArray,
							wxString &id);
		void Debug();
		bool WriteToDatabase();
		void GenericKeyDebug(
					ArrayOfGenericKey &arrayKey,
					char *tableName);
		void ReadPlayerCache();
		void WritePlayerCache();
		void PlayerListToPlayerCache();
		void ReadAliasList();
		void WriteAliasList();
		void AddAlias(AliasListEntry &aliasEntry);
		void DeleteAlias(AliasListEntry &aliasEntry);
		bool FindAlias(AliasListEntry &aliasEntry);
		void ReadAwardDefinitions();
		void ReadClanDefinitions();
		void ReadServersFromConfig();
		void ClearServers();
		int ClanIndex(wxString &name);
		void ReadScheduleConfig();
		void ReadMessageTimerConfig();
		void ReadProductionRunTimerConfig();
		void ReadQuickRunTimerConfig();

		bool StepAllowed(const char *step);
		void ResetProcessingStatus();

		void InitiateMessageCentres();
		void SendMessages();
		void ReadDeathmatchGametypes();
		void ReadTeamSwapWeapons();
		bool IsDeathmatchGametype(wxString &gametype);
		bool IsDeathmatchGametype(int gametype);
		bool IsTeamSwapWeapon(int weapon);
		bool IsTeamSwapWeapon(wxString &weapon);
		void OpenSpeechFile();
		void CloseSpeechFile();
		void WriteSpeechFile(wxDateTime &speechTime,wxString &guid,wxString &playerName,wxString &speech);
		void GenerateUpgrade(wxString &filename);
		void AddAKA(int playerIndex,wxString &name);
		float ScoreWeight(const char *key,float defaultValue);
		float ScoreWeight(const char *key,const char *prefix,float defaultValue);
		float ScoreWeight(wxString &key,float defaultValue);
		float GametypeScoreWeight(wxString &gametype);
		float GametypeScoreWeight(int gametype);
		float GametypeSkillWeight(wxString &gametype);
		float GametypeSkillWeight(int gametype);
		float SkillWeight(const char *key,float defaultValue);
		float SkillWeight(const char *key,const char *prefix,float defaultValue);
		float SkillWeight(wxString &key,float defaultValue);
		int ReadPriority(bool fullRun);
		void WritePriority(bool fullRun,int priority);
		wxString AutomaticAliasConfigKey();
		void EmptyAliasCache();
		void AddToAliasCache(ArrayOfAliasCacheEntry *cache,
							wxString &name,
							wxString &guid,
							int index);
		void ReadStaticItemsFromDatabase();
		void ReadGenericKeyFromDatabase(ArrayOfGenericKey &keyList,
										const char *tableName);
		void LocateExistingPlayers();
		void DropExistingPlayers();
		void UpdateExistingPlayersDropStatus(wxArrayInt &playerIndexList,bool dropped);
		void UpdateExistingPlayersClan();
		void DropExistingCheat(int playerIndex,int worldPlayerIndex);
		void FlagStartOfRun();
		void FlagEndOfRun();
		bool AreStatsRunning();
		
	public:
		TemplateFile				templateFile;
		time_t					startTime;
		PlayerDataList				*playerDataAvatars;
		PlayerDataList				*playerDataWebsite;
		PlayerDataList				*playerDataPicture;
		PlayerDataList				*playerDataXfire;
		PlayerDataList				*playerDataClanRole;
		PlayerDataList				*playerDataRealName;
		PlayerDataList				*playerDataMisc1;
		PlayerDataList				*playerDataMisc2;
		ArrayOfPlayerCacheEntry		playerCache;
		//ArrayOfServer				servers;
		ArrayOfPointers				servers;
		ArrayOfAliasListEntry		aliasList;
		ArrayOfAKAListEntry			AKAList;
		DropList					dropList;
		//ArrayOfPlayers				players;
		PlayerList					playerList;
		ArrayOfRounds				rounds;
		ArrayOfAwardDefinition		awardDefinitions;
		ArrayOfClan					clans;
		ConfigData					configData;
		StatsgenDatabase			statsgenDatabase;

		ArrayOfGenericKey			keysAction;
		ArrayOfGenericKey			keysAmmo;
		ArrayOfGenericKey			keysMap;
		ArrayOfGenericKey			keysWeapon;
		ArrayOfGenericKey			keysClass;
		ArrayOfGenericKey			keysTeam;
		ArrayOfGenericKey			keysGametype;
		ArrayOfGenericKey			keysLocation;
		ArrayOfGenericKey			keysXP;
		wxArrayInt					playersChanged;
		wxArrayInt					droppedPlayersChanged;
		wxArrayInt					roundsChanged;

		KillTotalLists				killTotalLists;
		XPTotals					totalXP;

		int							roundsAlreadyProcessed;

		ScheduleItem				messageTimer;
		ScheduleItem				productionRunTimer;
		ScheduleItem				quickRunTimer;
		wxArrayString				deathmatchGametypes;
		wxArrayString				teamSwapWeapons;
		wxArrayString				bannedSpeech;

		FILE *speechfp;

		bool negativeSkillAllowed;
		WeightList					skillWeights;
		WeightList					scoreWeights;
		wxThread					*currentRunningThread;			
		wxEvtHandler				*mainEventHandler;

		bool						useAutomaticAliases;
		bool						aliasCacheEnabled;
		ArrayOfAliasCacheEntry		*aliasCache;
		ArrayOfAliasCacheEntry		*aliasPrimaryCache;
		Cheaters					cheaters;

		WebServer					webServer;

		bool						hiddenFeatures;

		bool						recoveryMode;

		bool						statsRunning;
};

extern GlobalStatistics	globalStatistics;
#endif

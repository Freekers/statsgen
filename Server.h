#ifndef __SERVER
#define __SERVER

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes
#include "LogFile.h"
#include "MessageCentre.h"

class Server;
class LogFile;

#define SERVER_TYPE_COD1		"COD1"
#define SERVER_TYPE_COD2		"COD2"
#define SERVER_TYPE_COD4		"COD4"
#define SERVER_TYPE_COD5		"COD5"
#define SERVER_TYPE_MOHAA		"MOHAA"
#define SERVER_TYPE_QUAKEWARS		"QUAKEWARS"
#define SERVER_TYPE_WOLFENSTEIN		"WOLFENSTEIN"

#define SERVER_TYPE_NAME_COD1		"Call Of Duty 1"
#define SERVER_TYPE_NAME_COD2		"Call Of Duty 2"
#define SERVER_TYPE_NAME_COD4		"Call Of Duty 4"
#define SERVER_TYPE_NAME_COD5		"Call Of Duty 5"
#define SERVER_TYPE_NAME_MOHAA		"Medal Of Honour"
#define SERVER_TYPE_NAME_QUAKEWARS	"Enemy Territory: Quake Wars"
#define SERVER_TYPE_NAME_WOLFENSTEIN	"RTCW / Wolfenstein ET"

#define SERVER_TYPE_COUNT		7

#define STAT_TYPE_CODE_ACTION		"action"
#define STAT_TYPE_CODE_LOCATION		"location"
#define STAT_TYPE_CODE_WEAPON		"weapon"
#define STAT_TYPE_CODE_MAP		"map"
#define STAT_TYPE_CODE_TEAM		"team"
#define STAT_TYPE_CODE_CLASS		"class"
#define STAT_TYPE_CODE_GAMETYPE		"gametype"

#define STAT_TYPE_NAME_ACTION		"Action"
#define STAT_TYPE_NAME_LOCATION		"Location"
#define STAT_TYPE_NAME_WEAPON		"Weapon"
#define STAT_TYPE_NAME_MAP		"Map"
#define STAT_TYPE_NAME_TEAM		"Team"
#define STAT_TYPE_NAME_CLASS		"Class"
#define STAT_TYPE_NAME_GAMETYPE		"Game Type"

#define SKILL_TYPE_COUNT		3
#define IMAGE_TYPE_COUNT		7

extern char *ImageTypeCodes[];
extern char *ImageTypeNames[];
extern char *SkillTypeCodes[];
extern char *SkillTypeNames[];
extern char *ServerTypeCodes[];
extern char *ServerTypeNames[];

#define SERVER_BAN_TYPE_PB	"PUNKBUSTER"
#define SERVER_BAN_TYPE_COD	"COD"

WX_DECLARE_OBJARRAY(Server,ArrayOfServer);
class Server
{
	public:
		Server();
		virtual ~Server();

		void SetID(wxString &IDIn);
		bool Initiate();
		bool FindLogfiles();
		bool CreateLogFiles();
		bool ProcessLogFiles(int serverIndex);
		bool ProcessBanFiles();
		bool DownloadFiles();
		bool DownloadBanFiles();
		void StartFromBeginning();
		void InitiateMessageCentre();
		void SendMessage();
		void AddVariable(wxString &variable,wxString &value);
		void WriteToDatabase(int serverIndex);
		void AdjustLogfilePosition();
		static wxString SQLCreateTable();
		static wxString SQLCreateVariableTable();
		static wxString SQLCreateProgressTable();
		static wxString SQLTableName();
		static wxString SQLVariableTableName();
		static wxString SQLProgressTableName();
		wxString WildCardFormatToWildCard(wxString &wildCardFormat);
		void SortFiles(wxArrayString &filenameList,wxString &wildCard);
		static int CompareFilenames(const wxString &first,const wxString &second);
		static int CompareInts(int *first,int *second);
		long LogfileSize();

	public:
		bool		restartDownload;
		bool		fakeTeamIDs;
		wxString	ID;
		wxArrayString	logFilenames;
		wxArrayString	secondaryLogFilenames;
		//ArrayOfLogFile	logFiles;
		LogFile			*logFiles;

		MessageCentre	messageCentre;


		wxString	archiveDirectory;
		wxString	archiveWildcard;
		wxString	latestDirectory;
		wxString	latestFilename;

		wxString	secondaryArchiveDirectory;
		wxString	secondaryArchiveWildcard;
		wxString	secondaryLatestDirectory;
		wxString	secondaryLatestFilename;
		wxString	serverType;

		int		gamePort;
		wxString	gameIP;

		bool		FTPEnabled;
		wxString	FTPArchiveDirectory;
		wxString	FTPArchiveWildcard;
		wxString	FTPLatestDirectory;
		wxString	FTPLatestFilename;
		wxString	FTPSecondaryArchiveDirectory;
		wxString	FTPSecondaryArchiveWildcard;
		wxString	FTPSecondaryLatestDirectory;
		wxString	FTPSecondaryLatestFilename;

		wxString	FTPBanFile1Directory;
		wxString	FTPBanFile1Filename;
		wxString	banFile1Type;
		wxString	ban1Directory;

		wxString	FTPBanFile2Directory;
		wxString	FTPBanFile2Filename;
		wxString	banFile2Type;
		wxString	ban2Directory;

		wxArrayString	logfileVariableKeys;
		wxArrayString	logfileVariableValues;
		long		logfilesSize;
		long		maxLogfileSize;
		
};


#endif

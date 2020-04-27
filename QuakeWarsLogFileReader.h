#ifndef __QUAKEWARSLOGFILEREADER
#define __QUAKEWARSLOGFILEREADER

#define QUAKEWARS_LOGLINEPREFIX_MAPSTARTED		"Map Started: Real Time: "
#define QUAKEWARS_LOGLINEPREFIX_SERVER			"Server: "
#define QUAKEWARS_LOGLINEPREFIX_MAP				"Map: "
#define QUAKEWARS_LOGLINEPREFIX_TIMELIMIT		"Timelimit: "
#define QUAKEWARS_LOGLINEPREFIX_RULESET			"Ruleset: "
#define QUAKEWARS_LOGLINEPREFIX_TEAM			"Team: "
#define QUAKEWARS_LOGLINEPREFIX_PLAYER			"Player: "
#define QUAKEWARS_LOGLINEPREFIX_MAPFINISHED		"Map Finished: Real Time: "
#define QUAKEWARS_LOGLINEPREFIX_WINNINGTEAM		"Winning Team: "
#define QUAKEWARS_LOGLINEPREFIX_ALLSTATSCLEARED	"All Stats Cleared"
#define QUAKEWARS_LOGLINE_PLAYER_SUBSTITUTE		"PLAYER"
#define QUAKEWARS_LOGLINEPREFIX_PLAYER_JOIN		"PLAYER joined the "
#define QUAKEWARS_LOGLINEPREFIX_PLAYER_JOIN2	" joined the "
#define QUAKEWARS_LOGLINEPREFIX_PLAYER_SPEECH	"PLAYER: "
#define QUAKEWARS_LOGLINEPREFIX_PLAYER_KILL		"PLAYER ["
#define QUAKEWARS_LOGLINESUFFIX_PLAYER_KILL		"] PLAYER"
#define QUAKEWARS_LOGLINEPREFIX_PLAYER_PBGUID	"PunkBuster Server: New Connection (slot #"
#define QUAKEWARS_LOGLINEPREFIX_PLAYER_XP		"XP: "
// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "ErrorData.h"
#include "LogFileReader.h"
#include "Round.h"
#include "MultiFile.h"

class Server;
	
class QuakeWarsLogFileReader : public LogFileReader
{
	public:
		QuakeWarsLogFileReader(wxArrayString &filenameIn,
								wxArrayString &secondaryFilenameIn,
								wxString &serverTypeIn);
		virtual ~QuakeWarsLogFileReader();
		virtual void ProcessLogFile(Server *server,int serverIndex);
		virtual void StartFromBeginning();
		virtual bool IsWorldPlayer(wxString &guid,wxString &team,wxString &playerName);

	protected:
		wxString PlayerGUID(wxString &playerName,wxArrayString &pbGUIDs,wxArrayString &pbNames);
		bool DecodeRound(MultiFile &logfileBuffer,
						MultiFile &secondaryLogfileBuffer,
						int serverIndex,
						wxArrayString &pbGUIDs,
						wxArrayString &pbNames);
		time_t DecodeMapStartTime(wxString &lineRead,
								const char *prefix);
		time_t DecodeTime(wxString &timeString);

		time_t StripTimeFromLine(wxString &lineRead);
		void TrimQuotes(wxString &line);
		void SortPlayersByLength(
					wxArrayString &playerNames,
					wxArrayString &playerTeams,
					wxArrayString &playerClasses);
		wxString SubstitutePlayerNamesInLine(
						wxString &lineRead,
						wxArrayString &playerNames);

		
	private:
		wxArrayString	secondaryFilenames;

};

#endif

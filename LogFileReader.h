#ifndef __LOGFILEREADER
#define __LOGFILEREADER

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "ErrorData.h"
#include "Round.h"
#include "MultiFile.h"

#define WORLD_PLAYER_NAME	"STATSGENWORLDPLAYER"
#define WORLD_PLAYER_ID		"STATSGENWORLDID"
#define WORLD_PLAYER_TEAM	"STATSGENWORLDTEAM"
#define WORLD_PLAYER_CLASS	"STATSGENWORLDCLASS"

void StatsgenDebug(char *message);
void StatsgenDebug(wxString &message);
bool EndsWith(wxString &source,const char *match);
wxString GetLineToken(wxString &line,
						char *sep,
						int tokenNumber,
						int *tokenCount);

enum LogFileLineTypes
{
	LOG_LINE_TYPE_NOTHING=0,
	LOG_LINE_TYPE_START_ROUND,
	LOG_LINE_TYPE_END_ROUND,
	LOG_LINE_TYPE_JOIN_GAME,
	LOG_LINE_TYPE_LEAVE_GAME,
	LOG_LINE_TYPE_ACTION,
	LOG_LINE_TYPE_KILL,
	LOG_LINE_TYPE_DAMAGE,
	LOG_LINE_TYPE_SPEECH,
	LOG_LINE_TYPE_TEAM_WIN,
	LOG_LINE_TYPE_TEAM_LOSS
};

class Server;
	
class LogFileReader
{
	public:
		void FakeTeamIDs(bool value);
		virtual bool IsWorldPlayer(wxString &id,wxString &team,wxString &name);
		LogFileReader(wxArrayString &filenameIn,wxString &serverTypeIn);
		virtual ~LogFileReader();
		virtual void ProcessLogFile(Server *server,int serverIndex,long maxLogfileSize);
		virtual bool DecodeRoundStart(wxString &line,DecodedRoundStart &decode,Round *round);
		virtual bool DecodePlayerJoin(wxString &line,DecodedPlayerJoin &decode);
		virtual bool DecodePlayerQuit(wxString &line,DecodedPlayerQuit &decode);
		virtual bool DecodeKill(wxString &line,DecodedKill &decode);
		virtual bool DecodeTeamWin(wxString &line,DecodedTeamWin &decode);
		virtual bool DecodeTeamLoss(wxString &line,DecodedTeamLoss &decode);
		virtual bool DecodeAction(wxString &line,DecodedAction &decode);
		virtual bool DecodeSpeech(wxString &line,DecodedSpeech &decode);
		wxString GetServerType();
		void StartFromBeginning();
		void SetLastRoundPositions(long lastRoundEndedAt,
								long secondaryLastRoundEndedAt);	
		void RetrieveLastRoundPositions(long *lastRoundEndedAt,
								long *secondaryLastRoundEndedAt);	

	protected:
		wxString FindPlayerRoundInfo2(wxString &name,
									wxArrayString &listnames,
									wxArrayString &listIDs,
									char *defaultValue);
		void UpdatePlayerRoundInfo2(wxString &name,
									wxArrayString &listnames,
									wxArrayString &listIDs,
									wxString &id);
		wxArrayString playerIDs;
		wxArrayString playerTeamList;
		bool DecodeInitGame(wxString &line,DecodedRoundStart &decode,Round *round);
		virtual bool StripTimeFromLine(wxString &lineRead,wxDateTime &lineTime);
		virtual bool IsRoundStart(wxString &line);
		virtual bool IsRoundEnd(wxString &line);

		Round currentRound;
		
		wxString	serverType;
		Server		*thisServer;
		bool		fakeTeamIDs;

	protected:
		wxString ReadLine(MultiFile &logfile,
						ErrorData &errorData);

		wxArrayString	filenames;
		off_t			lastRoundEndedAt;
		off_t			secondaryLastRoundEndedAt;

};

#endif

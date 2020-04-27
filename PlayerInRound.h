#ifndef __PLAYERINROUND
#define __PLAYERINROUND

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>
#include <wx/datetime.h>

// statsgen includes
#include "StreakData.h"
#include "KillData.h"
#define PLAYERINROUND_AWARD_ZERO	-999999

class PlayerInRound;
WX_DECLARE_OBJARRAY(PlayerInRound,ArrayOfPlayersInRound);
class PlayerInRound
{
	public:
		PlayerInRound();
		virtual ~PlayerInRound();
		void LogTime(wxDateTime &timeStamp,bool resetTime);
		void CalculateStreaks(ArrayOfKillData *kills,
							int gameType);
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		static wxString SQLCreateTableAward();
		static wxString SQLTableNameAward();
		bool WriteToDatabase(int roundIndex,int itemIndex);
		void UpdateActionAwardPoints(wxString &actionCode);
		void UpdateWeaponAwardPoints(wxString &code,bool kill,bool teamkill);
		void UpdateLocationAwardPoints(wxString &code,bool kill,bool teamkill);
		void UpdateMiscAwardPoints(char *code,int count);
		void UpdateXPAwardPoints(wxString &code,float points);

		void Debug();
		void ZeroAwardPoints();

		int	playerIndex;

		wxTimeSpan			accumulatedDuration;
		float				score;
		float				skill;
		int					kills;
		int					deaths;
		int					teamKills;
		int					teamWins;
		int					teamLosses;
		int					suicides;
		ArrayOfStreakData	teamKillStreak;
		ArrayOfStreakData	killStreak;
		ArrayOfStreakData	deathStreak;
		wxArrayInt			awardPoints;


		wxDateTime		lastActivityTime;
		bool			timeLogged;
};

#endif

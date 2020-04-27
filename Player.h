#ifndef __PLAYER
#define __PLAYER

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

class Player;

WX_DECLARE_OBJARRAY(Player,ArrayOfPlayers);
class Player
{
	public:
		Player();
		virtual ~Player();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int itemIndex);
		bool WriteToDatabaseUpdate(int itemIndex,bool update);
		bool UpdateFromDatabase(int newPlayerIndex);
		void Add(Player &secondary);
		void ZeroScore();

		wxString			name;
		wxString			ID;
		float				score;
		float				skill;
		float				scoreWeighted;
		float				skillWeighted;
		int					kills;
		int					deaths;
		int					teamKills;
		int					suicides;
		int					teamWins;
		int					teamLosses;
		int					longestKillStreak;
		int					longestTeamKillStreak;
		int					longestDeathStreak;
		int					rounds;
		wxTimeSpan			accumulatedDuration;
		bool				dropped;
		int					lastActiveRound;
		int					clanIndex;
		int					lastRoundPlayed;
		int					firstRoundPlayed;
		int					actualPlayerIndex;
};

#endif

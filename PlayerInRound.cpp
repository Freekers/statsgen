// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/datetime.h>

// Statsgen includes
#include "PlayerInRound.h"
#include "GlobalStatistics.h"
#include "Player.h"
#include "ErrorData.h"

PlayerInRound::PlayerInRound()
{
	timeLogged=false;
	teamKills=0;
	teamWins=0;
	teamLosses=0;
	suicides=0;
	kills=0;
	deaths=0;
	score=0.0;
	skill=0.0;
}

PlayerInRound::~PlayerInRound()
{
}

void PlayerInRound::LogTime(wxDateTime &timeStamp,bool resetTime)
{
	wxTimeSpan	activityDuration;

	if (!timeLogged || resetTime)
	{
		lastActivityTime=timeStamp;
	}

	activityDuration=timeStamp.Subtract(lastActivityTime);
	if (activityDuration<0)
	{
		// logfile has logged a negative time
		activityDuration=0;
	}
	if (activityDuration>3600)
	{
		// something wrong here - too long between activities
		activityDuration=0;
	}
	accumulatedDuration+=activityDuration;
	lastActivityTime=timeStamp;
	timeLogged=true;
}

void PlayerInRound::CalculateStreaks(ArrayOfKillData *kills,
									int gameType)
{
	KillData	kill;
	int			killIndex;
	int			killCount;
	StreakData	streak;
	KillTypes	lastKillType=KILL_TYPE_KILL;
	KillTypes	thisKillType=KILL_TYPE_KILL;
	bool		killDataFound=false;

	killCount=kills->GetCount();

	streak.streakSize=0;
	streak.streakEndedPlayerIndex=-1;
	for (killIndex=0;killIndex<killCount;killIndex++)
	{
		kill=kills->Item(killIndex);
		if ((kill.playerIndex==playerIndex) || 
			(kill.targetIndex==playerIndex))
		{
			// This kill/death belongs to this player
			thisKillType=kill.KillType(gameType,playerIndex);
			if ((!killDataFound)||(thisKillType!=lastKillType))
			{
				// This is a new streak type
				if (killDataFound)
				{
					// and it is not the first
					if (streak.streakSize>1)
					{
						// this is a significant streak
						// what sort is it, who ended it?
						switch (lastKillType)
						{
							case KILL_TYPE_KILL:
								// killer ended the kill streak
								streak.streakEndedPlayerIndex=kill.playerIndex;
								killStreak.Add(streak);
								break;
							case KILL_TYPE_DEATH:
								// target ended the death streak
								streak.streakEndedPlayerIndex=kill.targetIndex;
								deathStreak.Add(streak);
								break;
							case KILL_TYPE_TEAMKILL:
								// target ended the teamkill streak
								streak.streakEndedPlayerIndex=kill.targetIndex;
								teamKillStreak.Add(streak);
								break;
							default:
								// not interested in other streak types
								break;
						}
					}
					else
					{
						// This streak is insignificant
					}

					// Lets initiate the current streak
					streak.streakSize=1;
					streak.streakEndedPlayerIndex=-1;

					
				}
				else
				{
					// This is the first item of the streak
					streak.streakSize=1;
					// Currently nobody has ended the streak
					streak.streakEndedPlayerIndex=-1;
				}
			}
			else
			{
				// This is the same as the last streak type (or the first one)
				if (killDataFound)
				{
					// This is a continuation of the current streak
					streak.streakSize++;
					// Currently nobody has ended the streak
					streak.streakEndedPlayerIndex=-1;
					
				}
			}
			lastKillType=thisKillType;
			killDataFound=true;
		}
		else
		{
			// This kill data has nothing to do with this player
		}
	}

	// If the current streak has >0 entries then the round ended with a
	// streak active - need to see if it is worth adding.
	if (streak.streakSize>1)
	{
		// this is a significant streak
		// what sort is it, who ended it?
		// nobody ended this streak
		streak.streakEndedPlayerIndex=-1;
		switch (lastKillType)
		{
			case KILL_TYPE_KILL:
				// killer ended the kill streak
				killStreak.Add(streak);
				break;
			case KILL_TYPE_DEATH:
				// target ended the death streak
				deathStreak.Add(streak);
				break;
			case KILL_TYPE_TEAMKILL:
				// target ended the teamkill streak
				teamKillStreak.Add(streak);
				break;
			default:
				// not interested in other streak types
				break;
		}
	}
	
}

void PlayerInRound::Debug()
{
	wxString	durationString;
	wxString	errorMessage;
	Player		player;
	StreakData	streak;
	int			streakIndex;
	int			streakCount;
	STATSGEN_DEBUG_FUNCTION_START("PlayerInRound","Debug")

	player=globalStatistics.GetPlayer(playerIndex);
	durationString=accumulatedDuration.Format("%H:%M:%S");
	STATSGEN_DEBUG_CODE(errorMessage.Printf(
	"PLIDX[%05d]DUR[%s]K[%06d]D[%06d]TK[%06d]SU[%06d]SC[%0.2f]SK[%0.2f][%s]",
				playerIndex,
				STRING_TO_CHAR(durationString),
				kills,
				deaths,
				teamKills,
				suicides,
				score,
				skill,
				STRING_TO_CHAR(player.name));)
	STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	streakCount=killStreak.GetCount();
	if (streakCount>0)
	{
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Kill Streaks %d",streakCount);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	}
	for (streakIndex=0;streakIndex<streakCount;streakIndex++)
	{
		streak=killStreak.Item(streakIndex);
		streak.Debug();
	}

	streakCount=deathStreak.GetCount();
	if (streakCount>0)
	{
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Death Streaks %d",streakCount);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	}
	for (streakIndex=0;streakIndex<streakCount;streakIndex++)
	{
		streak=deathStreak.Item(streakIndex);
		streak.Debug();
	}

	streakCount=teamKillStreak.GetCount();
	if (streakCount>0)
	{
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Team Kill Streaks %d",streakCount);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	}
	for (streakIndex=0;streakIndex<streakCount;streakIndex++)
	{
		streak=teamKillStreak.Item(streakIndex);
		streak.Debug();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

wxString PlayerInRound::SQLTableName()
{
	wxString	retVal="playerinround";

	return (retVal);
}

wxString PlayerInRound::SQLTableNameAward()
{
	wxString	retVal="awardpoints";

	return (retVal);
}

wxString PlayerInRound::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"playerindex integer,"
				"score float,"
				"skill float,"
				"kills integer,"
				"deaths integer,"
				"teamkills integer,"
				"suicides integer,"
				"duration integer"
			")",
			STRING_TO_CHAR(SQLTableName()));

	return SQL;
}

wxString PlayerInRound::SQLCreateTableAward()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"roundindex integer,"
				"playerindex integer,"
				"awardindex integer,"
				"score integer"
			")",
			STRING_TO_CHAR(SQLTableNameAward()));

	return SQL;
}

bool PlayerInRound::WriteToDatabase(int roundIndex,int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	int			streakCount;
	int			streakIndex;
	StreakData	streak;
	int			awardPoint;
	int			awardIndex;
	int			awardCount;
	Player		player;
	int			actualPlayerIndex;

	STATSGEN_DEBUG_FUNCTION_START("PlayerInRound","WriteToDatabase")
	player=globalStatistics.playerList.Item(playerIndex);
	actualPlayerIndex=player.actualPlayerIndex;
	SQL.Printf("insert into %s"
				"(roundindex,playerindex,score,skill,"
				"kills,deaths,teamkills,suicides,duration)"
				"values"
				"('%d','%d','%f','%f',"
				"'%d','%d','%d','%d','%d')",
				STRING_TO_CHAR(SQLTableName()),
				roundIndex,actualPlayerIndex,score,skill,
				kills,deaths,teamKills,suicides,
				(int)accumulatedDuration.GetSeconds().ToLong());
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Written playerinround")
	awardCount=awardPoints.GetCount();
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		awardPoint=awardPoints.Item(awardIndex);
		if (awardPoint!=PLAYERINROUND_AWARD_ZERO)
		{
			SQL.Printf("insert into %s"
				"(roundindex,playerindex,awardindex,score)"
				"values"
				"('%d','%d','%d','%d')",
				STRING_TO_CHAR(SQLTableNameAward()),
				roundIndex,actualPlayerIndex,awardIndex,awardPoint);
			globalStatistics.statsgenDatabase.SimpleExecute(SQL);
		}
	}
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Written awardpoints")
	streakCount=teamKillStreak.GetCount();
	for (streakIndex=0;streakIndex<streakCount;streakIndex++)
	{
		streak=teamKillStreak.Item(streakIndex);
		streak.WriteToDatabase(roundIndex,actualPlayerIndex,streakIndex,
								KILL_TYPE_TEAMKILL);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Written teamkillstreak")
	streakCount=killStreak.GetCount();
	for (streakIndex=0;streakIndex<streakCount;streakIndex++)
	{
		streak=killStreak.Item(streakIndex);
		streak.WriteToDatabase(roundIndex,actualPlayerIndex,streakIndex,
								KILL_TYPE_KILL);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Written killstreak")
	streakCount=deathStreak.GetCount();
	for (streakIndex=0;streakIndex<streakCount;streakIndex++)
	{
		streak=deathStreak.Item(streakIndex);
		streak.WriteToDatabase(roundIndex,actualPlayerIndex,streakIndex,
								KILL_TYPE_DEATH);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Written deathstreak")
	STATSGEN_DEBUG_FUNCTION_END
	return retVal;
}

void PlayerInRound::ZeroAwardPoints()
{
	int			awardCount;
	int			awardIndex;
	int			awardPoint=PLAYERINROUND_AWARD_ZERO;

	awardCount=globalStatistics.awardDefinitions.GetCount();
	awardPoints.Clear();
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		awardPoints.Add(awardPoint);
	}
}

void PlayerInRound::UpdateActionAwardPoints(wxString &actionCode)
{
	int				awardCount;
	int				awardIndex;
	int				awardPoint;
	wxArrayInt		tempAwardPoints;
	bool			noScore;
	int				componentScore;

	awardCount=globalStatistics.awardDefinitions.GetCount();
	
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		AwardDefinition	award=globalStatistics.awardDefinitions.Item(awardIndex);
		awardPoint=awardPoints.Item(awardIndex);
		componentScore=award.ActionScore(&noScore,actionCode);
		if (!noScore)
		{
			if (awardPoint==PLAYERINROUND_AWARD_ZERO)
			{
				awardPoint=componentScore;
			}
			else
			{
				awardPoint+=componentScore;
			}
		}
		tempAwardPoints.Add(awardPoint);
	}

	awardPoints=tempAwardPoints;
}

void PlayerInRound::UpdateWeaponAwardPoints(wxString &weaponCode,bool kill,bool teamkill)
{
	int				awardCount;
	int				awardIndex;
	int				awardPoint;
	wxArrayInt		tempAwardPoints;
	bool			noScore;
	int				componentScore;

	awardCount=globalStatistics.awardDefinitions.GetCount();
	
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		AwardDefinition	award=globalStatistics.awardDefinitions.Item(awardIndex);
		awardPoint=awardPoints.Item(awardIndex);
		componentScore=award.WeaponScore(&noScore,weaponCode,kill,teamkill);
		if (!noScore)
		{
			if (awardPoint==PLAYERINROUND_AWARD_ZERO)
			{
				awardPoint=componentScore;
			}
			else
			{
				awardPoint+=componentScore;
			}
		}
		tempAwardPoints.Add(awardPoint);
	}

	awardPoints=tempAwardPoints;
}

void PlayerInRound::UpdateLocationAwardPoints(wxString &locationCode,bool kill,bool teamkill)
{
	int				awardCount;
	int				awardIndex;
	int				awardPoint;
	wxArrayInt		tempAwardPoints;
	bool			noScore;
	int				componentScore;

	awardCount=globalStatistics.awardDefinitions.GetCount();
	
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		AwardDefinition	award=globalStatistics.awardDefinitions.Item(awardIndex);
		awardPoint=awardPoints.Item(awardIndex);
		componentScore=award.LocationScore(&noScore,locationCode,kill,teamkill);
		if (!noScore)
		{
			if (awardPoint==PLAYERINROUND_AWARD_ZERO)
			{
				awardPoint=componentScore;
			}
			else
			{
				awardPoint+=componentScore;
			}
		}
		tempAwardPoints.Add(awardPoint);
	}

	awardPoints=tempAwardPoints;
}

void PlayerInRound::UpdateMiscAwardPoints(char *code,int count)
{
	int				awardCount;
	int				awardIndex;
	int				awardPoint;
	wxArrayInt		tempAwardPoints;
	bool			noScore;
	int				componentScore;

	awardCount=globalStatistics.awardDefinitions.GetCount();
	
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		AwardDefinition	award=globalStatistics.awardDefinitions.Item(awardIndex);
		awardPoint=awardPoints.Item(awardIndex);
		componentScore=award.MiscScore(&noScore,code,count);
		if (!noScore)
		{
			if (awardPoint==PLAYERINROUND_AWARD_ZERO)
			{
				awardPoint=componentScore;
			}
			else
			{
				awardPoint+=componentScore;
			}
		}
		tempAwardPoints.Add(awardPoint);
	}

	awardPoints=tempAwardPoints;
}

void PlayerInRound::UpdateXPAwardPoints(wxString &xpCode,float xpPoints)
{
	int				awardCount;
	int				awardIndex;
	int				awardPoint;
	wxArrayInt		tempAwardPoints;
	bool			noScore;
	int				componentScore;

	awardCount=globalStatistics.awardDefinitions.GetCount();
	
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		AwardDefinition	award=globalStatistics.awardDefinitions.Item(awardIndex);
		awardPoint=awardPoints.Item(awardIndex);
		componentScore=award.XPScore(&noScore,xpCode,xpPoints);
		if (!noScore)
		{
			if (awardPoint==PLAYERINROUND_AWARD_ZERO)
			{
				awardPoint=componentScore;
			}
			else
			{
				awardPoint+=componentScore;
			}
		}
		tempAwardPoints.Add(awardPoint);
	}

	awardPoints=tempAwardPoints;
}


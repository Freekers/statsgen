// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>

// Statsgen includes
#include "Player.h"
#include "GlobalStatistics.h"

Player::Player()
{
	name="";
	ID="";
	score=0.0;
	skill=0.0;
	scoreWeighted=0.0;
	skillWeighted=0.0;
	kills=0;
	deaths=0;
	teamKills=0;
	teamWins=0;
	teamLosses=0;
	longestKillStreak=0;
	longestTeamKillStreak=0;
	longestDeathStreak=0;
	suicides=0;
	rounds=0;
	accumulatedDuration=0;
	dropped=false;
	lastActiveRound=0;
	clanIndex=-1;
	lastRoundPlayed=0;
	firstRoundPlayed=-1;
	actualPlayerIndex=-1;
}

Player::~Player()
{
}
wxString Player::SQLTableName()
{
	wxString	retVal="player";

	return (retVal);
}

wxString Player::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"playerindex integer,"
				"%s,"
				"%s,"
				"score float,"
				"skill float,"
				"scoreweighted float,"
				"skillweighted float,"
				"kills integer,"
				"deaths integer,"
				"teamkills integer,"
				"teamwins integer,"
				"teamlosses integer,"
				"longestkillstreak integer,"
				"longestteamkillstreak integer,"
				"longestdeathstreak integer,"
				"suicides integer,"
				"rounds integer,"
				"duration integer,"
				"lastactiveround integer,"
				"clanindex integer,"
				"dropped varchar(1)"
			")",
			SQLTableName().GetData(),
	StatsgenDatabase::StringFieldDefinition("name","playername",FIELD_WIDTH_PLAYER_NAME).GetData(),
	StatsgenDatabase::StringFieldDefinition("id","playerguid",FIELD_WIDTH_PLAYER_GUID).GetData()
			);

	return SQL;
}

bool Player::WriteToDatabase(int itemIndex)
{
	bool	updateExisting;

	updateExisting=(itemIndex<globalStatistics.statsgenDatabase.NextPlayerIndex());
	return WriteToDatabaseUpdate(itemIndex,updateExisting);

	
}

bool Player::WriteToDatabaseUpdate(int itemIndex,bool updateExisting)
{
	wxString	SQL;
	bool		retVal=true;
	char		droppedChar;

	if (dropped)
	{
		droppedChar='Y';
	}
	else
	{
		droppedChar='N';
	}

	if (updateExisting)
	{
		// Updating an existing player
		SQL.Printf("update %s set "
					"score='%f',"
					"skill='%f',"
					"scoreweighted='%f',"
					"skillweighted='%f',"
					"kills='%d',"
					"deaths='%d',"
					"teamkills='%d',"
					"teamwins='%d',"
					"teamlosses='%d',"
					"longestkillstreak='%d',"
					"longestteamkillstreak='%d',"
					"longestdeathstreak='%d',"
					"suicides='%d',"
					"rounds='%d',"
					"duration='%d',"
					"lastactiveround='%d',"
					"clanindex='%d',"
					"dropped='%c' "
					"where playerindex='%d'",
				SQLTableName().GetData(),
				score, skill,
				scoreWeighted, skillWeighted,
				kills,deaths,
				teamKills,
				teamWins,teamLosses,
				longestKillStreak,
				longestTeamKillStreak,
				longestDeathStreak,
				suicides,
				rounds,
				(int)accumulatedDuration.GetSeconds().ToLong(),
				lastActiveRound,
				clanIndex,
				droppedChar,
				itemIndex);
	}
	else
	{
		// Inserting a new player
		SQL.Printf("insert into %s"
				"(playerindex,name,id,score,skill,scoreweighted,skillweighted,"
				"kills,deaths,teamkills,teamwins,teamlosses,"
				"longestkillstreak,longestteamkillstreak,longestdeathstreak,"
				"suicides,rounds,duration,lastactiveround,clanindex,dropped)"
				"values"
				"('%d',"	// Player Index
				"'%s',"		// name
				"'%s',"		// id
				"'%f',"		// score
				"'%f',"		// skill
				"'%f',"		// score weighted
				"'%f',"		// skill weighted
				"'%d',"		// kills
				"'%d',"		// deaths
				"'%d',"		// teamkills
				"'%d',"		// teamwins
				"'%d',"		// teamlosses
				"'%d',"		// longestkillstreak
				"'%d',"		// longestteamkillstreak
				"'%d',"		// longestdeathstreak
				"'%d',"		// suicides
				"'%d',"		// rounds
				"'%d',"		// duration
				"'%d',"		// lastactiveround
				"'%d',"		// clanindex
				"'%c')",	// dropped
				SQLTableName().GetData(),
				itemIndex,
				StatsgenDatabase::SafeForInsert(name).GetData(),
				StatsgenDatabase::SafeForInsert(ID).GetData(),
				score, skill,
				scoreWeighted, skillWeighted,
				kills,deaths,
				teamKills,
				teamWins,teamLosses,
				longestKillStreak,
				longestTeamKillStreak,
				longestDeathStreak,
				suicides,
				rounds,
				(int)accumulatedDuration.GetSeconds().ToLong(),
				lastActiveRound,
				clanIndex,
				droppedChar);
	}
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	return retVal;
}

void Player::ZeroScore()
{
	score					=0.0;
	skill					=0.0;
	scoreWeighted			=0.0;
	skillWeighted			=0.0;
	kills					=0;
	deaths					=0;
	teamKills				=0;
	suicides				=0;
	teamWins				=0;
	teamLosses				=0;
	longestKillStreak		=0;
	longestTeamKillStreak	=0;
	accumulatedDuration		=0;

}

bool Player::UpdateFromDatabase(int newPlayerIndex)
{
	wxString			sql;
	TemplateOpenQuery	query;
	bool				playerUpdated;
	static wxString		propertyPlayerIndex				="playerindex";
	static wxString		propertyScore					="score";
	static wxString		propertySkill					="skill";
	static wxString		propertyKills					="kills";
	static wxString		propertyDeaths					="deaths";
	static wxString		propertyTeamKills				="teamkills";
	static wxString		propertyLongestKillStreak		="longestkillstreak";
	static wxString		propertyLongestTeamKillStreak	="longestteamkillstreak";
	static wxString		propertyLongestDeathStreak		="longestdeathstreak";
	static wxString		propertyTeamWins				="teamwins";
	static wxString		propertyTeamLosses				="teamlosses";
	static wxString		propertySuicides				="suicides";
	static wxString		propertyRounds					="rounds";
	static wxString		propertyDuration				="duration";
	static wxString		propertyDropped					="dropped";

	sql.Printf("select * from player where name=\'%s\'",StatsgenDatabase::SafeForInsert(name).GetData());

	query.Initiate(sql,globalStatistics.statsgenDatabase.DBHandle());
	playerUpdated=query.NextRow();
	if (playerUpdated)
	{
		actualPlayerIndex		=atoi(query.RetrieveProperty(propertyPlayerIndex).GetData());
		score					=atof(query.RetrieveProperty(propertyScore).GetData());
		skill					=atof(query.RetrieveProperty(propertySkill).GetData());
		kills					=atoi(query.RetrieveProperty(propertyKills).GetData());
		deaths					=atoi(query.RetrieveProperty(propertyDeaths).GetData());
		teamWins				=atoi(query.RetrieveProperty(propertyTeamWins).GetData());
		teamLosses				=atoi(query.RetrieveProperty(propertyTeamLosses).GetData());
		teamKills				=atoi(query.RetrieveProperty(propertyTeamKills).GetData());
		longestKillStreak		=atoi(query.RetrieveProperty(propertyLongestKillStreak).GetData());
		longestTeamKillStreak	=atoi(query.RetrieveProperty(propertyLongestTeamKillStreak).GetData());
		longestDeathStreak		=atoi(query.RetrieveProperty(propertyLongestDeathStreak).GetData());
		suicides				=atoi(query.RetrieveProperty(propertySuicides).GetData());
		rounds					=atoi(query.RetrieveProperty(propertyRounds).GetData());
		accumulatedDuration		=wxTimeSpan::Seconds(atoi(query.RetrieveProperty(propertyDuration).GetData()));
		dropped					=(query.RetrieveProperty(propertyDropped).CmpNoCase("Y") == 0);
	}
	else
	{
		actualPlayerIndex=newPlayerIndex;
	}

	return (playerUpdated);
}

void Player::Add(Player &secondary)
{
	score+=secondary.score;
	skill+=secondary.skill;
	kills+=secondary.kills;
	deaths+=secondary.deaths;
	teamKills+=secondary.teamKills;
	suicides+=secondary.suicides;
	teamWins+=secondary.teamWins;
	teamLosses+=secondary.teamLosses;
	rounds+=secondary.rounds;
	accumulatedDuration=accumulatedDuration.Add(secondary.accumulatedDuration);
	scoreWeighted+=score/rounds;
	skillWeighted+=skill/rounds;
	if (secondary.longestKillStreak>longestKillStreak)
	{
		longestKillStreak=secondary.longestKillStreak;
	}
	if (secondary.longestTeamKillStreak>longestTeamKillStreak)
	{
		longestTeamKillStreak=secondary.longestTeamKillStreak;
	}
	if (secondary.longestDeathStreak>longestDeathStreak)
	{
		longestDeathStreak=secondary.longestDeathStreak;
	}
	if (secondary.lastActiveRound>lastActiveRound)
	{
		lastActiveRound=secondary.lastActiveRound;
	}
	if (secondary.lastRoundPlayed>lastRoundPlayed)
	{
		lastRoundPlayed=secondary.lastRoundPlayed;
	}
	if (secondary.firstRoundPlayed<firstRoundPlayed)
	{
		firstRoundPlayed=secondary.firstRoundPlayed;
	}
}

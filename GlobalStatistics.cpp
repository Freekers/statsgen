// wxWindows includes
#include <wx/timer.h>

// Statsgen includes
#include "GlobalStatistics.h"
#include "Player.h"
#include "GenericKey.h"
#include "TemplateFile.h"
#include "Server.h"
#include "Website.h"
#include "AwardDefinition.h"
#include "Progress.h"
#include "KillTotal.h"
#include "ExternalDatabase.h"
#include "AliasCacheEntry.h"
#include "Cheater.h"
#include "Version.h"

GlobalStatistics	globalStatistics;

GlobalStatistics::GlobalStatistics()
{
	hiddenFeatures=false;
	currentRunningThread=NULL;
	aliasCacheEnabled=false;
	aliasCache=NULL;
	aliasPrimaryCache=NULL;
	aliasCache=new ArrayOfAliasCacheEntry(AliasCacheEntry::Compare);
	aliasPrimaryCache=new ArrayOfAliasCacheEntry(AliasCacheEntry::Compare);

	playerDataAvatars=new PlayerDataList("/General/PlayerDataFilenameAvatar",
								"playerdataavatars",
								"playerdataavatar.txt");
	playerDataWebsite=new PlayerDataList("/General/PlayerDataFilenameWebsite",
								"playerdatawebsite",
								"playerdatawebsite.txt");
	playerDataPicture=new PlayerDataList("/General/PlayerDataFilenamePicture",
								"playerdatapicture",
								"playerdatapicture.txt");
	playerDataXfire=new PlayerDataList("/General/PlayerDataFilenameXfire",
								"playerdataxfire",
								"playerdataxfire.txt");
	playerDataClanRole=new PlayerDataList("/General/PlayerDataFilenameClanRole",
								"playerdataclanrole",
								"playerdataclanrole.txt");
	
	playerDataRealName=new PlayerDataList("/General/PlayerDataFilenameRealName",
								"playerdatarealname",
								"playerdatarealname.txt");
	playerDataMisc1=new PlayerDataList("/General/PlayerDataFilenameMisc1",
								"playerdatamisc1",
								"playerdatamisc1.txt");
	playerDataMisc2=new PlayerDataList("/General/PlayerDataFilenameMisc2",
								"playerdatamisc2",
								"playerdatamisc2.txt");
	// Read the schedule configs
	//ReadScheduleConfig();
	FlagEndOfRun();
}

void GlobalStatistics::FlagStartOfRun()
{
	statsRunning = true;
}

void GlobalStatistics::FlagEndOfRun()
{
	statsRunning = false;
}

bool GlobalStatistics::AreStatsRunning()
{
	return (statsRunning);
}

void GlobalStatistics::EmptyAliasCache()
{
	if (aliasCache!=NULL)
	{
		int	aliasCacheCount;
		int	aliasCacheIndex;
		AliasCacheEntry		*cacheEntry;
		aliasCacheCount=aliasCache->GetCount();
		for (aliasCacheIndex=0;aliasCacheIndex<aliasCacheCount;aliasCacheIndex++)
		{
			cacheEntry=aliasCache->Item(aliasCacheIndex);
			if (cacheEntry!=NULL)
			{
				delete cacheEntry;
			}
		}
		
		aliasCache->Clear();
	}
	if (aliasPrimaryCache!=NULL)
	{
		int	aliasCacheCount;
		int	aliasCacheIndex;
		AliasCacheEntry		*cacheEntry;
		aliasCacheCount=aliasPrimaryCache->GetCount();
		for (aliasCacheIndex=0;aliasCacheIndex<aliasCacheCount;aliasCacheIndex++)
		{
			cacheEntry=aliasPrimaryCache->Item(aliasCacheIndex);
			if (cacheEntry!=NULL)
			{
				delete cacheEntry;
			}
		}
		
		aliasPrimaryCache->Clear();
	}
}

GlobalStatistics::~GlobalStatistics()
{
	if (aliasCache!=NULL)
	{
		EmptyAliasCache();
		delete aliasCache;
		aliasCache=NULL;
	}
	if (aliasPrimaryCache!=NULL)
	{
		delete aliasPrimaryCache;
		aliasPrimaryCache=NULL;
	}
	if (playerDataAvatars!=NULL)
	{
		delete playerDataAvatars;
	}
	if (playerDataPicture=NULL)
	{
		delete playerDataPicture;
	}
	if (playerDataWebsite!=NULL)
	{
		delete playerDataWebsite;
	}
	if (playerDataClanRole!=NULL)
	{
		delete playerDataClanRole;
	}
	if (playerDataXfire!=NULL)
	{
		delete playerDataXfire;
	}
	if (playerDataRealName!=NULL)
	{
		delete playerDataRealName;
	}
	if (playerDataMisc1!=NULL)
	{
		delete playerDataMisc1;
	}
	if (playerDataMisc2!=NULL)
	{
		delete playerDataMisc2;
	}
	ClearServers();
}

Player GlobalStatistics::GetPlayer(int index)
{
	Player		player;

	//player=players.Item(index);
	player=playerList.GetPlayer(index);
	return(player);
}

int GlobalStatistics::FindPlayerIndex(wxString &name)
{
	/*
	int		playerCount;
	int		playerIndex;
	Player		player;
	int		foundIndex=-1;

	playerCount=players.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=players.Item(playerIndex);
		if (player.name.Cmp(name)==0)
		{
			foundIndex=playerIndex;
			break;
		}
	}

	return(foundIndex);
	*/
	return(playerList.FindPlayerIndex(name));
}

int GlobalStatistics::AddPlayer(wxString &name,wxString &id)
{
	return(playerList.AddPlayer(name,id));
	/*
	Player			player;
	int				playerIndex=-1;
	int				guidIndex=-1;
	AliasListEntry	alias;
	wxString		AKAName;
	bool			AKAFound;
	int				aliasCount;
	int				aliasIndex;
	bool			aliasFound;

	alias.aliasGUID=id;
	alias.aliasName=name;
	if (FindAlias(alias))
	{
		AKAFound=true;
		AKAName=name;
		id=alias.primaryGUID;
		name=alias.primaryName;
		if (name.Cmp(AKAName)==0)
		{
			// This is the same name - not really an AKA
			AKAFound=false;
		}
	}
	else
	{
		AKAFound=false;
		if (useAutomaticAliases)
		{
			// auto aliasing is enabled and this is not
			// already an alias
			if (id.Cmp("COD2_0")!=0)
			{
				// this is not the 0 guid bug for cod2
				// does this guid already exist in a current
				// alias list .. if so we need to add it
				aliasCount=aliasList.GetCount();
				aliasFound=false;
				for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
				{
					alias=aliasList.Item(aliasIndex);
					if (id.Cmp(alias.primaryGUID)==0)
					{
						// found a matching one
						// add it to the list
						aliasFound=true;
						alias.aliasGUID=id;
						alias.aliasName=name;
						AddAlias(alias);
						AKAFound=true;
						AKAName=name;
						break;
					}
				}
				if (!aliasFound)
				{
					// ok - no matching guid found in existing alias list
					// we need to scan back through the player list
					// to see if there is a matching guid
					// if so ... this is an alias
					// and we need to add it
					guidIndex=FindGUIDIndex(id);
					if (guidIndex!=-1)
					{
						// guid exists currently
						// got to see if this is actually
						// the same player as we currently are
						playerIndex=FindPlayerIndex(name);
						if (guidIndex!=playerIndex)
						{
							// this player has the same guid as an
							// existing player - but it is not the
							// same player
							// we need to add this player as an alias of
							// of the original
							alias.aliasGUID=id;
							alias.aliasName=name;
							player=players.Item(guidIndex);
							alias.primaryGUID=player.ID;
							alias.primaryName=player.name;
							AddAlias(alias);
							AddAKA(guidIndex,name);
							return (guidIndex);
						}
						else
						{
							// this is the same player
							return (playerIndex);
						}
					}
				}
			}
		}
		else
		{
		}
	}

	playerIndex=FindPlayerIndex(name);
	if (playerIndex==-1)
	{
		player.name=name;
		player.ID=id;
		players.Add(player);
		playerIndex=players.GetCount()-1;
	}

	if (AKAFound)
	{
		AddAKA(playerIndex,AKAName);
	}

	return(playerIndex);
	*/
}
void GlobalStatistics::GenericKeyDebug(ArrayOfGenericKey &arrayKey,
						char *tableName)
{
	int			keyIndex;
	int			keyCount;
	GenericKey	key;
	wxString	msg;
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","GenericKeyDebug")

	keyCount=arrayKey.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,tableName)
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=arrayKey.Item(keyIndex);
		STATSGEN_DEBUG_CODE(msg.Printf("%03d:%s",keyIndex,key.id.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::Debug()
{
	wxString	errorMessage;
	int			roundCount;
	int			roundIndex;
	Player		player;
	Round		round;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","Debug")
	/*
	playerCount=players.GetCount();
	roundCount=rounds.GetCount();
	errorMessage.Printf("Player Count=[%d]",playerCount);
	STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=players.Item(playerIndex);
		errorMessage.Printf("[%d] [%s] [%s]",
			playerIndex,
			player.ID.GetData(),
			player.name.GetData());
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	}
	*/

	GenericKeyDebug(keysAction		,TABLE_NAME_KEY_ACTION);
	GenericKeyDebug(keysAmmo		,TABLE_NAME_KEY_AMMO);
	GenericKeyDebug(keysMap			,TABLE_NAME_KEY_MAP);
	GenericKeyDebug(keysWeapon		,TABLE_NAME_KEY_WEAPON);
	GenericKeyDebug(keysClass		,TABLE_NAME_KEY_CLASS);
	GenericKeyDebug(keysTeam		,TABLE_NAME_KEY_TEAM);
	GenericKeyDebug(keysGametype	,TABLE_NAME_KEY_GAMETYPE);
	GenericKeyDebug(keysLocation	,TABLE_NAME_KEY_LOCATION);

	STATSGEN_DEBUG_CODE(errorMessage.Printf("Round Count=[%d]",roundCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		STATSGEN_DEBUG_CODE(errorMessage.Printf("Round=%d",roundIndex);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
		round=rounds.Item(roundIndex);
		round.Debug();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

wxString GlobalStatistics::GenericKeyID(ArrayOfGenericKey &keyArray,int index)
{
	GenericKey	id;
	wxString	msg;

	id=keyArray.Item(index);
	return(id.id);
}

int GlobalStatistics::GenericKeyIndex(ArrayOfGenericKey &keyArray,wxString &id)
{
	int			keyCount;
	int			keyIndex;
	GenericKey	idFound;
	int			foundIndex=-1;

	keyCount=keyArray.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		idFound=keyArray.Item(keyIndex);
		if (idFound.id.Cmp(id)==0)
		{
			foundIndex=keyIndex;
			break;
		}
	}

	return(foundIndex);
}

int GlobalStatistics::GenericKeyAdd(ArrayOfGenericKey &keyArray,wxString &id)
{
	int			keyIndex=-1;
	GenericKey	keyToAdd;

	keyIndex=GenericKeyIndex(keyArray,id);
	if (keyIndex==-1)
	{
		keyToAdd.id=id;
		keyArray.Add(keyToAdd);
		keyIndex=keyArray.GetCount()-1;
	}

	return(keyIndex);
}

bool GlobalStatistics::WriteToDatabase()
{
	bool			retVal=true;
	int				keyCount;
	int				keyIndex;
	GenericKey		key;
	int				roundCount;
	int				roundIndex;
	int				awardCount;
	int				awardIndex;
	int				clanCount;
	int				clanIndex;
	Round			round;
	Player			player;
	wxString		msg;
	wxString		beginTransaction;
	wxString		endTransaction;
	int				serverCount;
	int				serverIndex;
	int				AKACount;
	int				AKAIndex;
	AKAListEntry	AKAEntry;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","WriteToDatabase")
	//statsgenDatabase.DropIndexes();
	// Write Generic Keys
	// Action
	progress->Initiate((long)rounds.GetCount(),
						" rounds",
						1,
						" rounds",
						1);
	beginTransaction="begin";
	endTransaction="commit";
	statsgenDatabase.SimpleExecute(beginTransaction);

	skillWeights.WriteToDatabase();
	scoreWeights.WriteToDatabase();
	cheaters.WriteToDatabase();

	keyCount=keysXP.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysXP.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_XP,keyIndex);
	}
	
	keyCount=keysAction.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysAction.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_ACTION,keyIndex);
	}
	
	// Ammo
	keyCount=keysAmmo.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysAmmo.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_AMMO,keyIndex);
	}
	
	// Weapon
	keyCount=keysWeapon.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysWeapon.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_WEAPON,keyIndex);
	}
	
	// Map
	keyCount=keysMap.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysMap.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_MAP,keyIndex);
	}
	
	// Team
	keyCount=keysTeam.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysTeam.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_TEAM,keyIndex);
	}
	
	// Class
	keyCount=keysClass.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysClass.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_CLASS,keyIndex);
	}
	
	// Gametype
	keyCount=keysGametype.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysGametype.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_GAMETYPE,keyIndex);
	}
	
	// Location
	keyCount=keysLocation.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keysLocation.Item(keyIndex);
		key.WriteToDatabase(TABLE_NAME_KEY_LOCATION,keyIndex);
	}
	
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Writing players to database")
	/*
	playerCount=players.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=players.Item(playerIndex);
		player.WriteToDatabase(playerIndex);
	}
	*/
	playerList.WriteToDatabase();

	playerDataAvatars->WriteToDatabase();
	playerDataRealName->WriteToDatabase();
	playerDataPicture->WriteToDatabase();
	playerDataWebsite->WriteToDatabase();
	playerDataXfire->WriteToDatabase();
	playerDataClanRole->WriteToDatabase();
	playerDataMisc1->WriteToDatabase();
	playerDataMisc2->WriteToDatabase();

	STATSGEN_DEBUG(DEBUG_ALWAYS,"Writing award definition")
	awardCount=awardDefinitions.GetCount();
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		AwardDefinition	award=awardDefinitions.Item(awardIndex);
		award.WriteToDatabase(awardIndex);
	}

	STATSGEN_DEBUG(DEBUG_ALWAYS,"Writing clan definition")
	clanCount=clans.GetCount();
	STATSGEN_DEBUG_CODE(msg.Printf("CLAN COUNT WRITING=%d",clanCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	for (clanIndex=0;clanIndex<clanCount;clanIndex++)
	{
		Clan	*clan=clans.Detach(0);
		clan->WriteToDatabase(clanIndex);
		clans.Add(clan);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Written clan definition")

	killTotalLists.WriteToDatabase();
	totalXP.WriteToDatabase();

	// Write Server Info
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Writing server info")
	serverCount=servers.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		Server	*server;

		//server=servers.Detach(0);
		server=(Server *)servers.Item(serverIndex);
		server->WriteToDatabase(serverIndex);
		//servers.Add(server);
	}

	// Write AKA Info
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Writing AKA info")
	AKACount=AKAList.GetCount();
	for (AKAIndex=0;AKAIndex<AKACount;AKAIndex++)
	{
		AKAEntry=AKAList.Item(AKAIndex);
		AKAEntry.WriteToDatabase();
	}

	statsgenDatabase.SimpleExecute(endTransaction);
	// Write Round Info (all sub bits written by the round writing)
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Writing rounds")
	roundCount=rounds.GetCount();
	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		progress->Update((long)(roundIndex+1));
		STATSGEN_DEBUG_CODE(msg.Printf("Writing Round %d of %d to database", roundIndex+1,roundCount);)
		STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
		round=rounds.Item(roundIndex);
		if ((roundIndex % 100)==0)
		{
			if (roundIndex!=0)
			{
				statsgenDatabase.SimpleExecute(endTransaction);
			}
			statsgenDatabase.SimpleExecute(beginTransaction);
		}
		round.WriteToDatabase(statsgenDatabase.NextRoundIndex()+roundIndex);
	}

	if (roundCount!=0)
	{
		statsgenDatabase.SimpleExecute(endTransaction);
	}

	// Update rounds and players based upon changed drop status
	statsgenDatabase.WriteChangedPlayersList(droppedPlayersChanged);
	statsgenDatabase.WriteChangedRoundsList();
	// update rounds and players based upon newly created rounds
	statsgenDatabase.WriteChangedPlayersList(playersChanged);
	statsgenDatabase.WriteChangedRoundsList(roundsChanged);

	//statsgenDatabase.CreateIndexes();
	STATSGEN_DEBUG_FUNCTION_END
	return retVal;
}

int GlobalStatistics::FindGUIDIndex(wxString &guid)
{
	return (playerList.FindGUIDIndex(guid));
	/*
	int		playerCount;
	int		playerIndex;
	Player		player;
	int		foundIndex=-1;

	playerCount=players.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=players.Item(playerIndex);
		if (player.ID.Cmp(guid)==0)
		{
			foundIndex=playerIndex;
			break;
		}
	}

	return(foundIndex);
	*/
}

void GlobalStatistics::ReadPlayerCache()
{
	PlayerCacheEntry	playerCacheEntry;
	int					playerCount;
	int					playerIndex;
	wxTextFile			fp;
	wxString			configKey="/General/playercache";
	wxString			configValue;
	bool				retVal;
	wxString			lineRead;
	wxString			guid;
	wxString			name;

	configData.ReadTextValue(configKey,&configValue,"playercache.dat");
	playerCache.Clear();
	if (wxFileExists(configValue))
	{
		retVal=fp.Open(configValue);
		if (retVal)
		{
			playerCount=fp.GetLineCount();
			for (playerIndex=0;playerIndex<playerCount;playerIndex++)
			{
				lineRead=fp.GetLine(playerIndex);
				guid=lineRead.BeforeFirst(',');
				name=lineRead.AfterFirst(',');
				playerCacheEntry.guid=guid;
				playerCacheEntry.name=name;
				playerCache.Add(playerCacheEntry);
			}
		}
	}
}

void GlobalStatistics::WritePlayerCache()
{
	PlayerCacheEntry	playerCacheEntry;
	int					playerCount;
	int					playerIndex;
	FILE				*fp;
	wxString			configKey="/General/playercache";
	wxString			configValue;

	configData.ReadTextValue(configKey,&configValue,"playercache.dat");
	fp=fopen(configValue.GetData(),"w");
	if (fp!=NULL)
	{
		playerCount=playerCache.GetCount();
		for (playerIndex=0;playerIndex<playerCount;playerIndex++)
		{
			playerCacheEntry=playerCache.Item(playerIndex);
			fprintf(fp,"%s,%s\n",
					playerCacheEntry.guid.GetData(),
					playerCacheEntry.name.GetData());
		}
		fclose(fp);
	}
}

void GlobalStatistics::PlayerListToPlayerCache()
{
	PlayerCacheEntry	playerCacheEntry;
	Player				player;
	int					playerCount;
	int					playerIndex;
	int					playerCacheCount;
	int					playerCacheIndex;
	PlayerCacheEntry	currentPlayerCacheEntry;

	ReadPlayerCache();

	playerCount=playerList.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=playerList.Item(playerIndex);
		
		playerCacheEntry.guid=player.ID;
		playerCacheEntry.name=player.name;

		playerCacheCount=playerCache.GetCount();
		for (playerCacheIndex=0;playerCacheIndex<playerCacheCount;playerCacheIndex++)
		{
			currentPlayerCacheEntry=playerCache.Item(playerCacheIndex);
			if (currentPlayerCacheEntry.name.Cmp(player.name) == 0)
			{
				playerCache.RemoveAt(playerCacheIndex);
				break;
			}
		}
		playerCache.Add(playerCacheEntry);
	}
}

void GlobalStatistics::ReadAliasList()
{
	AliasListEntry		aliasListEntry;
	int					aliasCount;
	int					aliasIndex;
	wxTextFile			fp;
	wxString			configKey="/General/aliaslist";
	wxString			configValue;
	bool				retVal;
	wxString			status;

	status="Reading Aliases"; progress->SetStatus(status);
	configData.ReadTextValue(configKey,&configValue,"aliaslist.dat");
	aliasList.Clear();
	EmptyAliasCache();
	if (wxFileExists(configValue))
	{
		retVal=fp.Open(configValue);
		if (retVal)
		{
			aliasCount=fp.GetLineCount()/2;
			progress->Initiate(aliasCount,
							"",
							(long)1,
							"",
							1);
			for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
			{
				progress->Update(aliasIndex+1);
				aliasListEntry.ReadFromFile(&fp,aliasIndex);
				aliasList.Add(aliasListEntry);
				AddToAliasCache(aliasCache,
								aliasListEntry.aliasName,
								aliasListEntry.aliasGUID,
								aliasIndex);
				AddToAliasCache(aliasPrimaryCache,
								aliasListEntry.primaryName,
								aliasListEntry.primaryGUID,
								aliasIndex);
			}
		}
	}
}

void GlobalStatistics::WriteAliasList()
{
	AliasListEntry		aliasListEntry;
	int					aliasCount;
	int					aliasIndex;
	FILE				*fp;
	wxString			configKey="/General/aliaslist";
	wxString			configValue;

	configData.ReadTextValue(configKey,&configValue,"aliaslist.dat");
	fp=fopen(configValue.GetData(),"w");
	if (fp!=NULL)
	{
		aliasCount=aliasList.GetCount();
		for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
		{
			aliasListEntry=aliasList.Item(aliasIndex);
			aliasListEntry.WriteToFile(fp);
		}
		fclose(fp);
	}
}

void GlobalStatistics::AddToAliasCache(ArrayOfAliasCacheEntry *cache,
									wxString &name,
									wxString &guid,
									int index)
{
	AliasCacheEntry	*aliasCacheEntry;
	int				searchIndex;
	AliasCacheEntry	searchKey;

	searchKey.guid=guid;
	searchKey.name=name;
	searchIndex=cache->Index(&searchKey);
	if (searchIndex==wxNOT_FOUND)
	{
		aliasCacheEntry=new AliasCacheEntry;
		aliasCacheEntry->guid=guid;
		aliasCacheEntry->name=name;
		aliasCacheEntry->index=index;
		cache->Add(aliasCacheEntry);
	}
}

void GlobalStatistics::AddAlias(AliasListEntry &aliasEntry)
{
	wxString	msg;
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","AddAlias")
	if ((aliasEntry.primaryGUID.Cmp(aliasEntry.aliasGUID)!=0) ||
		(aliasEntry.primaryName.Cmp(aliasEntry.aliasName)!=0))
	{
		msg.Printf("primary [%s,%s] alias [%s,%s]",
				aliasEntry.primaryGUID.GetData(),
				aliasEntry.primaryName.GetData(),
				aliasEntry.aliasGUID.GetData(),
				aliasEntry.aliasName.GetData());
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		if (!FindAlias(aliasEntry))
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Alias does not already exist")
			// alias does not already exist - so safe to add
			aliasList.Add(aliasEntry);
			if (aliasCacheEnabled)
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,"Alias Cache Enabled")
				AddToAliasCache(aliasCache,
									aliasEntry.aliasName,
									aliasEntry.aliasGUID,
									aliasList.GetCount()-1);
				AddToAliasCache(aliasPrimaryCache,
									aliasEntry.primaryName,
									aliasEntry.primaryGUID,
									aliasList.GetCount()-1);
			}
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

bool GlobalStatistics::FindAlias(AliasListEntry &aliasEntry)
{
	int				count;
	int				index;
	bool			found;
	AliasListEntry	foundAliasEntry;
	int				foundIndex;
	int				searchIndex;
	wxString		msg;

	found=false;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","FindAlias")

	msg.Printf("finding alias for [%s,%s]",
					aliasEntry.aliasGUID.GetData(),
					aliasEntry.aliasName.GetData());
	STATSGEN_DEBUG(DEBUG_RARELY,msg);
			
	if (aliasCacheEnabled)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Alias Cache Enabled")
		AliasCacheEntry	searchKey;
		searchKey.guid=aliasEntry.aliasGUID;
		searchKey.name=aliasEntry.aliasName;

		// See if the alias is a primary name
		searchIndex=aliasPrimaryCache->Index(&searchKey);
		if (searchIndex==wxNOT_FOUND)
		{
			found=false;
		}
		else
		{
			AliasCacheEntry	*foundKey;
			foundKey=aliasPrimaryCache->Item(searchIndex);
			foundIndex=foundKey->index;
			foundAliasEntry=aliasList.Item(foundIndex);
			aliasEntry.primaryGUID=foundAliasEntry.primaryGUID;
			aliasEntry.primaryName=foundAliasEntry.primaryName;
			found=true;
		}
		if (!found)
		{
			// This is not a primary (already created) alias
			// is it a child of a primary alias?
			searchIndex=aliasCache->Index(&searchKey);
			if (searchIndex==wxNOT_FOUND)
			{
				found=false;
			}
			else
			{
				AliasCacheEntry	*foundKey;
				foundKey=aliasCache->Item(searchIndex);
				foundIndex=foundKey->index;
				foundAliasEntry=aliasList.Item(foundIndex);
				aliasEntry.primaryGUID=foundAliasEntry.primaryGUID;
				aliasEntry.primaryName=foundAliasEntry.primaryName;
				found=true;
			}
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Alias Cache Not Enabled")
		count=aliasList.GetCount();
		for (index=0;index<count;index++)
		{
			foundAliasEntry=aliasList.Item(index);
			msg.Printf("found alias for primary [%s,%s] alias [%s,%s]",
				foundAliasEntry.primaryGUID.GetData(),
				foundAliasEntry.primaryName.GetData(),
				foundAliasEntry.aliasGUID.GetData(),
				foundAliasEntry.aliasName.GetData());
			STATSGEN_DEBUG(DEBUG_RARELY,msg)
			if (
				((foundAliasEntry.aliasGUID.Cmp(aliasEntry.aliasGUID)==0) &&
				(foundAliasEntry.aliasName.Cmp(aliasEntry.aliasName)==0))
				||
				((foundAliasEntry.primaryGUID.Cmp(aliasEntry.aliasGUID)==0) &&
				(foundAliasEntry.primaryName.Cmp(aliasEntry.aliasName)==0)))
			{
				STATSGEN_DEBUG(DEBUG_RARELY,"Its a match")
				// This is a matching alias
				// so update the aliasEntry with the primary data
				aliasEntry.primaryGUID=foundAliasEntry.primaryGUID;
				aliasEntry.primaryName=foundAliasEntry.primaryName;
				found=true;
				break;
			}
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (found);
}

void GlobalStatistics::DeleteAlias(AliasListEntry &aliasEntry)
{
	int				count;
	int				index;
	AliasListEntry	foundAliasEntry;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","DeleteAlias")
	STATSGEN_DEBUG_CODE(msg.Printf("Deleting Alias Parent = [%s][%s] Child = [%s][%s]",
			aliasEntry.primaryGUID.GetData(),
			aliasEntry.primaryName.GetData(),
			aliasEntry.aliasGUID.GetData(),
			aliasEntry.aliasName.GetData());)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	count=aliasList.GetCount();
	for (index=0;index<count;index++)
	{
		foundAliasEntry=aliasList.Item(index);
		STATSGEN_DEBUG_CODE(msg.Printf("checking against alias = [%s][%s] Child = [%s][%s]",
			foundAliasEntry.primaryGUID.GetData(),
			foundAliasEntry.primaryName.GetData(),
			foundAliasEntry.aliasGUID.GetData(),
			foundAliasEntry.aliasName.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		if ((foundAliasEntry.primaryGUID.Cmp(aliasEntry.primaryGUID)==0) &&
			(foundAliasEntry.primaryName.Cmp(aliasEntry.primaryName)==0) &&
			(foundAliasEntry.aliasName.Cmp(aliasEntry.aliasName)==0) &&
			(foundAliasEntry.aliasName.Cmp(aliasEntry.aliasName)==0))
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Found alias at [%d]",index);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			// This is a matching alias
			// so delete it
			aliasList.RemoveAt(index);
			break;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::DropPlayersAndRounds()
{
	long	topValue=3;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","DropPlayersAndRounds")
	STATSGEN_DEBUG_CODE(msg.Printf("players=%d rounds=%d",playerList.GetCount(),rounds.GetCount());)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	progress->DisableTimeToGo();
	progress->Initiate(topValue,
						" Steps",
						(long)1,
						" Steps",
						(long)1);
	DropRounds();
	progress->Update(1);
	DropPlayers();
	//progress->Update(2);
	//DropInactiveRounds();
	progress->Update(3);
	DropExistingPlayers();
	progress->Update(4);
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ProduceStats(
								bool fullRun,
								wxArrayInt &allowedSteps,
								TemplateFile &templateFile)
{
	int				serverCount;
	int				serverIndex;
	wxString		templateFilename;
	wxString		configKey;
	wxString		configValue;
	Website			website;
	wxString		msg;
	bool			result;
	static time_t	startTime;

	wxBusyCursor	busyCursor;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ProduceStats")
	configKey=AutomaticAliasConfigKey();
	configData.ReadTextValue(configKey,&configValue,"n");
	useAutomaticAliases=(configValue.CmpNoCase("y")==0);

	if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS,allowedSteps))
	{
		startTime=time(&startTime);

		progress->ClearErrors();
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_GENERAL);

		// Make sure the group gets deleted
		configData.DeleteGroup("MessageCentreValues");
		configData.CommitChanges();

		configData.ReadList("BannedSpeech",bannedSpeech);

		ReadDeathmatchGametypes();
		ReadTeamSwapWeapons();
		skillWeights.Initiate("SkillWeights");
		scoreWeights.Initiate("ScoreWeights");
		if (fullRun)
		{
			// Make everything start from scratch
			configKey="/Template/FullRun";
			OpenSpeechFile();
		}
		else
		{
			configKey="/Template/QuickRun";
		}
		configData.ReadTextValue(configKey,&templateFilename,"");
		progress->LogError(templateFilename);
		ReadAliasList();
		ReadAwardDefinitions();
		ReadClanDefinitions();
		dropList.Load();

		progress->Finalise();
		InitiateSkillCalculationType();
	}
	serverCount=servers.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		//Server			*server=servers.Detach(0);
		Server			*server=(Server *)servers.Item(serverIndex);

		if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS,allowedSteps))
		{
			if (fullRun)
			{
				if (serverIndex==0)
				{
					progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS);
				}
				if (StepAllowed(STEP_DOWNLOAD_LOGS))
				{
					STATSGEN_DEBUG(DEBUG_ALWAYS,"Downloading files")
					server->DownloadBanFiles();
					server->DownloadFiles();
				}
	
				if (serverIndex==(serverCount-1))
				{
					progress->Finalise();
				}
			}
		}

		if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS,allowedSteps))
		{
			if (serverIndex==0)
			{
				progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS);
				playerList.EnableCache();
				aliasCacheEnabled=true;
			}
			if (StepAllowed(STEP_CREATE_LOG_FILES))
			{
				server->FindLogfiles();
			}

			if (fullRun)
			{
				server->ProcessBanFiles();
			}
	
			if (StepAllowed(STEP_CREATE_LOG_FILES))
			{
				server->CreateLogFiles();
			}

			if (StepAllowed(STEP_PROCESS_LOG_FILES))
			{
				server->ProcessLogFiles(serverIndex);
			}
		}

		//servers.Add(server);
		if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS,allowedSteps))
		{
			if (serverIndex==(serverCount-1))
			{
				progress->Finalise();
				playerList.DisableCache();
				aliasCacheEnabled=false;
			}
		}
	}

	if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS,allowedSteps))
	{
		if (useAutomaticAliases)
		{
			WriteAliasList();
		}
		if (fullRun)
		{
			CloseSpeechFile();
			dropList.Write();
		}

		// By now all the stats have been gathered together - we
		// no longer need the server objects
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DROPPING_PLAYERS);
		if (StepAllowed(STEP_DROP_PLAYERS))
		{
			// drop players who have not played enough rounds
			DropPlayersAndRounds();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES1);
		if (StepAllowed(STEP_CALCULATE_SCORES))
		{
			CalculateScores();
		}
		progress->Finalise();

		//Debug();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_CREATE_DATABASE);
		if (StepAllowed(STEP_CREATE_DATABASE))
		{
			statsgenDatabase.Create();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_EMPTY_DATABASE);
		if (StepAllowed(STEP_EMPTY_DATABASE))
		{
			statsgenDatabase.Empty();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE);
		if (StepAllowed(STEP_WRITE_DATABASE))
		{
			playerDataAvatars->ReadFromFile();
			playerDataRealName->ReadFromFile();
			playerDataWebsite->ReadFromFile();
			playerDataPicture->ReadFromFile();
			playerDataXfire->ReadFromFile();
			playerDataClanRole->ReadFromFile();
			playerDataMisc1->ReadFromFile();
			playerDataMisc2->ReadFromFile();
			WriteToDatabase();
			statsgenDatabase.WriteMiscellaneousStatistic("starttime",startTime);
			statsgenDatabase.WriteMiscellaneousStatistic("roundsprocessed",
													(int)rounds.GetCount());
		}
		progress->Finalise();

	}

	if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_TRANSFER_DATABASE,allowedSteps))
	{
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_TRANSFER_DATABASE);
		if (StepAllowed(STEP_TRANSFER_DATABASE))
		{
			if (fullRun)
			{
				ExternalDatabase	externalDatabase;
				externalDatabase.TransferFromInternal();
			}
		}
		progress->Finalise();
	}

	if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_READ_TEMPLATE,allowedSteps))
	{
		if (fullRun)
		{
			configKey="/Template/FullRun";
		}
		else
		{
			configKey="/Template/QuickRun";
		}
		configData.ReadTextValue(configKey,&templateFilename,"");

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_READ_TEMPLATE);
		progress->DisableTimeToGo();
		if (StepAllowed(STEP_READ_TEMPLATE))
		{
			result=templateFile.ReadTemplate(templateFilename);
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE);
		progress->DisableTimeToGo();
		if (StepAllowed(STEP_PROCESS_TEMPLATE))
		{
			// Delete the existing message centre values
//			templateFile.Debug();
			templateFile.Process();
		}

		if (StepAllowed(STEP_POST_PROCESS_TEMPLATE))
		{
			PlayerListToPlayerCache();
			WritePlayerCache();
		}

		progress->Finalise();

	}
	if (ProcessStepAllowed(WINDOW_ID_PROGRESS_PANEL_WEBSITE,allowedSteps))
	{
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WEBSITE);
		if (StepAllowed(STEP_WEBSITE))
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Website Transfer Step Allowed");
			// Now time to transfer to the website
			if (fullRun)
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,"Website Transfer Full Run");
				wxArrayString	createdFiles;

				templateFile.GetCreatedFiles(createdFiles);
				website.TransferFiles(createdFiles);
			}
		}
		progress->Finalise();

		// Commit any config changes made during production
		configData.CommitChanges();
		// Reset the message centres with the newly produced stats
		InitiateMessageCentres();
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::CalculateScores()
{
	int				roundCount;
	int				roundIndex;
	Round			currentRound;
	Player			*player;
	wxString		msg;
	int				playerCount;
	int				playerIndex;
	int				killCount;
	int				killIndex;
	KillData		killData;
	int				minRoundIndex;
	int				maxRoundIndex;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","CalculateScores")
	roundCount=rounds.GetCount();
	STATSGEN_DEBUG_CODE(msg.Printf("rounds=%d",roundCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	playerCount=playerList.GetCount();
	progress->Initiate(
						(long)roundCount,
						" Rounds",
						1,
						" Rounds",
						1);

	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		roundsChanged.Add(roundIndex+statsgenDatabase.NextRoundIndex());
	}
	if (roundCount == 0)
	{
		msg="0 rounds found - make sure you have the correct server type set and that you have managed to successfully download logs for this server";
		progress->LogError(msg,SeverityCaution);
	}
	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		STATSGEN_DEBUG_CODE(msg.Printf("roundindex=%d",roundIndex);)
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
		progress->Update((long)roundIndex+1);
		currentRound=rounds.Item(0);
		currentRound.CalculateScores(roundIndex);
		// remove existing round and add it to the end of the list
		// as we keep removing Round 0 it should add them back in order
		rounds.RemoveAt(0);
		rounds.Add(currentRound);
	}

	progress->Finalise();
	progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES2);
	progress->Initiate(
						(long)playerCount,
						" Players",
						1,
						" Players",
						1);
	STATSGEN_DEBUG_CODE(msg.Printf("players=%d",playerCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		progress->Update((long)playerIndex+1);
		player=playerList.Detach(0);
		if (player->actualPlayerIndex < statsgenDatabase.NextPlayerIndex())
		{
			// This player already exists so we need to gather the existing
			// kill totals them from the DB
			statsgenDatabase.UpdatePlayerKillTotals(playerIndex,
													player->actualPlayerIndex,
													killTotalLists.maps,
													killTotalLists.gameTypes,
													killTotalLists.weapons,
													killTotalLists.locations,
													killTotalLists.teams,
													killTotalLists.classes);
			
		}
		player->clanIndex=ClanIndex(player->name);
		if(player->rounds>0)
		{
			player->scoreWeighted=((player->score)/((float)player->rounds));
			player->skillWeighted=((player->skill)/((float)player->rounds));
		}
		else
		{
			player->scoreWeighted=0.0;
			player->skillWeighted=0.0;
		}
		playerList.Add(player);
		minRoundIndex=player->firstRoundPlayed;
		maxRoundIndex=player->lastRoundPlayed;
		if (minRoundIndex<0)
		{
			continue;
		}

		if (minRoundIndex==-1)
		{
			minRoundIndex=0;
		}
		// Calculate Totals for this player
		for (roundIndex=minRoundIndex;
			roundIndex<=maxRoundIndex;roundIndex++)
		{
			currentRound=rounds.Item(roundIndex);
			if (!currentRound.dropped)
			{
				killCount=currentRound.kills.GetCount();
				for (killIndex=0;
					killIndex<killCount;
					killIndex++)
				{
					killData=currentRound.kills.Item(killIndex);
					if ((killData.playerIndex==playerIndex)||
						(killData.targetIndex==playerIndex))
					{
						// This kill is something to do with this player
						/*
						if (roundsAlreadyProcessed==0)
						{
							// prepare for quick add
							KillTotalLists::Add(killData,playerIndex,
											maps,
											gameTypes,
											weapons,
											locations,
											teams,
											classes);
						}
						else
						*/
						{
							// gradual add
							KillTotalLists::Add(killData,playerIndex,
											killTotalLists.maps,
											killTotalLists.gameTypes,
											killTotalLists.weapons,
											killTotalLists.locations,
											killTotalLists.teams,
											killTotalLists.classes);
						}
					}
				}
			}
		}

		// we should now have all the totals for this player
		// add it to the totalslist
		/*
		if (roundsAlreadyProcessed==0)
		{
			// gonna do a quick add
			killTotalLists.Add(
					maps,
					gameTypes,
					weapons,
					locations,
					teams,
					classes);
		}
		*/
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ReadAwardDefinitions()
{
	wxArrayString	awardList;
	wxString		awardID;
	int				awardIndex;
	int				awardCount;
	wxString		status;

	status="Read Award Definitions"; progress->SetStatus(status);
	awardDefinitions.Clear();

	configData.ReadList("AWARDLIST",awardList);
	awardCount=awardList.GetCount();
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		awardID=awardList.Item(awardIndex);
		AwardDefinition	award(awardID);

		awardDefinitions.Add(award);
	}
}

void GlobalStatistics::DropPlayers()
{
	int				roundCount;
	int				roundIndex;
	int				playerInRoundCount;
	int				playerInRoundIndex;
	int				playerIndex;
	int				playerCount;
	Player			player;
	Round			round;
	PlayerInRound	playerInRound;
	wxString		minRoundsPerPlayerString;
	int				minRoundsPerPlayer;
	bool			cheat;
	wxString		msg;
	bool			orgDropped;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","DropPlayers")
	// Minimum players in round is a "Hard" drop
	// i.e. we don't want to count any of these stats
	// at all

	globalStatistics.configData.ReadTextValue(
									"/General/MinRoundsPerPlayer",
									&minRoundsPerPlayerString,
									"1");

	minRoundsPerPlayer=atoi(minRoundsPerPlayerString.GetData());



	/*
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Zero players")
	// zero each players round
	playerCount=playerList.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=playerList.Item(0);
		playerList.RemoveAt(0);
		player.rounds=0;
		playerList.Add(player);
	}
	*/
	// step through each round and add the round count
	// to any player in it's list
	playerCount=playerList.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Count Rounds")
	roundCount=rounds.GetCount();
	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		round=rounds.Item(roundIndex);
		// don't bother counting dropped rounds
		if (!round.dropped)
		{
			playerInRoundCount=round.playersInRound.GetCount();
			for (playerInRoundIndex=0;
				playerInRoundIndex<playerInRoundCount;
				playerInRoundIndex++)
			{
				playerInRound=round.playersInRound.Item(playerInRoundIndex);
				player=playerList.Item(playerInRound.playerIndex);
				playerList.RemoveAt(playerInRound.playerIndex);
				player.rounds++;
				playerList.Insert(player,playerInRound.playerIndex);
			}
		}
	}

	STATSGEN_DEBUG(DEBUG_ALWAYS,"Drop Players")
	// Added all the rounds up now - drop any players that don't meet minimum
	// round count
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=playerList.Item(0);
		playerList.RemoveAt(0);
		orgDropped=player.dropped;
		if (player.name.Cmp(WORLD_PLAYER_NAME)==0)
		{
			player.dropped=true;
		}
		else
		{
		if (player.rounds<=minRoundsPerPlayer)
		{
			STATSGEN_DEBUG_CODE(msg.Printf("player index=%d dropped",playerIndex);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			player.dropped=true;
		}
		else
		{
			//player.dropped=false;
			if ((player.name.Cmp(WORLD_PLAYER_NAME)==0)&&
				(player.ID.Cmp(WORLD_PLAYER_ID)==0))
			{
				player.dropped=true;
			}
			else
			{
				player.dropped=dropList.IsDropped(player.name,&cheat);
			}
		}
		if (player.dropped!=orgDropped)
		{
			// this player was dropped - but now is not -  or vice versa
			if (player.actualPlayerIndex<statsgenDatabase.NextPlayerIndex())
			{
				droppedPlayersChanged.Add(player.actualPlayerIndex);
			}
		}
		}
		playerList.Add(player);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::DropRounds()
{
	int				roundCount;
	int				roundIndex;
	int				playerInRoundCount;
	int				playerInRoundIndex;
	Round			round;
	PlayerInRound	playerInRound;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","DropRounds")
	// Minimum players in round is a "Hard" drop
	// i.e. we don't want to count any of these stats
	// at all

	roundCount=rounds.GetCount();
	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		round=rounds.Item(0);
		rounds.RemoveAt(0);
		round.activePlayers=0;
		playerInRoundCount=round.playersInRound.GetCount();
		for (playerInRoundIndex=0;
			playerInRoundIndex<playerInRoundCount;
			playerInRoundIndex++)
		{
			playerInRound=round.playersInRound.Item(playerInRoundIndex);
			round.activePlayers++;
		}
		if (round.activePlayers==0)
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Round %d dropped",roundIndex);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			round.dropped=true;
		}
		else
		{
			round.dropped=false;
		}
		rounds.Add(round);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::DropInactiveRounds()
{
	int				roundCount;
	int				roundIndex;
	int				playerInRoundCount;
	int				playerInRoundIndex;
	Round			round;
	Player			player;
	PlayerInRound	playerInRound;
	bool			allPlayersDropped=false;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","DropInactiveRounds")
	// Minimum players in round is a "Hard" drop
	// i.e. we don't want to count any of these stats
	// at all

	roundCount=rounds.GetCount();
	for (roundIndex=0;roundIndex<roundCount;roundIndex++)
	{
		round=rounds.Item(0);
		rounds.RemoveAt(0);

		allPlayersDropped=true;
		playerInRoundCount=round.playersInRound.GetCount();

		for (playerInRoundIndex=0;
			playerInRoundIndex<playerInRoundCount;
			playerInRoundIndex++)
		{
			playerInRound=round.playersInRound.Item(playerInRoundIndex);
			player=playerList.Item(playerInRound.playerIndex);
			if (!player.dropped)
			{
				allPlayersDropped=false;
				break;
			}
		}

		if (allPlayersDropped)
		{
			STATSGEN_DEBUG_CODE(msg.Printf("inactive round %d dropped",roundIndex);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			round.dropped=true;
		}
		rounds.Add(round);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ReadServersFromConfig()
{
	wxArrayString	serverIDs;
	wxString		configServerID;
	int				configServerCount;
	int				configServerIndex;
	int				activeServerCount;
	wxString		msg;
	wxString		status;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ReadServersFromConfig")
	status="Updating Servers"; progress->SetStatus(status);
	configData.ReadList("SERVERS",serverIDs);

	// Match up the config server id's to the active server id's
	configServerCount=serverIDs.GetCount();
	activeServerCount=servers.GetCount();
	if (activeServerCount==0)
	{
		for (configServerIndex=0;
			configServerIndex<configServerCount;
			configServerIndex++)
		{
			configServerID=serverIDs.Item(configServerIndex);
			// This config server is new - so add it to the end
			Server	*server=new Server();

			server->SetID(configServerID);
			server->Initiate();
			server->CreateLogFiles();
			servers.Add(server);
		}
	}
	else
	{
		// servers have already been loaded don't do it again
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ReadClanDefinitions()
{
	wxArrayString	clanList;
	wxString		clanID;
	int				clanIndex;
	int				clanCount;
	wxString		status;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ReadClanDefinitions")
	status="Read Clan Definitions"; progress->SetStatus(status);
	clans.Clear();

	configData.ReadList("CLANS",clanList);
	clanCount=clanList.GetCount();
	STATSGEN_DEBUG_CODE(msg.Printf("CLAN COUNT READ=%d",clanCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	for (clanIndex=0;clanIndex<clanCount;clanIndex++)
	{
		clanID=clanList.Item(clanIndex);
		Clan	*clan=new Clan(clanID);

		clans.Add(clan);
	}
	clanCount=clans.GetCount();
	STATSGEN_DEBUG_FUNCTION_END
}

int GlobalStatistics::ClanIndex(wxString &name)
{
	int		clanCount;
	int		clanIndex;
	int		retVal=-1;

	clanCount=clans.GetCount();
	for (clanIndex=0;clanIndex<clanCount;clanIndex++)
	{
		Clan	*clan=clans.Detach(0);
		clans.Add(clan);
		if (retVal==-1)
		{
			if (clan->IsMember(name))
			{
				retVal=clanIndex;
			}
		}
	}

	return (retVal);
}

void GlobalStatistics::ReadScheduleConfig()
{
	ReadMessageTimerConfig();
	ReadQuickRunTimerConfig();
	ReadProductionRunTimerConfig();
}

void GlobalStatistics::ReadMessageTimerConfig()
{
	wxString	configKey;
	wxString	configValue;
	wxString	interval;

	configKey="/RCONSettings/MessageIntervalTimer";
	configData.ReadTextValue(configKey,&interval,"60");
	messageTimer.IntervalStart(interval);
}

void GlobalStatistics::ReadProductionRunTimerConfig()
{
	wxString	configKey;
	wxString	configValue;
	wxString	baseTime;
	wxString	interval;

	configKey="/Schedule/ProductionRun";
	configData.ReadTextValue(configKey,&configValue,"00:00:00,86400");
	baseTime=configValue.BeforeFirst(',');
	interval=configValue.AfterFirst(',');
	productionRunTimer.ScheduledStart(baseTime,interval);
}

void GlobalStatistics::ReadQuickRunTimerConfig()
{
	wxString	configKey;
	wxString	configValue;
	wxString	interval;

	configKey="/Schedule/QuickRun";
	configData.ReadTextValue(configKey,&interval,"0");

	quickRunTimer.IntervalStart(interval);
}

bool GlobalStatistics::StepAllowed(const char *step)
{
	wxString	configKey;
	wxString	configValue;
	bool		stepAllowed;

	configKey.Printf("/Debug/Step%s",step);
	configData.ReadTextValue(configKey,&configValue,"y");
	stepAllowed=(configValue.CmpNoCase("y")==0);

	return (stepAllowed);
}

void GlobalStatistics::ClearServers()
{
	int		serverCount;
	int		serverIndex;
	ResetProcessingStatus();

	serverCount=servers.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		Server *server=(Server *)servers.Item(serverIndex);
		delete server;
	}
	servers.Clear();
}

void GlobalStatistics::ResetProcessingStatus()
{
	int			serverCount;
	int			serverIndex;

	// We need to reset all the servers and lists etc
	// for a full run
	cheaters.Clear();
	playerList.Clear();
	rounds.Clear();
	keysAction.Clear();
	keysAmmo.Clear();
	keysMap.Clear();
	keysWeapon.Clear();
	keysClass.Clear();
	keysTeam.Clear();
	keysGametype.Clear();
	keysLocation.Clear();
	keysXP.Clear();
	killTotalLists.Clear();
	totalXP.Clear();
	AKAList.Clear();
	skillWeights.Clear();
	scoreWeights.Clear();
	playersChanged.Clear();
	droppedPlayersChanged.Clear();
	roundsChanged.Clear();

	serverCount=servers.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		//Server			*server=servers.Detach(0);
		Server			*server=(Server *)servers.Item(serverIndex);

		server->StartFromBeginning();
		//servers.Add(server);
	}
}

void GlobalStatistics::InitiateMessageCentres()
{
	int		serverCount;
	int		serverIndex;

	serverCount=servers.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		//Server			*server=servers.Detach(0);
		Server			*server=(Server *)servers.Item(serverIndex);

		server->InitiateMessageCentre();
		//servers.Add(server);
	}
}

void GlobalStatistics::SendMessages()
{
	int			serverCount;
	int			serverIndex;
	static		bool	firstTimeRun=true;

	if (firstTimeRun)
	{
		InitiateMessageCentres();
		firstTimeRun=false;
	}
	serverCount=servers.GetCount();
	for (serverIndex=0;serverIndex<serverCount;serverIndex++)
	{
		//Server		*server=servers.Detach(0);
		Server		*server=(Server *)servers.Item(serverIndex);

		server->SendMessage();
		//servers.Add(server);
	}
}

void GlobalStatistics::ReadTeamSwapWeapons()
{
	int			teamSwapCount;
	wxString	teamSwap;

	configData.ReadList("TEAMSWAP",teamSwapWeapons);
	teamSwapCount=teamSwapWeapons.GetCount();
	if (teamSwapCount==0)
	{
		teamSwap="COD1_MOD_SUICIDE";teamSwapWeapons.Add(teamSwap);
		teamSwap="COD2_MOD_SUICIDE";teamSwapWeapons.Add(teamSwap);
		configData.WriteList("TEAMSWAP",teamSwapWeapons);
	}
}

void GlobalStatistics::ReadDeathmatchGametypes()
{
	int			gametypeCount;
	wxString	gametype;

	configData.ReadList("DEATHMATCH",deathmatchGametypes);
	gametypeCount=deathmatchGametypes.GetCount();
	if (gametypeCount==0)
	{
		gametype="COD1_dm";deathmatchGametypes.Add(gametype);
		gametype="COD2_dm";deathmatchGametypes.Add(gametype);
		configData.WriteList("DEATHMATCH",deathmatchGametypes);
	}
}

bool GlobalStatistics::IsDeathmatchGametype(wxString &gametypeSearch)
{
	int			gametypeCount;
	int			gametypeIndex;
	wxString	gametype;
	bool		retVal=false;

	gametypeCount=deathmatchGametypes.GetCount();
	for (gametypeIndex=0;gametypeIndex<gametypeCount;gametypeIndex++)
	{
		gametype=deathmatchGametypes.Item(gametypeIndex);
		if (gametype.CmpNoCase(gametypeSearch)==0)
		{
			retVal=true;
			break;
		}
	}

	return (retVal);
}

bool GlobalStatistics::IsDeathmatchGametype(int gametypeInt)
{
	GenericKey		keyGametype;
	bool			retVal;

	keyGametype=keysGametype.Item(gametypeInt);

	retVal=IsDeathmatchGametype(keyGametype.id);

	return (retVal);
}

float GlobalStatistics::GametypeScoreWeight(wxString &gametypeSearch)
{
	wxString	scoreWeightKey;

	scoreWeightKey=gametypeSearch;

	return (ScoreWeight(scoreWeightKey,1.0));
}

float GlobalStatistics::GametypeScoreWeight(int gametypeInt)
{
	GenericKey		keyGametype;
	bool			retVal;
	wxString		gametypeSearch;

	keyGametype=keysGametype.Item(gametypeInt);
	gametypeSearch="gametype_";
	gametypeSearch+=keyGametype.id;
	return (GametypeScoreWeight(gametypeSearch));
}

float GlobalStatistics::GametypeSkillWeight(wxString &gametypeSearch)
{
	wxString	skillWeightKey;

	skillWeightKey=gametypeSearch;

	return (SkillWeight(skillWeightKey,1.0));
}

float GlobalStatistics::GametypeSkillWeight(int gametypeInt)
{
	GenericKey		keyGametype;
	bool			retVal;
	wxString		gametypeSearch;

	keyGametype=keysGametype.Item(gametypeInt);
	gametypeSearch="gametype_";
	gametypeSearch+=keyGametype.id;
	return (GametypeSkillWeight(gametypeSearch));
}
bool GlobalStatistics::IsTeamSwapWeapon(wxString &weaponSearch)
{
	int			teamSwapCount;
	int			teamSwapIndex;
	wxString	weapon;
	bool		retVal=false;

	teamSwapCount=teamSwapWeapons.GetCount();
	for (teamSwapIndex=0;teamSwapIndex<teamSwapCount;teamSwapIndex++)
	{
		weapon=teamSwapWeapons.Item(teamSwapIndex);
		if (weapon.CmpNoCase(weaponSearch)==0)
		{
			retVal=true;
			break;
		}
	}

	return (retVal);
}

bool GlobalStatistics::IsTeamSwapWeapon(int weaponInt)
{
	GenericKey		keyWeapon;
	bool			retVal;

	keyWeapon=keysWeapon.Item(weaponInt);

	retVal=IsTeamSwapWeapon(keyWeapon.id);

	return (retVal);
}

void GlobalStatistics::OpenSpeechFile()
{
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","OpenSpeechFile")
	wxString filename;

	speechfp=NULL;
	configData.ReadTextValue(CONFIG_KEY_SPEECHFILE,&filename);
	if (filename.Length()>0)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Speech File Opened")
		speechfp=fopen(filename.GetData(),"a");
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::CloseSpeechFile()
{
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","CloseSpeechFile")
	if (speechfp!=NULL)
	{
		fclose(speechfp);
	}
	speechfp=NULL;
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::WriteSpeechFile(
					wxDateTime &speechTime,
					wxString &guid,
					wxString &playername,
					wxString &speech
					)
{
	wxString	timeString;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","WriteSpeechFile")
	
	timeString=speechTime.Format("%d/%m/%Y %H:%M:%S");
	if (speechfp!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,speech)
		fprintf(speechfp,"%s %32s (%s): %s\n",
				timeString.GetData(),
				playername.GetData(),
				guid.GetData(),
				speech.GetData());
	}
	STATSGEN_DEBUG_FUNCTION_END
}

wxString GlobalStatistics::SkillCalculationTypeConfigKey()
{
	wxString configKey="/General/AllowNegativeSkill";

	return (configKey);
}

void GlobalStatistics::InitiateSkillCalculationType()
{
	wxString	configKey;
	wxString	configValue;

	configKey=SkillCalculationTypeConfigKey();

	configData.ReadTextValue(configKey,&configValue,"y");
	negativeSkillAllowed=(configValue.CmpNoCase("y")==0);
}

bool GlobalStatistics::AllowNegativeSkill()
{
	return (negativeSkillAllowed);
}

void GlobalStatistics::GenerateUpgrade(wxString &filename)
{
	wxFileName		iniFilename(filename);
	ConfigData		upgradeConfigData;
	wxArrayString	awardIDs;
	wxString		awardID;
	wxString		description;
	int				awardCount;
	int				awardIndex;
	wxString		awardKey;
	wxString		configKey;
	wxArrayString	upgradeList;
	wxString		configValue;

	upgradeList.Clear();
	upgradeConfigData.Initiate(iniFilename);
	configKey="/UPGRADELIST/Description";
	configValue.Printf("Full Upgrade File for Statsgen Version %s",PROGRAM_VERSION);
	upgradeConfigData.WriteTextValue(configKey,configValue);

	configKey="/UPGRADELIST/UpgradeType";
	configValue=UPGRADE_TYPE_CONFIG;
	upgradeConfigData.WriteTextValue(configKey,configValue);

	configData.ReadList("AWARDLIST",awardIDs);
	awardCount=awardIDs.GetCount();
	upgradeConfigData.Export(upgradeList,configData,"Which Gametypes Are Deathmatch","DEATHMATCH",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Overwrite");
	upgradeConfigData.Export(upgradeList,configData,"Which Weapons Are Team Swap","TEAMSWAP",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Overwrite");
	upgradeConfigData.Export(upgradeList,configData,"MOHAA Line Identification","LINEIDMOHAA",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Overwrite");
	upgradeConfigData.Export(upgradeList,configData,"Wolfenstein Line Identification","LINEIDWOLFENSTEIN",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Overwrite");
	upgradeConfigData.Export(upgradeList,configData,"Which GUIDs should not be auto aliased","AutoAliasBlockedGUIDS",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Overwrite");
	upgradeConfigData.Export(upgradeList,configData,"RCON Message Formats","MessageCentreFormats",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Award Definitions","AWARDLIST",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	for (awardIndex=0;awardIndex<awardCount;awardIndex++)
	{
		awardID=awardIDs.Item(awardIndex);
		awardKey="AWARDDEFINITION"+awardID;
		configKey="/"+awardKey+"/Name";
		configData.ReadTextValue(configKey,&description);
		description="Award - "+description;
		upgradeConfigData.Export(upgradeList,configData,
					(char *)description.GetData(),
					(char *)awardKey.GetData(),
					CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_USE_LAST,"");
		
	}

	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Game Types","LogSubstituteGAMETYPE",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Maps","LogSubstituteMAP",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Teams","LogSubstituteTEAM",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Weapons","LogSubstituteWEAPON",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Ammo","LogSubstituteAMMO",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Weapons & Ammo","LogSubstituteWEAPONAMMO",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Locations","LogSubstituteLOCATION",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Objectives","LogSubstituteACTION",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Logfile Substitution - Player Class","LogSubstituteCLASS",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Do Nothing");
	upgradeConfigData.Export(upgradeList,configData,"Log Entries - Objectives","LogEntriesACTION",CONFIG_EXPORT_ALL,CONFIG_ANSWER_TYPE_ASK,"Update");
	upgradeConfigData.Export(upgradeList,configData,"Log Entries - Weapons","LogEntriesWEAPON",CONFIG_EXPORT_ALL,CONFIG_ANSWER_TYPE_ASK,"Update");
	upgradeConfigData.Export(upgradeList,configData,"Log Entries - Locations","LogEntriesLOCATION",CONFIG_EXPORT_ALL,CONFIG_ANSWER_TYPE_ASK,"Update");
	upgradeConfigData.Export(upgradeList,configData,"Score Weights","ScoreWeights",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Update");
	upgradeConfigData.Export(upgradeList,configData,"Skill Weights","SkillWeights",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Update");
	upgradeConfigData.Export(upgradeList,configData,"Image References","Images",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Update");
	upgradeConfigData.Export(upgradeList,configData,"Real Names","RealNames",CONFIG_EXPORT_SET_ONLY,CONFIG_ANSWER_TYPE_ASK,"Update");
	wxArrayString	weaponGroups;
	wxString		weaponGroup;
	int				weaponGroupCount;
	int				weaponGroupIndex;
	wxString		weaponGroupDescription;
	weaponGroups=configData.ReadWeaponGroups();
	weaponGroupCount=weaponGroups.GetCount();
	for (weaponGroupIndex=0;weaponGroupIndex<weaponGroupCount;weaponGroupIndex++)
	{
		weaponGroup=weaponGroups.Item(weaponGroupIndex);
		weaponGroupDescription.Printf("Weapon Group - %s",weaponGroup.GetData());
		upgradeConfigData.Export(upgradeList,configData,
							(char *)weaponGroupDescription.GetData(),
							(char *)weaponGroup.GetData(),
							CONFIG_EXPORT_ALL,CONFIG_ANSWER_TYPE_ASK,"Overwrite");
	}
	upgradeConfigData.WriteList("UPGRADELIST",upgradeList);
	upgradeConfigData.CommitChanges();
}

void GlobalStatistics::AddAKA(int playerIndex,wxString &name)
{
	int				AKACount;
	AKAListEntry	AKAEntry;
	int				AKAIndex;
	bool			AKAFound=false;

	AKACount=AKAList.GetCount();
	for (AKAIndex=0;AKAIndex<AKACount;AKAIndex++)
	{
		AKAEntry=AKAList.Item(AKAIndex);
		if ((AKAEntry.playerIndex==playerIndex)&&
			(AKAEntry.name.Cmp(name)==0))
		{
			AKAFound=true;
			break;
		}
	}
	if (!AKAFound)
	{
		AKAEntry.playerIndex=playerIndex;
		AKAEntry.name=name;
		AKAList.Add(AKAEntry);
	}
}

float GlobalStatistics::ScoreWeight(const char *key,float defaultValue)
{
	wxString	keyStr=key;

	return (ScoreWeight(keyStr,defaultValue));
}

float GlobalStatistics::ScoreWeight(const char *key,
				const char*prefix,
				float defaultValue)
{
	wxString	keyStr;

	keyStr.Printf("%s_%s",prefix,key);
	return (ScoreWeight(keyStr,defaultValue));
}

float GlobalStatistics::ScoreWeight(wxString &key,float defaultValue)
{
	wxString	keyStr=key;

	return (scoreWeights.Weight(keyStr,defaultValue));
}

float GlobalStatistics::SkillWeight(const char *key,float defaultValue)
{
	wxString	keyStr=key;

	return (SkillWeight(keyStr,defaultValue));
}

float GlobalStatistics::SkillWeight(const char *key,
				const char*prefix,
				float defaultValue)
{
	wxString	keyStr;

	keyStr.Printf("%s_%s",prefix,key);
	return (SkillWeight(keyStr,defaultValue));
}

float GlobalStatistics::SkillWeight(wxString &key,float defaultValue)
{
	wxString	keyStr=key;

	return (skillWeights.Weight(keyStr,defaultValue));
}

int GlobalStatistics::ReadPriority(bool fullRun)
{
	wxString	configKey;
	wxString	configValue;

	if (fullRun)
	{
		configKey="/General/FullRunPriority";
	}
	else
	{
		configKey="/General/QuickRunPriority";
	}

	configData.ReadTextValue(configKey,&configValue,"50");
	return (atoi(configValue.GetData()));
}

void GlobalStatistics::WritePriority(bool fullRun,int priority)
{
	wxString	configKey;
	wxString	configValue;

	if (fullRun)
	{
		configKey="/General/FullRunPriority";
	}
	else
	{
		configKey="/General/QuickRunPriority";
	}
	configValue.Printf("%d",priority);

	configData.WriteTextValue(configKey,configValue);
}

bool GlobalStatistics::ProcessStepAllowed(int step,wxArrayInt &allowedSteps)
{
	int		stepCount;
	int		stepIndex;
	int		thisStep;
	bool	stepAllowed=false;

	stepCount=allowedSteps.GetCount();
	for (stepIndex=0;stepIndex<stepCount;stepIndex++)
	{
		thisStep=allowedSteps.Item(stepIndex);
		if (thisStep==step)
		{
			stepAllowed=true;
			break;
		}
	}

	return (stepAllowed);
}

wxString GlobalStatistics::AutomaticAliasConfigKey()
{
	wxString	retVal;

	retVal="/General/AutomaticAlias";

	return (retVal);
}

void GlobalStatistics::ProduceStatsInitiate()
{
	wxString	configKey;
	wxString	configValue;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ProduceStatsInitiate")
	FlagStartOfRun();
	configData.OpenWeaponGroupCache();

	startTime=time(&startTime);
	statsgenDatabase.OpenDB();

	recoveryMode=progress->RecoveryMode();
	if (!recoveryMode)
	{
		progress->ClearErrors();
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_GENERAL);

		configKey=AutomaticAliasConfigKey();
		configData.ReadTextValue(configKey,&configValue,"n");
		useAutomaticAliases=(configValue.CmpNoCase("y")==0);



		// Make sure the group gets deleted
		configData.DeleteGroup("MessageCentreValues");
		configData.CommitChanges();

		configData.ReadList("BannedSpeech",bannedSpeech);

		ReadDeathmatchGametypes();
		ReadTeamSwapWeapons();
		skillWeights.Initiate("SkillWeights");
		scoreWeights.Initiate("ScoreWeights");

		OpenSpeechFile();
		ReadAliasList();
		ReadAwardDefinitions();
		ReadClanDefinitions();
		dropList.Load();

		InitiateSkillCalculationType();

		ClearServers();
		ReadServersFromConfig();
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_CREATE_DATABASE);
		if (StepAllowed(STEP_CREATE_DATABASE))
		{
			statsgenDatabase.Create();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_APPLY_ALIASES);
		if (StepAllowed(STEP_APPLY_ALIASES))
		{
			statsgenDatabase.ApplyAliases(playersChanged);
			UpdateExistingPlayersClan();
		}
		progress->Finalise();
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ProduceStatsDownload()
{
	int		serverCount;
	int		serverIndex;
	Server	*server;
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ProduceStatsDownload")

	if (!recoveryMode)
	{
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS);
		if (StepAllowed(STEP_DOWNLOAD_LOGS))
		{
			serverCount=servers.GetCount();
			for (serverIndex=0;serverIndex<serverCount;serverIndex++)
			{
				server=(Server *)servers.Item(serverIndex);
				server->DownloadBanFiles();
				server->DownloadFiles();
			}
		}
		progress->Finalise();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ProduceStatsProcess()
{
	int				serverCount;
	int				serverIndex;
	Server			*server;
	wxString		configKey;
	wxString		templateFilename;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ProduceStatsProcess")

	if (!recoveryMode)
	{
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DATABASE_READ_STATIC);
		if (StepAllowed(STEP_DATABASE_READ_STATIC))
		{
			ReadStaticItemsFromDatabase();
			statsgenDatabase.UpdatePlayerCount();
			statsgenDatabase.UpdateRoundCount();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_EMPTY_DATABASE);
		if (StepAllowed(STEP_EMPTY_DATABASE))
		{
			statsgenDatabase.Empty();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS);
		if (StepAllowed(STEP_PROCESS_LOG_FILES))
		{
			serverCount=servers.GetCount();

			playerList.EnableCache();
			aliasCacheEnabled=true;

			for (serverIndex=0;serverIndex<serverCount;serverIndex++)
			{
				server=(Server *)servers.Item(serverIndex);
				server->ProcessBanFiles();
				server->FindLogfiles();
				server->CreateLogFiles();
				server->AdjustLogfilePosition();
			}
			for (serverIndex=0;serverIndex<serverCount;serverIndex++)
			{
				server=(Server *)servers.Item(serverIndex);
				server->ProcessLogFiles(serverIndex);
			}
			playerList.DisableCache();
			aliasCacheEnabled=false;
			if (useAutomaticAliases)
			{
				WriteAliasList();
			}
			CloseSpeechFile();
			dropList.Write();
		}
		progress->Finalise();
	
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DATABASE_LOCATE_EXISTING_PLAYERS);
		if (StepAllowed(STEP_DATABASE_LOCATE_EXISTING_PLAYERS))
		{
			LocateExistingPlayers();
		}
		progress->Finalise();
		// By now all the stats have been gathered together - we
		// no longer need the server objects
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DROPPING_PLAYERS);
		if (StepAllowed(STEP_DROP_PLAYERS))
		{
			// drop players who have not played enough rounds
			DropPlayersAndRounds();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES1);
		if (StepAllowed(STEP_CALCULATE_SCORES))
		{
			CalculateScores();
		}
		progress->Finalise();

		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE);
		if (StepAllowed(STEP_WRITE_DATABASE))
		{
			playerDataAvatars->ReadFromFile();
			playerDataRealName->ReadFromFile();
			playerDataWebsite->ReadFromFile();
			playerDataPicture->ReadFromFile();
			playerDataXfire->ReadFromFile();
			playerDataClanRole->ReadFromFile();
			playerDataMisc1->ReadFromFile();
			playerDataMisc2->ReadFromFile();
			WriteToDatabase();
			statsgenDatabase.WriteMiscellaneousStatistic("starttime",startTime);
			statsgenDatabase.WriteMiscellaneousStatistic("roundsprocessed",
													(int)rounds.GetCount());
			statsgenDatabase.Optimize();
		}
		progress->Finalise();
	}

	configKey="/Template/FullRun";
	configData.ReadTextValue(configKey,&templateFilename,"");

	progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_READ_TEMPLATE);
	progress->DisableTimeToGo();
	if (StepAllowed(STEP_READ_TEMPLATE))
	{
		templateFile.ReadTemplate(templateFilename);
	}
	progress->Finalise();

	progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE);
	progress->DisableTimeToGo();
	if (StepAllowed(STEP_PROCESS_TEMPLATE))
	{
		templateFile.Process();
	}

	if (StepAllowed(STEP_POST_PROCESS_TEMPLATE))
	{
		PlayerListToPlayerCache();
		WritePlayerCache();
	}

	progress->Finalise();
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ProduceStatsTransfer()
{
	Website	website;
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ProduceStatsTransfer")

	progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_TRANSFER_DATABASE);
	if (StepAllowed(STEP_TRANSFER_DATABASE))
	{
		ExternalDatabase	externalDatabase;
		externalDatabase.TransferFromInternal();
	}
	progress->Finalise();

	progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_WEBSITE);
	if (StepAllowed(STEP_WEBSITE))
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Website Transfer Step Allowed");
		// Now time to transfer to the website
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Website Transfer Full Run");
		wxArrayString	createdFiles;

		templateFile.GetCreatedFiles(createdFiles);
		website.TransferFiles(createdFiles);
	}
	progress->Finalise();
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ProduceStatsFinalise()
{
	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","ProduceStatsFinalise")
	configData.CommitChanges();
	// Reset the message centres with the newly produced stats
	ClearServers();
	ReadServersFromConfig();
	InitiateMessageCentres();
	configData.CloseWeaponGroupCache();
	FlagEndOfRun();
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::ReadGenericKeyFromDatabase(ArrayOfGenericKey &keyList,
													const char *tableName)
{
	TemplateOpenQuery	query;
	wxString			sql;
	wxString			property;
	wxString			id;
	GenericKey			key;

	keyList.Clear();
	property="id";

	sql.Printf("select * from %s order by idx asc", tableName);
	query.Initiate(sql,statsgenDatabase.DBHandle());
	while (query.NextRow())
	{
		id=query.RetrieveProperty(property);
		key.id=id;
		keyList.Add(key);
	}
}

void GlobalStatistics::ReadStaticItemsFromDatabase()
{
	ReadGenericKeyFromDatabase(keysAction,		TABLE_NAME_KEY_ACTION);
	ReadGenericKeyFromDatabase(keysAmmo,		TABLE_NAME_KEY_AMMO);
	ReadGenericKeyFromDatabase(keysMap,			TABLE_NAME_KEY_MAP);
	ReadGenericKeyFromDatabase(keysWeapon,		TABLE_NAME_KEY_WEAPON);
	ReadGenericKeyFromDatabase(keysClass,		TABLE_NAME_KEY_CLASS);
	ReadGenericKeyFromDatabase(keysTeam,		TABLE_NAME_KEY_TEAM);
	ReadGenericKeyFromDatabase(keysGametype,	TABLE_NAME_KEY_GAMETYPE);
	ReadGenericKeyFromDatabase(keysLocation,	TABLE_NAME_KEY_LOCATION);
	ReadGenericKeyFromDatabase(keysXP,			TABLE_NAME_KEY_XP);
	scoreWeights.ReadFromDatabase();
	skillWeights.ReadFromDatabase();
}

void GlobalStatistics::LocateExistingPlayers()
{
	int					playerCount;
	int					playerIndex;
	Player				*player;
	int					databasePlayerCount;
	int					newPlayerIndex;
	wxString			msg;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","LocateExistingPlayers")

	databasePlayerCount=statsgenDatabase.NextPlayerIndex();
	newPlayerIndex=databasePlayerCount;
	playerCount=playerList.GetCount();
	msg.Printf("db count=%d playercount=%d",databasePlayerCount,playerCount);
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	progress->Initiate(playerCount," players", 1, "players ", 1);
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		progress->Update(playerIndex);
		msg.Printf("playerindex=%d newplayerIndex=%d",playerIndex,newPlayerIndex);
		STATSGEN_DEBUG(DEBUG_RARELY,msg);
		player=playerList.Detach(0);
		if (!player->UpdateFromDatabase(newPlayerIndex))
		{
			STATSGEN_DEBUG(DEBUG_RARELY,"not from db") 
			newPlayerIndex++;
		}
		playersChanged.Add(player->actualPlayerIndex);
		playerList.Add(player);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::DropExistingPlayers()
{
	wxArrayInt			newlyDroppedPlayers;
	wxArrayInt			newlyInstatedPlayers;
	wxString			sql;
	TemplateOpenQuery	query;
	wxString			propertyName="name";
	wxString			propertyRounds="rounds";
	wxString			propertyPlayerIndex="playerindex";
	wxString			propertyDropped="dropped";
	wxString			name;
	int					rounds;
	int					playerIndex;
	bool				dropped;
	bool				dbDropped;
	Player				player;
	wxString			minRoundsPerPlayerString;
	int					minRoundsPerPlayer;
	bool				cheat;
	wxString			msg;
	int					worldPlayerIndex=-1;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","DropExistingPlayers")
	name=WORLD_PLAYER_NAME;
	sql.Printf("update player set dropped='Y' where name='%s'",StatsgenDatabase::SafeForInsert(name).GetData());
	statsgenDatabase.SimpleExecute(sql);
	globalStatistics.configData.ReadTextValue(
									"/General/MinRoundsPerPlayer",
									&minRoundsPerPlayerString,
									"1");

	minRoundsPerPlayer=atoi(minRoundsPerPlayerString.GetData());

	sql="select * from player";

	query.Initiate(sql,statsgenDatabase.DBHandle());
	while (query.NextRow())
	{
		name=query.RetrieveProperty(propertyName);
		if (name.Cmp(WORLD_PLAYER_NAME)==0)
		{
			continue;
		}
		cheat=false;
		rounds=atoi(query.RetrieveProperty(propertyRounds).GetData());
		dbDropped=(query.RetrieveProperty(propertyDropped).CmpNoCase("Y")==0);
		playerIndex=atoi(query.RetrieveProperty(propertyPlayerIndex).GetData());
		player.name=name;
		player.rounds=rounds;
		dropped=(rounds<=minRoundsPerPlayer) || dropList.IsDropped(name,&cheat);
		msg.Printf(
			"dbDropped=[%d],dropped=[%d],playerIndex=[%d],name=[%s],rounds=[%d],minRounds=[%d]",
					dbDropped,
					dropped,
					playerIndex,
					name.GetData(),
					rounds,
					minRoundsPerPlayer);
STATSGEN_DEBUG(DEBUG_RARELY,msg)
		if (cheat)
		{
			// This existing player has been classified as a cheat
			// We need to drop existing cheats complete from the stats
			// Adjusting the scores of anybody affected by the cheats
			DropExistingCheat(playerIndex,worldPlayerIndex);
			cheaters.Add(name);
		}
		else
		{
			if (dropped != dbDropped)
			{
				// this player was dropped but isn't now - or vice versa
				if (dropped)
				{
					newlyDroppedPlayers.Add(playerIndex);
				}
				else
				{
					newlyInstatedPlayers.Add(playerIndex);
				}
			}
		}
	}
	UpdateExistingPlayersDropStatus(newlyDroppedPlayers,true);
	UpdateExistingPlayersDropStatus(newlyInstatedPlayers,false);
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::UpdateExistingPlayersClan()
{
	wxString			propertyClan="clanindex";
	wxString			propertyName="name";
	wxString			propertyPlayerIndex="playerindex";
	wxString			sql;
	TemplateOpenQuery	query;
	int					existingClanIndex;
	int					existingPlayerIndex;
	int					newClanIndex;
	wxString			existingName;

	sql="select name,playerIndex,clanindex from player";

	query.Initiate(sql,statsgenDatabase.DBHandle());
	while (query.NextRow())
	{
		existingClanIndex=atoi(query.RetrieveProperty(propertyClan).GetData());
		existingName=query.RetrieveProperty(propertyName);
		newClanIndex=ClanIndex(existingName);
		if (newClanIndex!=existingClanIndex)
		{
			existingPlayerIndex=atoi(query.RetrieveProperty(propertyPlayerIndex).GetData());
			sql.Printf("update player set clanindex=%d where playerindex=%d",
						newClanIndex,existingPlayerIndex);
			statsgenDatabase.SimpleExecute(sql);
			playersChanged.Add(existingPlayerIndex);
		}
	}
}

void GlobalStatistics::UpdateExistingPlayersDropStatus(
							wxArrayInt &playerIndexList,
							bool dropped)
{
	char		dropChar;
	int			playerIndexCount;
	int			playerListIndex;
	int			playerIndex;
	wxString	sql;
	int			playersChangedCount;
	int			playersChangedIndex;
	bool		alreadyAdded;

	STATSGEN_DEBUG_FUNCTION_START("GlobalStatistics","UpdateExistingPlayersDropStatus")
	if (dropped)
	{
		dropChar='Y';
	}
	else
	{
		dropChar='N';
	}

	playerIndexCount=playerIndexList.GetCount();
	for (playerListIndex=0;playerListIndex<playerIndexCount;playerListIndex++)
	{
		playerIndex=playerIndexList.Item(playerListIndex);
		sql.Printf("update player set dropped='%c' where playerindex=%d",
					dropChar,playerIndex);
		statsgenDatabase.SimpleExecute(sql);
		alreadyAdded=false;
		playersChangedCount=droppedPlayersChanged.GetCount();
		for (playersChangedIndex=0;playersChangedIndex<playersChangedCount;playersChangedIndex++)
		{
			if (droppedPlayersChanged.Item(playersChangedIndex)==playerIndex)
			{
				alreadyAdded=true;
				break;
			}
		}
		if (!alreadyAdded)
		{
			droppedPlayersChanged.Add(playerIndex);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void GlobalStatistics::DropExistingCheat(int playerIndex,int worldPlayerIndex)
{
	// This playerIndex player needs zapping from the database
	// as they have subsequently been identified as cheats
	static wxArrayString	tables;
	static wxArrayString	fields;
	static int				tableCount=0;						
	wxString				table;
	wxString				field;
	int						tableIndex=0;
	wxString				SQL;

	if (tableCount==0)
	{
		table="actiondata";			field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="akadata";			field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="awardpoints";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="killtotalclasses";	field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="killtotalgametypes";	field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="killtotallocations";	field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="killtotalmaps";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="killtotalteams";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="killtotalweapons";	field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="player";				field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="playerinround";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="speechdata";			field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="streakdata";			field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="teamlossdata";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="teamwindata";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="xppoints";			field="playerindex";
		tables.Add(table);			fields.Add(field);

		table="xptotalpoints";		field="playerindex";
		tables.Add(table);			fields.Add(field);

		tableCount=tables.GetCount();
	}

	for (tableIndex=0;tableIndex<tableCount;tableIndex++)
	{
		table=tables.Item(tableIndex);
		field=fields.Item(tableIndex);
		SQL.Printf("delete from %s where %s=%d",
					table.GetData(),
					field.GetData(),
					playerIndex);
		statsgenDatabase.SimpleExecute(SQL);
	}

	// Now we have to delete the kill data associated with this cheater
	// Basic delete for now - improve later to adjust victims scores and skill
	SQL.Printf("delete from killdata where playerindex=%d",playerIndex);
	statsgenDatabase.SimpleExecute(SQL);
	SQL.Printf("update killdata set targetindex=%d where targetindex=%d",
				worldPlayerIndex,playerIndex);
	statsgenDatabase.SimpleExecute(SQL);
}


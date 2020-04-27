// Statsgen Includes
#include "PlayerList.h"
#include "ErrorData.h"
#include "AliasListEntry.h"
#include "GlobalStatistics.h"


PlayerList::PlayerList()
{
	cacheEnabled=false;
	Clear();
	nameCache=NULL;
	nameCache=new ArrayOfKeyedIndex(KeyedIndex::Compare);
	guidCache=NULL;
	guidCache=new ArrayOfKeyedIndex(KeyedIndex::Compare);
}

PlayerList::~PlayerList()
{
	Clear();
	if (nameCache!=NULL)
	{
		delete nameCache;
	}
	if (guidCache!=NULL)
	{
		delete guidCache;
	}
}

void PlayerList::Clear()
{
	players.Clear();
	if (nameCache!=NULL)
	{
		int			cacheCount;
		int			cacheIndex;
		KeyedIndex	*cacheEntry;

		cacheCount=nameCache->GetCount();
		for (cacheIndex=0;cacheIndex<cacheCount;cacheIndex++)
		{
			cacheEntry=nameCache->Item(cacheIndex);
			delete cacheEntry;
		}
		nameCache->Clear();
	}
	if (guidCache!=NULL)
	{
		int			cacheCount;
		int			cacheIndex;
		KeyedIndex	*cacheEntry;

		cacheCount=guidCache->GetCount();
		for (cacheIndex=0;cacheIndex<cacheCount;cacheIndex++)
		{
			cacheEntry=guidCache->Item(cacheIndex);
			delete cacheEntry;
		}
		guidCache->Clear();
	}
}

void PlayerList::EnableCache()
{
	cacheEnabled=true;
}

void PlayerList::DisableCache()
{
	cacheEnabled=false;
}

Player &PlayerList::GetPlayer(int index)
{
	return (players.Item(index));
}
int PlayerList::FindPlayerIndex(wxString &name)
{
	int			playerCount;
	int			playerIndex;
	Player		player;
	int			foundIndex=-1;
	int			searchIndex;
	wxString	msg;

	if (cacheEnabled)
	{
		KeyedIndex		searchKey;
		searchKey.key=name;
		searchIndex=nameCache->Index(&searchKey);
		if (searchIndex==wxNOT_FOUND)
		{
			foundIndex=-1;
		}
		else
		{
			KeyedIndex	*foundKey;
			foundKey=nameCache->Item(searchIndex);
			foundIndex=foundKey->index;
		}
	}
	else
	{
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
	}

	return(foundIndex);
}
int PlayerList::AddPlayer(wxString &name,wxString &id)
{
	Player					player;
	int						playerIndex=-1;
	int						guidIndex=-1;
	AliasListEntry			alias;
	wxString				AKAName;
	bool					AKAFound;
	int						aliasCount;
	int						aliasIndex;
	bool					aliasFound;
	wxString				msg;
	static bool				blockedAutoAliasGUIDRead=false;
	static wxArrayString	blockedAutoAliasGUIDs;

	if (!blockedAutoAliasGUIDRead)
	{
		blockedAutoAliasGUIDRead=true;
		globalStatistics.configData.ReadList((char *)"AutoAliasBlockedGUIDS",
											blockedAutoAliasGUIDs);
		if (blockedAutoAliasGUIDs.GetCount()==0)
		{
			wxString	blockedGUID;
			blockedGUID="COD2_0";blockedAutoAliasGUIDs.Add(blockedGUID);
			blockedGUID="MOHAA_";blockedAutoAliasGUIDs.Add(blockedGUID);
			blockedGUID="WOLFENSTEIN_";blockedAutoAliasGUIDs.Add(blockedGUID);
			blockedGUID="QUAKEWARS_0";blockedAutoAliasGUIDs.Add(blockedGUID);
			globalStatistics.configData.WriteList((char *)"AutoAliasBlockedGUIDS",
											blockedAutoAliasGUIDs);
		}
	}
	alias.aliasGUID=id;
	alias.aliasName=name;
	if (globalStatistics.FindAlias(alias))
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
		if (globalStatistics.useAutomaticAliases)
		{
			// auto aliasing is enabled and this is not
			// already an alias
			if (blockedAutoAliasGUIDs.Index(id)==wxNOT_FOUND)
//			if (id.Cmp("COD2_0")!=0)
			{
				// this is not the 0 guid bug for cod2
				// does this guid already exist in a current
				// alias list .. if so we need to add it
				aliasCount=globalStatistics.aliasList.GetCount();
				aliasFound=false;
				for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
				{
					alias=globalStatistics.aliasList.Item(aliasIndex);
					if (id.Cmp(alias.primaryGUID)==0)
					{
						// found a matching one
						// add it to the list
						aliasFound=true;
						alias.aliasGUID=id;
						alias.aliasName=name;
						globalStatistics.AddAlias(alias);
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
							globalStatistics.AddAlias(alias);
							globalStatistics.AddAKA(guidIndex,name);
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
		if (cacheEnabled)
		{
			int			searchIndex;
			KeyedIndex	*nameCacheEntry;
			nameCacheEntry=new KeyedIndex;
			nameCacheEntry->key=name;
			nameCacheEntry->index=playerIndex;
			nameCache->Add(nameCacheEntry);

			KeyedIndex		searchKey;
			searchKey.key=id;
			searchIndex=guidCache->Index(&searchKey);
			if (searchIndex==wxNOT_FOUND)
			{
				// this guid not in cache
				KeyedIndex	*guidCacheEntry;
				guidCacheEntry=new KeyedIndex;
				guidCacheEntry->key=id;
				guidCacheEntry->index=playerIndex;
				guidCache->Add(guidCacheEntry);
			}
			else
			{
				// it's already in the cache
			}
		}
	}

	if (AKAFound)
	{
		globalStatistics.AddAKA(playerIndex,AKAName);
	}

	return(playerIndex);

}

void PlayerList::WriteToDatabase()
{
	Player	player;
	int		playerCount;
	int		playerIndex;

	playerCount=players.GetCount();
	for (playerIndex=0;playerIndex<playerCount;playerIndex++)
	{
		player=players.Item(playerIndex);
		player.WriteToDatabase(player.actualPlayerIndex);
	}
}

int PlayerList::FindGUIDIndex(wxString &guid)
{
	int		playerCount;
	int		playerIndex;
	Player		player;
	int		foundIndex=-1;
	int		searchIndex=-1;


	if (cacheEnabled)
	{
		KeyedIndex		searchKey;
		searchKey.key=guid;
		searchIndex=guidCache->Index(&searchKey);
		if (searchIndex==wxNOT_FOUND)
		{
			foundIndex=-1;
		}
		else
		{
			KeyedIndex	*foundKey;
			foundKey=guidCache->Item(searchIndex);
			foundIndex=foundKey->index;
		}
	}
	else
	{
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
	}

	return(foundIndex);

}

int PlayerList::GetCount()
{
	return (players.GetCount());
}

Player &PlayerList::Item(int index)
{
	return (players.Item(index));
}

Player *PlayerList::Detach(int index)
{
	return (players.Detach(index));
}

void PlayerList::Add(Player *player)
{
	players.Add(player);
}

void PlayerList::Add(Player &player)
{
	players.Add(player);
}

void PlayerList::RemoveAt(int index)
{
	players.RemoveAt(index);
}
void PlayerList::Insert(Player &player,int playerIndex)
{
	players.Insert(player,playerIndex);
}

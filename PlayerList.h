#ifndef __PLAYERLIST
#define __PLAYERLIST

#include <wx/wx.h>

// Statsgen Includes
#include "Player.h"
#include "DynamicArrays.h"
#include "KeyedIndex.h"

class PlayerList
{
	public:
		PlayerList();
		~PlayerList();
		Player &GetPlayer(int index);
		int FindPlayerIndex(wxString &name);
		int AddPlayer(wxString &name,wxString &id);
		int FindGUIDIndex(wxString &guid);
		int GetCount();
		void Clear();
		Player &Item(int index);
		Player *Detach(int index);
		void Add(Player *player);
		void Add(Player &player);
		void RemoveAt(int index);
		void Insert(Player &player,int playerIndex);
		void WriteToDatabase();
		void EnableCache();
		void DisableCache();

	protected:

	private:
		ArrayOfPlayers		players;
		bool				cacheEnabled;
		ArrayOfKeyedIndex	*nameCache;
		ArrayOfKeyedIndex	*guidCache;

};

#endif

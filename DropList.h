#ifndef __DROPLIST
#define __DROPLIST

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>

// Statsgen Includes
#include "DropListEntry.h"

class DropList
{
	public:
		bool IsDropped(wxString &name,bool *cheat);
		void Sort();
		void Load();
		void Write();
		DropList();
		virtual ~DropList();
		void AddCheat(wxString &name);
		void AddEntry(DropListEntry &entry);
		void RemoveEntry(DropListEntry &entry);
		int DropIndex(wxString &name,ArrayOfDropListEntry &list);
		void AddToCache(wxString &name,bool dropped,bool cheat);
		bool InCache(wxString &name,bool *dropped,bool *cheat);

		static int Compare(DropListEntry **entry1,DropListEntry **entry2);

		ArrayOfDropListEntry		allowList;
		ArrayOfDropListEntry		dropList;

		wxSortedArrayString		cheatListCache;
		wxSortedArrayString		dropListCache;
		wxSortedArrayString		notDroppedListCache;

		bool					useCache;

};


#endif

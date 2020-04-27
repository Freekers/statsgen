#ifndef __KEYEDINDEX
#define __KEYEDINDEX

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

class KeyedIndex;

WX_DEFINE_SORTED_ARRAY(KeyedIndex *,ArrayOfKeyedIndex);
class ArrayOfKeyedIndex;
class KeyedIndex
{
	public:
		static int Compare(KeyedIndex *item1,KeyedIndex *item2);
		wxString	key;
		int			index;
};

#endif

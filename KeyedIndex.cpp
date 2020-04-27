// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>

// Statsgen includes
#include "KeyedIndex.h"

int KeyedIndex::Compare(KeyedIndex *item1,KeyedIndex *item2)
{
	return (item1->key.Cmp(item2->key));
}

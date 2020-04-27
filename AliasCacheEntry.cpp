// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>

// Statsgen includes
#include "AliasCacheEntry.h"

int AliasCacheEntry::Compare(AliasCacheEntry *item1,AliasCacheEntry *item2)
{
	int	guidResult;
	int	retVal;

	guidResult=item1->guid.Cmp(item2->guid);
	if (guidResult==0)
	{
		retVal=item1->name.Cmp(item2->name);
	}
	else
	{
		retVal=guidResult;
	}
	return (retVal);
}

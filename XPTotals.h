#ifndef __XPTOTALS
#define __XPTOTALS

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

#include "XPPoints.h"
// Statsgen Includes

class XPTotals
{
	public:
		XPTotals();
		virtual ~XPTotals();
		void Add(XPPoints &points);
		bool WriteToDatabase();
		void Clear();
	public:
		ArrayOfXPPoints	xpPoints;
		
		
};


#endif

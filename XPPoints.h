#ifndef __XPPOINTS
#define __XPPOINTS

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class XPPoints;

WX_DECLARE_OBJARRAY(XPPoints,ArrayOfXPPoints);
class XPPoints
{
	public:
		XPPoints();
		virtual ~XPPoints();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		static wxString SQLTotalCreateTable();
		static wxString SQLTotalTableName();
		bool WriteToDatabase(int roundIndex);
		bool WriteTotalToDatabase();

	public:
		int		playerIndex;
		int		pointsIndex;
		float	points;
		
		
};


#endif

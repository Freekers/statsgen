// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "XPTotals.h"
#include "XPPoints.h"
#include "GlobalStatistics.h"

XPTotals::XPTotals()
{
}

XPTotals::~XPTotals()
{
}

void XPTotals::Add(XPPoints &xp)
{
	int			xpCount;
	int			xpIndex;
	XPPoints	xpFound;
	XPPoints	newXP;

	newXP=xp;
	xpCount=xpPoints.GetCount();
	for (xpIndex=0;xpIndex<xpCount;xpIndex++)
	{
		xpFound=xpPoints.Item(xpIndex);
		if ((xpFound.playerIndex==xp.playerIndex) &&
			(xpFound.pointsIndex==xp.pointsIndex))
		{
			xpPoints.RemoveAt(xpIndex);
			newXP.points+=(xpFound.points);
			break;
		}
	}
	xpPoints.Add(newXP);
}


void XPTotals::Clear()
{
	xpPoints.Clear();
}

bool XPTotals::WriteToDatabase()
{
	int			xpCount;
	int			xpIndex;
	XPPoints	xpFound;

	xpCount=xpPoints.GetCount();
	for (xpIndex=0;xpIndex<xpCount;xpIndex++)
	{
		xpFound=xpPoints.Item(xpIndex);
		xpFound.WriteTotalToDatabase();
	}

	return (true);
}



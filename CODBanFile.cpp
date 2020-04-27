// wxWindows includes
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/filename.h>

// Statsgen includes
#include "GlobalStatistics.h"
#include "Progress.h"
#include "CODBanFile.h"

CODBanFile::CODBanFile(wxFileName &filenameIn)
{
	filename=filenameIn;
	LoadFile();
}

CODBanFile::~CODBanFile()
{
}

void CODBanFile::LoadFile()
{
	int					banCount;
	int					banIndex;
	wxTextFile			fp;
	bool				retVal;
	wxString			status;
	wxString			line;
	wxString			name;
	wxString			guid;

	status="Reading Ban File"; progress->SetStatus(status);
	retVal=fp.Open(filename.GetFullPath());

	names.Clear();

	if (retVal)
	{
		banCount=fp.GetLineCount();
		for (banIndex=0;banIndex<banCount;banIndex++)
		{
			line=fp[banIndex];
			if (line.Length()>0)
			{
				guid=line.BeforeFirst(' ');
				name=line.AfterFirst(' ');

				names.Add(name);
			}
		}
	}
}

void CODBanFile::AddToDropList()
{
	int			banCount;
	int			banIndex;
	wxString	name;

	banCount=names.GetCount();
	for (banIndex=0;banIndex<banCount;banIndex++)
	{
		name=names.Item(banIndex);
		globalStatistics.dropList.AddCheat(name);
	}
}




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
#include "PunkbusterBanFile.h"

PunkbusterBanFile::PunkbusterBanFile(wxFileName &filenameIn)
{
	filename=filenameIn;
	LoadFile();
}

PunkbusterBanFile::~PunkbusterBanFile()
{
}

void PunkbusterBanFile::LoadFile()
{
	int					banCount;
	int					banIndex;
	wxTextFile			fp;
	bool				retVal;
	wxString			status;
	wxString			line;
	wxString			beforeName;
	wxString			afterName;
	wxString			name;
	wxString			reason;

	status="Reading Ban File"; progress->SetStatus(status);
	retVal=fp.Open(filename.GetFullPath());

	names.Clear();
	reasons.Clear();

	if (retVal)
	{
		banCount=fp.GetLineCount();
		for (banIndex=0;banIndex<banCount;banIndex++)
		{
			line=fp[banIndex];
			if (line.Length()>0)
			{
				beforeName=line.BeforeFirst('"');
				afterName=line.AfterLast('"');
				name=line.AfterFirst('"');
				name=name.BeforeLast('"');
				// name is currently set to "name and ip" 
				name=name.BeforeLast('"');
				name=name.BeforeLast('"');

				reason=beforeName+" "+afterName;
				reasons.Add(reason);
				names.Add(name);
			}
		}
	}
}

void PunkbusterBanFile::AddToDropList()
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




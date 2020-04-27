// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filename.h>
#include <wx/stream.h>
#include <wx/wfstream.h>

// Statsgen includes
#include "MultiFile.h"

MultiFile::MultiFile(wxArrayString &filenamesIn)
{
	LoadFiles(filenamesIn);
	currentFileIndex=0;
}

MultiFile::~MultiFile()
{
	UnloadFiles();
}

wxString MultiFile::Filename()
{
	int				fileCount;
	int				fileIndex;
	wxString		filename;
	wxString		retVal;

	fileCount=filenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=filenames.Item(fileIndex);
		retVal+=filename;
		retVal+=",";
	}
	return(retVal);
}

bool MultiFile::Ok()
{
	int						fileCount;
	int						fileIndex;
	wxFileInputStream		*filestream;
	bool					retVal;

	fileCount=filenames.GetCount();
	retVal=(fileCount>0);
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filestream=(wxFileInputStream *)filestreams.Item(fileIndex);
		if (filestream!=NULL)
		{
			retVal=(retVal && filestream->Ok());
		}
	}

	return (retVal);
}

void MultiFile::UnloadFiles()
{
	int						fileCount;
	int						fileIndex;
	wxFileInputStream		*filestream;
	wxBufferedInputStream	*bufferedstream;

	currentFileIndex=0;
	fileCount=filenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filestream=(wxFileInputStream *)filestreams.Item(fileIndex);
		bufferedstream=(wxBufferedInputStream *)bufferedstreams.Item(fileIndex);
		if (bufferedstream!=NULL)
		{
			delete bufferedstream;
		}
		if (filestream!=NULL)
		{
			delete filestream;
		}
	}
	bufferedstreams.Clear();
	filestreams.Clear();
}

void MultiFile::LoadFiles(wxArrayString &filenamesIn)
{
	int						fileCount;
	int						fileIndex;
	wxString				filename;
	wxFileInputStream		*filestream;
	wxBufferedInputStream	*bufferedstream;
	wxString				msg;

	UnloadFiles();
	filenames=filenamesIn;
	fileCount=filenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=filenames.Item(fileIndex);
		filestream=new wxFileInputStream(filename);
		if (filestream!=NULL)
		{
			bufferedstream=new wxBufferedInputStream(*filestream);
		}
		else
		{
			bufferedstream=NULL;
		}
		filestreams.Add(filestream);
		bufferedstreams.Add(bufferedstream);
	}
}

long MultiFile::Length()
{
	int						fileCount;
	int						fileIndex;
	long					retVal;
	wxFileInputStream		*filestream;
	wxString				msg;
	int						activeFiles=0;

	retVal=0;
	fileCount=filenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filestream=(wxFileInputStream *)filestreams.Item(fileIndex);
		if (filestream!=NULL)
		{
			activeFiles++;
			retVal+=filestream->GetSize();
		}
	}

	return (retVal);
}

long MultiFile::Size()
{
	return (Length());
}

long MultiFile::TellI()
{
	int						fileIndex;
	long					retVal;
	wxString				filename;
	wxBufferedInputStream	*filestream;

	retVal=0;
	for (fileIndex=0;fileIndex<=currentFileIndex;fileIndex++)
	{
		filestream=(wxBufferedInputStream *)bufferedstreams.Item(fileIndex);
		if (filestream!=NULL)
		{
			if (fileIndex<(currentFileIndex-1))
			{
				retVal+=(filestream->GetSize());
			}
			else
			{
				retVal+=(filestream->TellI());
			}
		}
	}

	return (retVal);
}

long MultiFile::SeekI(long newPosition)
{
	int						fileCount;
	int						fileIndex;
	long					currentPosition;
	wxString				filename;
	wxBufferedInputStream	*filestream;
	long					thisFileSize;

	currentPosition=0;
	currentFileIndex=0;
	fileCount=filenames.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		currentFileIndex=fileIndex;
		filestream=(wxBufferedInputStream *)bufferedstreams.Item(fileIndex);
		if (filestream!=NULL)
		{
			thisFileSize=filestream->GetSize();
			if (newPosition<=(currentPosition+thisFileSize))
			{
				// requested position is in this file
				filestream->SeekI(newPosition-currentPosition);
				currentPosition=newPosition;
				break;
			}
			else
			{
				filestream->SeekI(thisFileSize);
				currentPosition+=(thisFileSize);
			}
		}
		else
		{
		}
	}

	return (currentPosition);
}

bool MultiFile::Eof()
{
	wxBufferedInputStream	*filestream;
	bool					retVal=true;
	int						fileIndex;
	int						fileCount;

	fileCount=filenames.GetCount();
	for (fileIndex=currentFileIndex;fileIndex<fileCount;fileIndex++)
	{
		filestream=(wxBufferedInputStream *)filestreams.Item(currentFileIndex);
		if (filestream!=NULL)
		{
			retVal=filestream->Eof();
			if (!retVal)
			{
				break;
			}
		}
		currentFileIndex++;
	}
	return (retVal);
}

unsigned char MultiFile::GetC()
{
	wxBufferedInputStream	*filestream;
	unsigned char			retVal=0;
	int						fileIndex;
	int						fileCount;

	fileCount=filenames.GetCount();

	for (fileIndex=currentFileIndex;
		fileIndex<fileCount;
		fileIndex++)
	{
		//filestream=(wxBufferedInputStream *)filestreams.Item(currentFileIndex);
		filestream=(wxBufferedInputStream *)bufferedstreams.Item(currentFileIndex);
		if (filestream!=NULL)
		{
			if (!filestream->Eof())
			{
				retVal=filestream->GetC();
				break;
			}
			else
			{
				// eof
				if (fileIndex!=(fileCount-1))
				{
					// we are "between" files
					// we need to fake an end of line
					currentFileIndex=(fileIndex+1);
					retVal=0xa;
					break;
				}
			}
		}
		currentFileIndex=fileIndex;
	}
	return (retVal);
}


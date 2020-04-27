#ifndef __MULTIFILE
#define __MULTIFILE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>

// Statsgen includes
#include "DynamicArrays.h"

class MultiFile
{
	public:
		MultiFile(wxArrayString &filenamesIn);
		~MultiFile();
		wxString Filename();
		bool Eof();
		unsigned char GetC();
		bool Ok();
		long Length();
		long SeekI(long position);
		long TellI();
		long Size();

	private:
		void LoadFiles(wxArrayString &filenames);
		void UnloadFiles();
	public:
		wxArrayString		filenames;
		ArrayOfPointers		filestreams;
		ArrayOfPointers		bufferedstreams;
		long				currentFileIndex;
};

#endif

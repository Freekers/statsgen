#ifndef __SPEECHDATA
#define __SPEECHDATA

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class SpeechData;

WX_DECLARE_OBJARRAY(SpeechData,ArrayOfSpeechData);
class SpeechData
{
	public:
		SpeechData();
		virtual ~SpeechData();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int roundIndex,int itemIndex);

		
		wxDateTime		speechTime;
		int			playerIndex;
		wxString		speech;
		
};


#endif

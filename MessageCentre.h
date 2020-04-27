#ifndef __MESSAGECENTRE
#define __MESSAGECENTRE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>

// Statsgen Includes
#include "LogFile.h"

class MessageCentre
{
	public:
		MessageCentre();
		virtual ~MessageCentre();

		void SetID(wxString &IDIn);
		void SendMessage();
		void Initiate();
		void SendRCONCommand(wxString &command);
		void UpdatePunkbusterMessages();


		wxArrayString	messagesToSend;
		int				messageIndex;
		wxString		ID;

		int				gamePort;
		int				localPort;
		wxString		hostname;
		wxString		gameIP;
		wxString		password;

		bool			overallEnabled;
		bool			serverEnabled;
		bool			usingPunkbuster;
		wxString		remotePunkbusterDirectory;
		wxString		serverType;
		wxString		messagePrefix;
		wxString		punkbusterPostCommand;
};


#endif

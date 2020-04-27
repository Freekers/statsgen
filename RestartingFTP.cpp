#include "RestartingFTP.h"

RestartingFTP::RestartingFTP()
{
}


RestartingFTP::~RestartingFTP()
{
}

wxInputStream *RestartingFTP::GetInputStreamRestart(const wxString &path,int startPos)
{
	// For now disable it
	return NULL;
}

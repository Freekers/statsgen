#include "wx/wx.h"

#include "ContentTypes.h"

ContentTypes::ContentTypes( )
{
	contentTypes[_("txt")] = _("text/plain");
	contentTypes[_("html")] = _("text/html");
	contentTypes[_("htm")] = _("text/html");
	contentTypes[_("css")] = _("text/css");
	contentTypes[_("js")] = _("text/javascript");
	contentTypes[_("xml")] = _("text/xml");

	contentTypes[_("gif")] = _("image/gif");
	contentTypes[_("jpg")] = _("image/jpeg");
	contentTypes[_("png")] = _("image/png");
	
	contentTypes[_("zip")] = _("application/zip");
	contentTypes[_("pdf")] = _("application/pdf");
	contentTypes[_("swf")] = _("application/x-shockwave-flash");

	contentTypes[_("mp3")] = _("audio/mpeg");
	contentTypes[_("wav")] = _("audio/wav");
	contentTypes[_("wma")] = _("audio/x-ms-wma");
	contentTypes[_("aac")] = _("audio/aac");

	contentTypes[_("qt")] = _("video/quicktime");
	contentTypes[_("mov")] = _("video/quicktime");
	contentTypes[_("3gp")] = _("video/3gpp");
	contentTypes[_("avi")] = _("video/avi");
	contentTypes[_("wmv")] = _("video/x-ms-wmv");

	contentTypes[_("wml")] = _("text/vnd.wap.wml");
	contentTypes[_("jad")] = _("text/vnd.sun.j2me.app-descriptor");
	contentTypes[_("jar")] = _("application/java-archive");
}


ContentTypes::~ContentTypes()
{
}

wxString ContentTypes::GetContentType( const wxString& fileExt )
{
	wxString value = wxEmptyString;
	ContentTypesHash::iterator it;
	it = contentTypes.find( fileExt );
	if( it != contentTypes.end() )
        value = it->second;

	return value;
}


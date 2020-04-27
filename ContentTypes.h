#ifndef _CONTENTTYPES_H_
#define _CONTENTTYPES_H_

#include "wx/wx.h"

// hash map for content types
WX_DECLARE_STRING_HASH_MAP( wxString, ContentTypesHash );

class ContentTypes
{


public:
    wxString GetContentType( const wxString& fileExt );
    ContentTypes( );
    virtual ~ContentTypes();

private:
	ContentTypesHash contentTypes;

};

#endif


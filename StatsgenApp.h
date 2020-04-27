#ifndef __WXWIDGETSAPP_H
#define __WXWIDGETSAPP_H

#include <wx/wx.h>

class wxWidgetsApp : public wxApp
{
public:
    wxWidgetsApp();
    virtual ~wxWidgetsApp();
    virtual bool OnInit();
};

DECLARE_APP(wxWidgetsApp)

#endif //__WXWIDGETSAPP_H

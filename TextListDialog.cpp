// Statsgen Includes
#include "TextListDialog.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"
#include "LogFileReader.h"

BEGIN_EVENT_TABLE(TextListDialog,GenericOKCancelDialog)
	EVT_SIZE(TextListDialog::OnResize)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,TextListDialog::OnNew)
END_EVENT_TABLE()

TextListDialog::TextListDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				const char *groupIn) : GenericOKCancelDialog
											(parent, 
											id,
											title,
											pos,
											size,
											style,
											name)
{
	wxArrayString	textItems;

	group=groupIn;
	singleWords=true;

	newButton.Create(this,
					WINDOW_ID_BUTTON_NEW,
					_T(WINDOW_ID_BUTTON_NEW_TEXT),
					wxDefaultPosition);
	globalStatistics.configData.ReadList(group,textItems);
	panel=new TextListEditorPanel();
	panel->SetList(textItems);
	panel->Create(this,
				-1,
				wxDefaultPosition,
				wxDefaultSize
				);
}

bool TextListDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)panel));
}

TextListDialog::~TextListDialog()
{
}

void TextListDialog::OnSave(wxCommandEvent &event)
{
	wxString			msg;
	wxArrayString	textItems;

	panel->GetList(textItems);
	// Write the TextList List to disk

	globalStatistics.configData.WriteList(group,textItems);
	globalStatistics.configData.CommitChanges();
	// Do any standard Save
	GenericOKCancelDialog::OnSave(event);
}

void TextListDialog::OnQuit(wxCommandEvent &event)
{
	wxString			msg;

	// Do any standard Save
	GenericOKCancelDialog::OnQuit(event);
}

void TextListDialog::OnNew(wxCommandEvent &event)
{
	wxString	message="Enter New";
	wxString	caption="Enter New";
	wxString	newEntry;
	wxString	userEntry;
	int			tokenCount;
	int			tokenIndex;

	userEntry=wxGetTextFromUser(message,caption);
	if (userEntry.Length()>0)
	{
		if (singleWords)
		{
			GetLineToken(userEntry," ",1,&tokenCount);
			for (tokenIndex=1;tokenIndex<=tokenCount;tokenIndex++)
			{
				newEntry=GetLineToken(userEntry," ",tokenIndex,&tokenCount);
				panel->Add(newEntry);
			}
		}
		else
		{
			panel->Add(userEntry);
		}
	}
}

void TextListDialog::OnResize(wxSizeEvent &event)
{
	int 	newX;
	int		newWidth;
	int		newHeight;
	int		quitX;
	int		quitY;
	int		quitWidth;
	wxSize	itemSize;
	wxPoint	itemPos;
	GenericOKCancelDialog::OnResize(event);

	itemPos=quitButton.GetPosition();
	itemSize=quitButton.GetSize();
	quitWidth=itemSize.GetWidth();
	quitX=itemPos.x;
	quitY=itemPos.y;
	
	newX=quitX+quitWidth;
	itemSize=newButton.GetSize();
	newWidth=itemSize.GetWidth();
	newHeight=itemSize.GetHeight();
	newButton.SetSize(newX,quitY,newWidth,newHeight);
}

void TextListDialog::SortOn(bool value)
{
	panel->SortOn(value);
}
void TextListDialog::SingleWords(bool value)
{
	singleWords=value;
}

// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/menu.h>

// Statsgen Includes
#include "TextListEditorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "ErrorData.h"

BEGIN_EVENT_TABLE(TextListEditorPanel, wxPanel)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_TEXTLIST,
								TextListEditorPanel::OnTextListRightClick)
		EVT_MENU(TEXTLIST_POPUP_DELETE,TextListEditorPanel::OnPopupMenu)
		EVT_LIST_END_LABEL_EDIT(WINDOW_ID_TEXTLIST,TextListEditorPanel::OnLabelEdit)
END_EVENT_TABLE()

void TextListEditorPanel::SetList(wxArrayString &textListIn)
{
	mTextList = textListIn;
}

TextListEditorPanel::TextListEditorPanel()
{
	mTextListList	= NULL;
	mSortList		= true;
}

void TextListEditorPanel::SortOn(bool value)
{
	mSortList	= value;
}

bool TextListEditorPanel::Create(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
{

	wxPanel::Create( parent, id,
		pos,
		size,
		style,
		name);

	mTextListList=new wxListCtrl(this,
							WINDOW_ID_TEXTLIST,
							wxDefaultPosition,
							wxDefaultSize,
							wxLC_REPORT|
							wxLC_EDIT_LABELS);
	RefreshTextListTree();

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(mTextListList,1,wxEXPAND);

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	return (true);
}

void TextListEditorPanel::AddTextListEntry(int index,wxString &entry)
{
	long		itemIndex;
	long		listIndex;
	long		rowNumber;

	rowNumber=index;
	itemIndex=index;

	listIndex=mTextListList->InsertItem(rowNumber,entry);
	mTextListList->SetItemData(listIndex,itemIndex);
}


void TextListEditorPanel::RefreshTextListTree()
{
	wxString			textListEntry;
	int					textListCount;
	int					textListIndex;

	STATSGEN_DEBUG_FUNCTION_START("TextListEditorPanel","RefreshTextListTree")
	if (mSortList)
	{
		mTextList.Sort();
	}
	mTextListList->Hide();
	
	mTextListList->DeleteAllColumns();
	mTextListList->DeleteAllItems();
	mTextListList->InsertColumn(0,_T("Word"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);

	textListCount=mTextList.GetCount();
	for (textListIndex=0;textListIndex<textListCount;textListIndex++)
	{
		textListEntry=mTextList.Item(textListIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,textListEntry);
		AddTextListEntry(textListIndex,textListEntry);
	}

	mTextListList->SetColumnWidth(0,wxLIST_AUTOSIZE);
	mTextListList->Show();
	STATSGEN_DEBUG_FUNCTION_END
}

void TextListEditorPanel::OnTextListRightClick(wxListEvent &event)
{
	wxString		msg;
	wxMenu			popupMenu(_T(""));
	wxString		menuItem;

	// What have we right clicked on - parent or child?
	menuItem="Delete"; popupMenu.Append(TEXTLIST_POPUP_DELETE,menuItem);

	popupMenu.AppendSeparator();
	popupMenu.Append(TEXTLIST_POPUP_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void TextListEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	wxString			msg;
	int					id;
	int			selectedItem=-1;
	int			listIndex;

	STATSGEN_DEBUG_FUNCTION_START("TextListEditorPanel","OnPopupMenu")
	id=event.GetId();

	switch (id)
	{
		case TEXTLIST_POPUP_DELETE:
			selectedItem=mTextListList->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
			STATSGEN_DEBUG_CODE(msg.Printf("Deleting selected index %d",selectedItem);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			while (selectedItem!=-1)
			{
				listIndex=(int)mTextListList->GetItemData(selectedItem);
				STATSGEN_DEBUG_CODE(msg.Printf("Deleting list index %d",listIndex);)
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
				mTextList.RemoveAt(listIndex);
				selectedItem=-1;
			}
			RefreshTextListTree();
			break;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void TextListEditorPanel::OnLabelEdit(wxListEvent &event)
{
	wxString		msg;
	wxString		menuItem;
	wxMenu			popupMenu(_T(""));
	long			index;
	wxString		textListEntry;

	STATSGEN_DEBUG_FUNCTION_START("TextListEditorPanel","OnLabelEdit")
	index=event.GetIndex();

	STATSGEN_DEBUG_CODE(msg.Printf("Index edited %d",index);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	if (index>=0)
	{
		textListEntry=mTextList.Item(index);
		STATSGEN_DEBUG(DEBUG_ALWAYS,textListEntry);
		mTextList.RemoveAt(index);
		STATSGEN_DEBUG(DEBUG_ALWAYS,event.GetText());
		mTextList.Add(event.GetText());
	}

	event.Veto();
	RefreshTextListTree();
	STATSGEN_DEBUG_FUNCTION_END
}

void TextListEditorPanel::GetList(wxArrayString &textListOut)
{
	textListOut=mTextList;
}

void TextListEditorPanel::Add(wxString &entry)
{
	mTextList.Add(entry);

	RefreshTextListTree();
}


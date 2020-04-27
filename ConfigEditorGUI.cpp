// Statsgen Includes
#include "ConfigEditorGUI.h"
#include "GlobalStatistics.h"
#include "BoxedDropDown.h"
#include "LogFileReader.h"

BEGIN_EVENT_TABLE(ConfigEditorGUI, wxDialog)
		EVT_SIZE(ConfigEditorGUI::OnResize)
		EVT_LIST_ITEM_ACTIVATED(WINDOW_ID_LISTBOX_CONFIGITEMS,
					ConfigEditorGUI::OnListItemSelected)
		EVT_LIST_ITEM_SELECTED(WINDOW_ID_LISTBOX_CONFIGITEMS,
					ConfigEditorGUI::OnListItemSelected)
		EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,ConfigEditorGUI::OnSave)
		EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,ConfigEditorGUI::OnQuit)
	//	EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,ConfigEditorGUI::OnTextChange)
		EVT_COMBOBOX(WINDOW_ID_BOXED_DROPDOWN_FILTER1,ConfigEditorGUI::OnFilterChanged)
		EVT_COMBOBOX(WINDOW_ID_BOXED_DROPDOWN_FILTER2,ConfigEditorGUI::OnFilterChanged)
END_EVENT_TABLE()

ConfigEditorGUI::ConfigEditorGUI(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				BoxedDropDown *filter1In,
				BoxedDropDown *filter2In,
				GroupedConfigItemsPanel *configPanelIn,
				bool drawImagesIn):
			wxDialog(parent,
				id,
				title,
				pos,
				size,
				style,
				name)

{
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","ConfigEditorGUI")
	// Zero things out
	configKeys.Clear();
	configDisplayNames.Clear();
	filter1=filter1In;
	filter2=filter2In;
	configPanel=configPanelIn;
	drawImages=drawImagesIn;
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::OnTextChange(wxCommandEvent& event)
{
	wxString	msg;
	wxString	key;
	wxString	value;
	wxString	label="Enter Data";
	wxString	defaultValue="";
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","OnTextChange")

	value=valueText.GetValue();
	//globalStatistics.configData.WriteTextValue(lastSelectedKey,
	//									value);
	valueText.SetConfigKey(lastSelectedKey);
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::OnQuit(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void ConfigEditorGUI::OnSave(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void ConfigEditorGUI::OnListItemSelected(wxListEvent& event)
{
	wxString	msg;
	int			configIndex;
	wxString	value;
	wxString	label="Enter Data";
	wxString	defaultValue="";

	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","OnListItemSelected")
	configIndex=(int)event.GetData();
	lastSelectedKey=configKeys.Item(configIndex);
	valueText.SetConfigKey(lastSelectedKey);
	if (drawImages)
	{
		wxString	configKey;
		wxString	localOutputFolder;
		wxString	imageFilename;

		configKey="/General/LocalOutput";
		globalStatistics.configData.ReadTextValue(configKey,&localOutputFolder);
		imageFilename=localOutputFolder;
		imageFilename+="\\";
		imageFilename+=valueText.GetValue();
		if (wxFileExists(imageFilename))
		{
			wxImage		imageFile;

			imageFile.LoadFile(imageFilename);
			//imagePanel->SetSize(imagePanelX,imagePanelY,
			//			imagePanelWidth,imagePanelHeight);
			imagePanel->Scale(1.0);
			imagePanel->SetImage(imageFile);

			imagePanel->Proportion(imagePanelWidth,imagePanelHeight);
			imageFile.Destroy();
		}
		else
		{
			imagePanel->Clear();
		}
	}
	//globalStatistics.configData.ReadTextValue(lastSelectedKey,&value);
	//valueText.SetValue(value);
	STATSGEN_DEBUG_FUNCTION_END
	
}

ConfigEditorGUI::~ConfigEditorGUI()
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","~ConfigEditorGUI")
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::PopulateConfigItems()
{
	wxListItem	listColumn;
	int			count;
	int			index;
	long		listIndex;
	wxString	name;
	wxString	msg;
	wxString	filterString1="";
	wxString	filterString2="";
	int			tokenCount;
	wxString	keyPart1;
	wxString	keyPart2;
	wxString	key;
	bool		filtered;
	wxSizeEvent	event;

	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","PopulateConfigItems")
	if (filter1!=NULL)
	{
		filterString1=filter1->GetSelectedCode();
	}
	if (filter2!=NULL)
	{
		filterString2=filter2->GetSelectedCode();
	}

	configItems.DeleteAllColumns();
	configItems.DeleteAllItems();
	listColumn.SetText(_T("Key"));
	listColumn.SetWidth(100);
	configItems.InsertColumn(0,listColumn);

	count=configKeys.GetCount();
	for (index=0;index<count;index++)
	{
		name=configDisplayNames.Item(index);
		key=configBaseKeys.Item(index);
		keyPart1=GetLineToken(key,"_",1,&tokenCount);
		keyPart2=GetLineToken(key,"_",2,&tokenCount);
		filtered=false;
		if ((filterString1.Length()>0) &&
			(filterString1.CmpNoCase(keyPart1)!=0))
		{
			filtered=true;
		}
		if ((filterString2.Length()>0) &&
			(filterString2.CmpNoCase(keyPart2)!=0))
		{
			filtered=true;
		}
		if (globalStatistics.configData.IsWeaponGroupKey(key))
		{
			filtered=false;
		}
		if (!filtered)
		{
			listIndex=configItems.InsertItem((long)index,name,0);
			configItems.SetItemData(listIndex,index);
		}
	}
	OnResize(event);
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::CreateDialog()
{
	wxString	configKey="";
	wxString	configValue="";
	wxString	label="Enter Data";
	wxString	defaultValue="";

	wxPoint		configItemsPosition=wxDefaultPosition;
	wxSize		configItemsSize=wxDefaultSize;
	int			configItemsStyle=wxLC_REPORT | 
								wxLC_SINGLE_SEL | 
								wxLC_SORT_ASCENDING;
	wxString	configItemsTitle="List Box";
	wxString	valueTextValue="";
	wxPoint		valueTextPosition=wxDefaultPosition;
	wxSize		valueTextSize=wxDefaultSize;
	int			valueTextStyle=0;
	wxString	valueTextName="Value";
	wxString	key;
	wxString	value;

	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","CreateDialog")
	configItems.Create(this,
					WINDOW_ID_LISTBOX_CONFIGITEMS,
					configItemsPosition,
					configItemsSize,
					configItemsStyle,
					wxDefaultValidator,
					configItemsTitle);


	STATSGEN_DEBUG(DEBUG_ALWAYS,"configItems.Create")
	//valueText.Create(this,
	//				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
	//				valueTextValue,
	//				valueTextPosition,
	//				valueTextSize,
	//				valueTextStyle,
	//				wxDefaultValidator,
	//				valueTextName);
	valueText.Create(this,-1,valueTextPosition,valueTextSize);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"valueText.Create")
	valueText.Set(configKey,label,defaultValue,-1);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"valueText.Set")

	if (filter1!=NULL)
	{
		filter1->CreateDialog(this,
						WINDOW_ID_BOXED_DROPDOWN_FILTER1);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,"filter1.CreateDialog")
	if (filter2!=NULL)
	{
		filter2->CreateDialog(this,
						WINDOW_ID_BOXED_DROPDOWN_FILTER2);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,"filter.Created")
							
	saveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	quitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"buttons.Created")
	if (drawImages)
	{
		imagePanel=new ImagePanel(this,
								-1,
								wxDefaultPosition,
								wxDefaultSize,
								0,
								_T("imagepanel"));
	}

	PopulateConfigItems();
	STATSGEN_DEBUG_FUNCTION_END
					
}

void ConfigEditorGUI::DisplayDialog()
{
	int			dialogRetVal;

	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","DisplayDialog")
	// Called when we want to pop the dialog box
	// into existance for the first time

	// First we want to create all the items in the dialog box
	CreateDialog();

	// Now we can resize every item in the dialog to fit nicely

	// Then we pop it into existance
	dialogRetVal=ShowModal();

	// Now we do what is necessary dependent on the return code
	switch (dialogRetVal)
	{
		case WINDOW_ID_BUTTON_SAVE:
			// We have been asked to save the changes
			// just commit the config changes
			globalStatistics.configData.CommitChanges();
			break;
		case WINDOW_ID_BUTTON_QUIT:
		default:
			// We have been asked to quit without saving
			// rollback the config changes
			globalStatistics.configData.RollbackChanges();
			break;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::AddConfigKey(
					wxString &group,
					wxString &configKey,
					bool useRealName)
{
	wxString	displayName;
	wxString	key;
	wxString	fullKey;

	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","AddConfigKey")
	fullKey="/"+group+"/"+configKey;
	configKeys.Add(fullKey);
	configBaseKeys.Add(configKey);
	if (useRealName)
	{
		key="/RealNames/"+configKey;
		globalStatistics.configData.ReadTextValue(key,
												&displayName,
												(char *)configKey.GetData());
		displayName+=("(");
		displayName+=(configKey);
		displayName+=(")");
	}
	else
	{
		displayName=configKey;
	}

	configDisplayNames.Add(displayName);
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		quitWidth;
	int		quitHeight;
	int		saveWidth;
	int		saveHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		configItemsHeight;
	int		configItemsWidth;
	int		valueTextWidth;
	int		valueTextHeight;

	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","OnResize")
	itemSize=GetSize();
	dialogWidth=itemSize.GetWidth();
	dialogHeight=itemSize.GetHeight();

	// Quit and Save buttons are at the bottom of the screen
	itemSize=quitButton.GetSize();
	quitWidth=itemSize.GetWidth();
	quitHeight=itemSize.GetHeight();

	itemSize=saveButton.GetSize();
	saveWidth=itemSize.GetWidth();
	saveHeight=itemSize.GetHeight();

	// Save button
	itemPosition.x=BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	saveButton.SetPosition(itemPosition);

	// Quit button
	itemPosition.x=saveWidth+BUTTON_WIDTH_GAP+BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-quitHeight-DIALOG_BOTTOM_BORDER_SIZE;
	quitButton.SetPosition(itemPosition);

	// List Box - fills all left side of screen (45 %)
	configItemsWidth=(dialogWidth*45)/100;
	configItemsHeight=(dialogHeight-
					quitHeight-
					DIALOG_BOTTOM_BORDER_SIZE-
					BUTTON_HEIGHT_GAP);
	configItems.SetSize(0,0,configItemsWidth,configItemsHeight);
	configItems.SetColumnWidth(0,configItemsWidth);

	// Filters are at the top right of the screen
	itemPosition.x=dialogWidth/2;
	itemPosition.y=5;

	int filterIndex;
	BoxedDropDown *currentFilter=NULL;

	for (filterIndex=0;filterIndex<2;filterIndex++)
	{
		switch (filterIndex)
		{
			case 0:
				currentFilter=filter1;
				break;
			case 1:
				currentFilter=filter2;
				break;
		}
		if (currentFilter!=NULL)
		{
			currentFilter->SetPosition(itemPosition);
			itemSize=currentFilter->GetSize();
			itemPosition.y+=(itemSize.GetHeight());
			itemPosition.y+=5;
		}
	}

	if (drawImages)
	{
		imagePanelX			=dialogWidth/2;
		imagePanelY			=itemPosition.y;
		imagePanelWidth		=configItemsWidth;
		imagePanelHeight	=300;

		itemPosition.y+=imagePanelHeight;
		imagePanel->SetSize(imagePanelX,imagePanelY,
						imagePanelWidth,imagePanelHeight);
		imagePanel->Proportion(imagePanelWidth,imagePanelHeight);
	}
	if (configPanel!=NULL)
	{
		configPanel->SetPosition(itemPosition);
		itemSize=configPanel->GetSize();
		configPanel->SetSize(dialogWidth/2,
						itemPosition.y,
						configItemsWidth,
						itemSize.GetHeight());
		itemSize=configPanel->GetSize();
		itemPosition.y+=(itemSize.GetHeight());
		itemPosition.y+=5;
	}

	// Text ctrl is underneath any filters
	itemSize=valueText.GetSize();
	valueTextWidth=configItemsWidth;
	valueTextHeight=itemSize.GetHeight();
	valueText.SetSize(dialogWidth/2,
						itemPosition.y,
						valueTextWidth,
						valueTextHeight);

	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::OnFilterChanged(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","OnFilterChanged")
	PopulateConfigItems();
	STATSGEN_DEBUG_FUNCTION_END
}


// Statsgen Includes
#include "ConfigEditorGUI.h"
#include "GlobalStatistics.h"
#include "BoxedDropDown.h"
#include "LogFileReader.h"

BEGIN_EVENT_TABLE(ConfigEditorGUI, wxDialog)
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
	mConfigKeys.Clear();
	mConfigDisplayNames.Clear();
	mFilter1		= filter1In;
	mFilter2		= filter2In;
	mConfigPanel	= configPanelIn;
	mDrawImages		= drawImagesIn;
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

	value=mValueText.GetValue();
	//globalStatistics.configData.WriteTextValue(lastSelectedKey,
	//									value);
	mValueText.SetConfigKey(lastSelectedKey);
	mValueText.ApplyConfigKeyChange();
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
	lastSelectedKey=mConfigKeys.Item(configIndex);
	mValueText.SetConfigKey(lastSelectedKey);
	mValueText.ApplyConfigKeyChange();
	if (mDrawImages)
	{
		wxString	configKey;
		wxString	localOutputFolder;
		wxString	imageFilename;

		configKey="/General/LocalOutput";
		globalStatistics.configData.ReadTextValue(configKey,&localOutputFolder);
		imageFilename=localOutputFolder;
		imageFilename+="\\";
		imageFilename+=mValueText.GetValue();
		if (wxFileExists(imageFilename))
		{
			wxImage		imageFile;

			imageFile.LoadFile(imageFilename);
			//imagePanel->SetSize(imagePanelX,imagePanelY,
			//			imagePanelWidth,imagePanelHeight);
			mImagePanel->SetImage(imageFile);

			// Commented out by Shaun
			// mImagePanel->Proportion(imagePanelWidth,imagePanelHeight);
			// Commented out by Shaun
			imageFile.Destroy();
		}
		else
		{
			mImagePanel->Clear();
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
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","PopulateConfigItems")
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

	if (mFilter1!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Getting filter1 code");
		filterString1=mFilter1->GetSelectedCode();
	}
	if (mFilter2!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Getting filter2 code");
		filterString2=mFilter2->GetSelectedCode();
	}

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"DeleteAllColumns");
	mConfigItems.DeleteAllColumns();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"DeleteAllItems");
	mConfigItems.DeleteAllItems();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"SetText");
	listColumn.SetText(_T("Key"));
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"SetWidth");
	//listColumn.SetWidth(100);
	//mConfigItems.InsertColumn(0,listColumn);
	mConfigItems.InsertColumn(0,_T("Key"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);

	count=mConfigKeys.GetCount();
	for (index=0;index<count;index++)
	{
		name	= mConfigDisplayNames.Item(index);
		key		= mConfigBaseKeys.Item(index);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"name");
		STATSGEN_DEBUG(DEBUG_ALWAYS,name);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"key");
		STATSGEN_DEBUG(DEBUG_ALWAYS,key);
		keyPart1=GetLineToken(key,(char *)"_",1,&tokenCount);
		keyPart2=GetLineToken(key,(char *)"_",2,&tokenCount);
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
			listIndex=mConfigItems.InsertItem((long)index,name,0);
			mConfigItems.SetItemData(listIndex,index);
		}
	}
	mConfigItems.SetColumnWidth(0,wxLIST_AUTOSIZE);
	// Force a sizer refresh (refresh does not seem to work)
	PostSizeEventToParent();
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::CreateDialog()
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","CreateDialog")
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

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating WINDOW_ID_LISTBOX_CONFIGITEMS");
	mConfigItems.Create(this,
					WINDOW_ID_LISTBOX_CONFIGITEMS,
					configItemsPosition,
					configItemsSize,
					configItemsStyle,
					wxDefaultValidator,
					configItemsTitle);


	//valueText.Create(this,
	//				WINDOW_ID_TEXTCTRL_CONFIGVALUE,
	//				valueTextValue,
	//				valueTextPosition,
	//				valueTextSize,
	//				valueTextStyle,
	//				wxDefaultValidator,
	//				valueTextName);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Creating valueText");
	mValueText.CreateDisplay(this,wxID_ANY,label);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Setting valueText");
	mValueText.SetConfigKey(configKey,defaultValue);
	mValueText.ApplyConfigKeyChange();

	if (mFilter1!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"filter1->CreateDialog");
		mFilter1->CreateDialog(this, WINDOW_ID_BOXED_DROPDOWN_FILTER1);
	}
	if (mFilter2!=NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"filter2->CreateDialog");
		mFilter2->CreateDialog(this, WINDOW_ID_BOXED_DROPDOWN_FILTER2);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"create saveButton")
							
	mSaveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T(WINDOW_ID_BUTTON_SAVE_TEXT),
					wxDefaultPosition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"create quit Button")
	mQuitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"buttons Created")
	if (mDrawImages)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"drawing images")
		mImagePanel=new ImagePanel(this,
								wxID_ANY,
								wxDefaultPosition,
								wxDefaultSize,
								0,
								_T("imagepanel"));
	}

	wxBoxSizer	*controlsSizer;
	wxBoxSizer	*editSizer;
	wxBoxSizer	*valueSizer;

	controlsSizer = new wxBoxSizer(wxHORIZONTAL);
	controlsSizer->Add(&mSaveButton);
	controlsSizer->Add(&mQuitButton);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"controlsSizer created")
	valueSizer = new wxBoxSizer(wxVERTICAL);
	if (mFilter1 != NULL)
	{
		valueSizer->Add(mFilter1);
	}
	if (mFilter2 != NULL)
	{
		valueSizer->Add(mFilter2);
	}
	valueSizer->Add(&mValueText,0,wxEXPAND);
	if (mConfigPanel != NULL)
	{
		valueSizer->Add(mConfigPanel,0,wxEXPAND);
	}
	if (mDrawImages)
	{
		valueSizer->Add(mImagePanel,1,wxGROW);
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"valuesSizer created")

	editSizer = new wxBoxSizer(wxHORIZONTAL);
	editSizer->Add(&mConfigItems,1,wxEXPAND);
	editSizer->Add(valueSizer,0,wxEXPAND);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"editSizer created")

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(editSizer,1,wxEXPAND);
	mMainSizer->Add(controlsSizer);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"mainSizer created")

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"populate config items")
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
	mConfigKeys.Add(fullKey);
	mConfigBaseKeys.Add(configKey);
	if (useRealName)
	{
		key="/RealNames/"+configKey;
		globalStatistics.configData.ReadTextValue(key,
												&displayName,
												configKey);
		displayName+=("(");
		displayName+=(configKey);
		displayName+=(")");
	}
	else
	{
		displayName=configKey;
	}

	mConfigDisplayNames.Add(displayName);
	STATSGEN_DEBUG_FUNCTION_END
}

void ConfigEditorGUI::OnFilterChanged(wxCommandEvent& event)
{
	STATSGEN_DEBUG_FUNCTION_START("ConfigEditorGUI","OnFilterChanged")
	PopulateConfigItems();
	STATSGEN_DEBUG_FUNCTION_END
}


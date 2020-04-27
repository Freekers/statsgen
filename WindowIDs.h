#ifndef __WINDOWIDS
#define __WINDOWIDS

// wxWindows includes
#include <wx/wx.h>

enum WindowIDs
{
	WINDOW_ID_MENU_ABOUT = wxID_ABOUT,
	WINDOW_ID_MENU_QUIT = wxID_EXIT,
	WINDOW_ID_MENU_SCORE_WEIGHTS = wxID_HIGHEST,
	WINDOW_ID_MENU_SKILL_WEIGHTS,
	WINDOW_ID_MENU_REAL_NAMES,
	WINDOW_ID_MENU_ADVANCED,
	WINDOW_ID_MENU_IMAGEPACKS,
	WINDOW_ID_MENU_TEMPLATEPACKS,
	WINDOW_ID_MENU_FULL_CONFIG,
	WINDOW_ID_MENU_GO,
	WINDOW_ID_MENU_RESET_RECOVERY,
	WINDOW_ID_MENU_STORE_PROGRESS,
	WINDOW_ID_MENU_EMPTY_DATABASE,
	WINDOW_ID_MENU_IMAGES,
	WINDOW_ID_MENU_WEBSITE,
	WINDOW_ID_MENU_BAD_WORDS,
	WINDOW_ID_MENU_WEB_SERVER,
	WINDOW_ID_MENU_CUSTOM_MESSAGES,
	WINDOW_ID_MENU_OUTPUT,
	WINDOW_ID_MENU_EXTERNAL_DATABASE,
	WINDOW_ID_MENU_WEAPON_GROUPS,
	WINDOW_ID_MENU_TEMPLATE_CONFIG,
	WINDOW_ID_MENU_SERVER,
	WINDOW_ID_MENU_CLAN,
	WINDOW_ID_MENU_ALIAS,
	WINDOW_ID_MENU_DROPLIST,
	WINDOW_ID_MENU_PLAYERDATAAVATAR,
	WINDOW_ID_MENU_PLAYERDATAPICTURE,
	WINDOW_ID_MENU_PLAYERDATAWEBSITE,
	WINDOW_ID_MENU_PLAYERDATACLANROLE,
	WINDOW_ID_MENU_PLAYERDATAXFIRE,
	WINDOW_ID_MENU_PLAYERDATAREALNAME,
	WINDOW_ID_MENU_PLAYERDATAMISC1,
	WINDOW_ID_MENU_PLAYERDATAMISC2,
	WINDOW_ID_MENU_AWARD,
	WINDOW_ID_MENU_DEBUG,
	WINDOW_ID_MENU_EXPORT_TEMPLATE,
	WINDOW_ID_MENU_EXPORT_UPGRADE,
	WINDOW_ID_MENU_EXPORT_BUILD_INI,
	WINDOW_ID_MENU_PERFORM_UPGRADE,
	WINDOW_ID_MENU_RCON_CONFIG,
	WINDOW_ID_MENU_MESSAGE_FORMATS,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_GAMETYPE,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_MAP,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_CLASS,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_TEAM,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPON,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_AMMO,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_WEAPONAMMO,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_LOCATION,
	WINDOW_ID_MENU_LOG_SUBSTITUTE_ACTION,
	WINDOW_ID_MENU_THREAD,
	WINDOW_ID_MENU_HELP,
	WINDOW_ID_LISTBOX_CONFIGITEMS,
	WINDOW_ID_TEXTCTRL_CONFIGVALUE,
	WINDOW_ID_SELECTION_CONFIGVALUE,
	WINDOW_ID_BOXED_DROPDOWN_FILTER1,
	WINDOW_ID_BOXED_DROPDOWN_FILTER2,
	WINDOW_ID_PLAYERCACHE,
	WINDOW_ID_ALIASTREE,
	WINDOW_ID_DROPLIST,
	WINDOW_ID_TEXTLIST,
	WINDOW_ID_WEAPONLIST,
	WINDOW_ID_ACTIONLIST,
	WINDOW_ID_LOCATIONLIST,
	WINDOW_ID_MISCAWARDLIST,
	WINDOW_ID_XPAWARDLIST,
	WINDOW_ID_AWARDDEFINITIONPANEL,
	WINDOW_ID_AWARDCHOICESPANEL,
	WINDOW_ID_BUTTON_SAVE,
	WINDOW_ID_BUTTON_AUTO,
	WINDOW_ID_BUTTON_NEW,
	WINDOW_ID_BUTTON_DELETE,
	WINDOW_ID_BUTTON_QUIT,
	WINDOW_ID_ERROR_BUTTON,
	WINDOW_ID_CONFIG_BUTTON,
	WINDOW_ID_CONFIG_FULL_PRIORITY,
	WINDOW_ID_CONFIG_QUICK_PRIORITY,
	AWARD_DEFINITION_DELETE_COMPONENT,
	AWARD_CHOICES_ADD_POSITIVE,
	AWARD_CHOICES_ADD_NEGATIVE,
	AWARD_CHOICES_ADD_KILL_POSITIVE,
	AWARD_CHOICES_ADD_KILL_NEGATIVE,
	AWARD_CHOICES_ADD_DEATH_POSITIVE,
	AWARD_CHOICES_ADD_DEATH_NEGATIVE,
	AWARD_CHOICES_ADD_TK_POSITIVE,
	AWARD_CHOICES_ADD_TK_NEGATIVE,
	AWARD_CHOICES_ADD_KILL_AND_DEATH,
	WINDOW_ID_PROGRESS_PANEL_GENERAL,
	WINDOW_ID_PROGRESS_PANEL_CREATE_DATABASE,
	WINDOW_ID_PROGRESS_PANEL_APPLY_ALIASES,
	WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS,
	WINDOW_ID_PROGRESS_PANEL_DATABASE_READ_STATIC,
	WINDOW_ID_PROGRESS_PANEL_EMPTY_DATABASE,
	WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS,
	WINDOW_ID_PROGRESS_PANEL_DATABASE_LOCATE_EXISTING_PLAYERS,
	WINDOW_ID_PROGRESS_PANEL_DROPPING_PLAYERS,
	WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES1,
	WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES2,
	WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE,
	WINDOW_ID_PROGRESS_PANEL_READ_TEMPLATE,
	WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE,
	WINDOW_ID_PROGRESS_PANEL_TRANSFER_DATABASE,
	WINDOW_ID_PROGRESS_PANEL_WEBSITE,
	WINDOW_ID_PROGRESS_PANEL_MAX,
	WINDOW_ID_PROGRESS_PANEL_DYNAMIC,
	EVENT_ID_PROGRESS_STATUS_TEXT,
	EVENT_ID_PROGRESS_BUTTON,
	SOCKET_ID_WEB_SERVER,
	WINDOW_ID_WIZARD,
	POPUP_CANCEL
};

#define WINDOW_ID_BUTTON_SAVE_TEXT	"&Save"
#define WINDOW_ID_BUTTON_QUIT_TEXT	"&Quit"
#define WINDOW_ID_BUTTON_NEW_TEXT	"&New"
#define WINDOW_ID_BUTTON_DELETE_TEXT	"&Delete"
#define WINDOW_ID_BUTTON_AUTO_TEXT	"&Auto"

#define BUTTON_WIDTH_GAP		5
#define BUTTON_HEIGHT_GAP		5
#define DIALOG_BOTTOM_BORDER_SIZE	30

#define STATIC_BOX_TOP_GAP		20
#define STATIC_BOX_BOTTOM_GAP	10
#define STATIC_BOX_LEFT_GAP		10
#define STATIC_BOX_RIGHT_GAP	10

#define FONT_CHAR_WIDTH			8
#define FONT_TEXT_CTRL_GAP		8

#endif

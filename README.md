# Statsgen 2
Statsgen 2 is a Statistics Generator for Call Of Duty 1/2/4/5, MOHAA, Spearhead, Wolfenstein/Enemy Territory and Quake Wars. Statsgen automatically downloads the server logfiles, processes them using a flexible template system, and then transmits the resultant webpages to the webserver. Templating allows the pages produced to be very flexible to fit in with the style of your clan website.

# Building Statsgen 2

## Build environment
- MinGW 0.6.3
- Msys 1.0.11

### Installing MinGW on Windows
1. Download the MinGW Installer: https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/
1. Install MinGW to C:\MinGW
1. After installation, open the MinGW Installation Manager.
1. In the left tab, click 'Basic Setup' and mark the mingw32-gcc-g++ and mingw32-base packages for installation.
1. In the top left corner, click Installation > Apply Changes

### Installing MSYS on Windows
1. Download MSYS: http://downloads.sourceforge.net/mingw/MSYS-1.0.11.exe
1. The Default Destination of C:\msys\1.0 is fine, so click Next >
1. The default folder of MinGW is fine, so click Next > Install
1. A cmd shell will pop-up asking if you want to continue with the post install, enter y enter
1. Answer y you do have MinGW Installed.
1. Enter `c:/mingw` as the path where MinGW is installed. **NOTE the FORWARD slash (!)**
1. It should confirm that you have make.exe installed, press any key to continue.
1. Click Finish you are done the installation.

## Required third party components
- wxWidgets 3.1.1
- sqllite 3.26.0
**Note:** The versions listed above are the latest versions as tested by Shaun Jackson. Newer versions might work as well. 

### WxWidgets
1. Download the source: https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.1/wxWidgets-3.1.1.zip
1. Extract the source to a folder
1. Start MSys and navigate to the `build` folder inside the folder you just created
1. Run the following commands:
```
../configure --disable-shared --enable-monolithic
make
```
This builds the wxWidgets library how it is expected to be built

### SQLLite
1. Download the autoconf source code: https://www.sqlite.org/2018/sqlite-autoconf-3260000.tar.gz
1. Extract the source to a folder
1. Start MSys and navigate to the `build` folder inside the folder you just created
1. Type `make`

## Statsgen 2
1. Clone this repository
2. Edit the makefile. Change the paths pointing to wxWidgets and sqlite folders accordingly. 
3. The other 2 configurable variables inside the makefile are:
- DEBUG_FLAG=-DDEBUG
Setting this allows you to switch on debugging dynamically during runtime
comment this out if you want all debug to be disabled
- The final two state where statsgen tries to connect to in order to get it's
image packs
STATSGEN_WEBSITE_ADDRESS="www.statsgen.co.uk"
STATSGEN_WEBSITE_PORT=80
These should not be changed unless you intend to host your own imagepacks.
Note: As of April 2020, the imagepacks are back online on statsgen.co.uk.
4. Start MSys and navigate to the repository's folder
5. Type `make`
6. Once finished, the compiled .exe file will reside inside the repository's folder

# Installation / Usage Instructions
Download the latest release and unzip it's contents. The contents should include the following:
- statsgen2.exe
- help.zip
- libmySQL.dll - This is a library used to connect to MySQL databases
- statsgen2.ini
- readme.txt
- upgrade.ini - If prompted to upgrade - this is the file it is after

statsgen2.exe can now be run - various files / directories will be created including the default templates.

Perform the Run / First Time Configuration - DO NOT CANCEL THIS OR INSTALLATION WILL NOT COMPLETE

After a stats run is complete you can click on the buttons (tick, cross) to see what errors have been generated with each stage. A tick indicates all ok. Anything else is error / warning. You may get some initial X symbols in the General progress bar indicating alias / droplist files not available - this is not a problem and will go away once you have edited any of these items.

Image Packs (new statsgen 2 awards are there) see www.statsgen.co.uk - These can be uploaded automatically using the upload template pack functionality within statsgen mandatory menu.

To remove the statsgen banner at the top of each stats page and the link to the statsgen homepage.
Edit the Template Config (homepage stuff) (Mandatory Configuration). You can set it to your home page details or set them blank to remove altogether.

# License
The Statsgen 2 source code is distributed with permission from the original author © Shaun Jackson a.k.a. Nazarine

Unless otherwise specified, all code is released under the GNU Affero General Public License v3.0. See the repository's LICENSE file for details.

## Licenses for third party components
- wxWidgets: [wxWindows Library Licence](https://www.wxwidgets.org/about/licence/)
- sqlite: [Public Domain](https://www.sqlite.org/copyright.html/)

# Version History
Below is the original version history of Statsgen 2, as written by Shaun Jackson.

### 1.9.3 23rd November 2009
- Upgraded the MOHAA identification to properly identify maps and gametypes as well as location kills with some mods

### 1.9.2 10th September 2009
- Fixed an issue where MySQL database was not created correctly

### 1.9.0 22nd April 2009
- Updated default configurations to include 3 new COD5 maps.
- Updated default templates to give map and gametype individual pages.
- Imagepack uploading can now store the contents locally in your html folder. If imagepacks are stored locally you can now see the pictures in the image editor menu option (under display menu).

**Weapon Groups**

Weapon Grouping is now possible - weapon grouping allows you to logically group weapons together so that you can assign them to awards and as one item rather than lots of individual items. You can also assign images and skill settings to weapon groups as a whole.
I have updated the default awards to use the weapon groups (apart from that they are effectively the same as before).
Using weapon groups makes it easier to add a new weapon to an existing configuration so that you don't have to go into individual awards to edit them.
I have included some weapon groups in the default configuration (and the upgrade.ini file).
WARNING!!! - Do not make recursive weapon groups - I have not yet put protection in to stop this (I will do soon). If you create Weapon Group A that has Weapon Group B in it and Weapon Group B has Weapon Group A in it then statsgen will enter into an infinite loop as it tries to parse the weapon group.
The Enemy Territory Logfile decoder can now cope with time codes in the HH:MM:SS format
A few other minor changes that i am sure I have forgotten about.

### 1.8.8 12th March 2009
- Added ability to read COD5 1.3 Activities (bomb plants)
- Added ability to read COD5 1.3 Team Wins and Team Losses
- Corrected a slight mistake in the default template in regards to activity display on rounds

Made it so database field widths can be fixed width - this is governed by contents of
statsgen2.ini file - you will see it if you look because i have not written gui edit
screen for this yet. If you completely delete the database (statsgen2.db) i.e. completely delete the file the next time statsgen re-creates it it will be using fixed
width fields.
Advantages - smaller database - faster processing (30% or 40% faster)
Disadvantages - some data may not fit into the fixed width fields (but i think i have made em all big enough)

### 1.8.7 5th March 2009
- Updated to cope with new format of COD5 time stamps
- To come ... adding ability to capture COD5 activities (bomb plants etc)

### 1.8.6 2nd February 2009
Upgraded config file with some more cod5 weapons and maps
FTP Downloads can now restart where they left off (default is to do this - can be switched off in the advanced server editor window)
FTP Downloads now take account of Max Log File Usage and only download the required amount
A silly green progress bar on various Processing Steps Lines - I know it looks a bit cack and i will tidy it up but i only coded it 3 hours ago because i wanted to watch a film and see from a distance how far statsgen was progressing

Too Many COD5 Team Kills - If you run a cracked server then GUID's for cod5 will often be the same as each other - this causes the team name guessing to go astray and assign everybody the same team (and therefore lots of teamkills) - there is an option on the server editor (advanced) screen to be able to switch this behaviour off - you will not be able to identify teams but no more teamkills will be generated. The default behaviour is to have the team guessing switched ON

Upload Template Facility - Updated to allow for the usage of a configuration file as well - this means i can host Omega's rather stunning COD5 template

### 1.8.5 22nd November 2008
- Upgraded config file with some more cod5 weapons and maps

### 1.8.4 12th November 2008
- Upgraded config file with some more cod5 weapons and maps
- Added ability to get new templates from statsgen website

### 1.8.3 2nd November 2008
- Upgraded default template again to do awards properly so that it does not include dropped players (oops)
- Added a skill and score weighting possibility dependent on the gametype
- Added a new award for ww2 assault guns (not sure how i forgot that one)
- Added a few more cod5 weapons as well as all the cod5 gametypes and maps i have been able to decode from the beta fastfiles (10 maps in total for mp i think)

### 1.8.2 30th October 2008
- Added COD5 World At War capability

### 1.8.1 28th October 2008
- Fixed a screw up

### 1.8 28th October 2008
- Updated template processing routing, error checking is better and it is a little faster
- Updated bug regarding teamkills
- Updated so COD4 has a good stab at working out teamkills
- Updated GUIS to allow for Configuration Wizards or standard guis for several sections
- Updated Initial installation to use First Time Configuration Wizard  - should make life easier on installation
- Updated default template to be like the Omega template slightly modified to be more statsgen friendly
- Provided a Image Pack configuration gui - this gives you a suggested choice of imagepacks on the statsgen site and uploads them automatically to your website
- Probably a few more issues that i can't remember

### 1.7.8 Beta 27th Jan 2008
- Fixed the last of the alias issues. NOTE: Please delete the aliaslist.dat file as it is likely that aliases have been corrupted
- Fixed issue with server messaging where it would stop after a statsgen run

### 1.7.7 Beta
- Fixed lots of issues with aliases
- Player Data (avatar, real name, mugshot) - this should now work properly

### 1.7.6 Beta Release 21st January 2008
- Player Data (avatar, real name, mugshot) - this should now work properly
- Cheaters list should keep updating rather than keep deleting the old values (oops)
- Award Names can contain ' symbols now.
- Some other stuff i can't remember

### 1.7.5 Beta Release 6th January 2008
**Changes / Fixes**
- COD4 PAM MOD support - PAM MOD was most annoying in that is logged things in completely the wrong format, logged some things twice, logs players with only partial guid's - the list goes on - but anyway, I've adapted statsgen to cope with PAM mod now (hopefully)
- Upgrading re-worked - I can now supply database upgrade configuration files - if ever i need to supply a "database contents" patch.

### 1.7 Full Release 3rd December 2007
**Changes / Fixes**
- BIG change - backup your old statsgen2.exe and templates folder in case of problems
- Quick Template ability removed (redundant with persistant db)
- Persistant Database:
Statsgen now uses a persistant database. What this means is that it will continue from where it left off
each time it runs - This massively speeds up Logfile Processing, Database Writing, Score Calculation and Template Processing.
If you are upgrading it will remove your old db and switch it to a file based version if necessary
Memory usage is drastically reduced
On a first run through you will not notice any speed ups as it has to run through the template processing
at least once fully - from then on it will be a lot faster - only updating players that have changed.
Alias Editing - once a player has been combined together you cannot unjoin them in the existing db (only
for new rounds will the separation start)
Existing rounds - if you change the minimum players / round to register then existing rounds in the db will
not be changed (this may come later if i work out how to do it efficiently)

### 1.6 Full Release 11th November 2007
**Changes / Fixes**
- Addition of COD4 Processing

**Coming soon(ish)**
- statsgen acting as a web server - allow you to configure it using web pages
- statsgen console only - no need for gui - can configure using web server capability
- statsgen linux - soon as I get the above stuff working

### 1.5 Full Release 10th September 2007
**Changes / Fixes**
- Addition of Quake Wars Processing (see later for how to configure for Quake Wars)
- Better handling of Archive / Active files
- Updated default template to cope with Quake Wars
- Handled "xxxx already exists" database errors (i.e. I've ignored em - they don't matter - just scare people)
- Dropped Indexes prior to database writing (should speed things up on larger databases)
- Re-created indexes after database writing
- Added extra xp tables (used with Quake Wars)
- Added ability to have XP as part of an award
- Added ability to have Team Kills as part of Awards
- GUI's hide / disable options that are not appropriate (e.g. remote directories hidden if ftp is disabled)

**Quake Wars**

Firstly - Quakewars logfiles are a complete t**t. The kill files contain hardly any info, they don't log activities
such as bomb plants, kill locations (head shots), bridge building etc. Player names are logged without the colour
codes. If a player has their clan tag set then the kill logs don't include the tag. 
But .... worst of all ... the logs are split into multiple files - kills are logged in an optional kill file,
but map, game type details and xp points are logged in an objective file.
You need to configure your quakewars server to store the kill file with the following config entries
logfile "1"
logtimestamps "1"
logfilename "kills.log"
The actual kill file (on windows - i have no idea for linux servers) will be stored in your 
	"My Documents\Id Software\Enemy Territory - QUAKE Wars Beta\base\kills.log" file on your server
This file needs to be selected for your Remote Active Logfile on your server editor screen
The objective file is stored in your
	"Quake Wars Server installation folder\base\objective.log"
This file needs to be selected for your Secondary Active Logfile on your server editor screen
You must configure both logfiles within statsgen for Quake Wars kills to be processed properly in Statsgen

### 1.4.4 Unreleased Full Release 7th July 2007
**Changes / Fixes**
- Fix in default template

### 1.4.3 Unreleased Full Release 7th July 2007
**Changes / Fixes**
- Fix in default template: Recognises no team loglines for cod 2

### 1.4.2 Unreleased Full Release 29th June 2007
**Changes / Fixes**
- Speed ups - Lots of optimizations - upto 70% faster logfile processing
- Auto Aliasing - now ignores WOLFENSTEIN and MOHAA automatically cheaters database table - all players found in the logfile who are marked as cheaters are added to the cheaters table (new table)
- Default Template - updated to show list of found cheaters

### 1.4.1 Unreleased
**Changes / Fixes**
- Crash when round was empty in an unusual way

### 1.4.0 Full Release 9th June 2007
**Changes / Fixes**
- Sometimes ban files would not download properly. I don't know why they were failing but i have put some code in to work around it.
- Some more tidying up of the "Time To Go" text
- Added ability to Automatically Alias players during logfile processing. Basically it uses the GUID to work out which players are really the same person, adding them to the alias list - any existing aliases are kept as is.
- You can now add the command line argument `-runonce` e.g. `statsgen2.exe -runonce`. This will cause statsgen to start, perform a stats run, then exit. This is designed to be used by people who want to run statsgen on the Windows Scheduler system.

### 1.3.3 Full Release 9th April 2007
**Changes / Fixes**
- Message Center - Punkbuster Messaging now works, I made a several mistakes in this area :-p.
- Editor Screens - Image/Score/Skill editor screens now show the original code key as part of their display
- Thread Safe - The program is now (I hope) thread safe
- Screen Updates - The screen udpates properly now during processing
- Screen Updates - Fixed some issues where negative "Time To Go" info could be displayed
- Screen Updates - Fixed some issues when multiple logfiles caused "Time To Go" to be miscalculated
- Speed Ups - Massively sped up processing if large drop lists were used (e.g. if you used punkbuster bans)
- Speed Ups - Added a few more indexes to speed up the BF2 template
- Logfile Processing - Extreme MOD activities (ctfb_capture) now decoded
	
### 1.3.2 Full Release 15th March 2007
**Changes / Fixes**
oops - I made a mistake and managed to disable website transfer in the last release, and, to cap it off, I managed to include a bad default database config.

### 1.3.1 Full Release 10th March 2007
**Changes / Fixes**
- File menu renamed to Run.

You now have the ability to change the Run Priority of the quick and full templates. This should help
anybody who is running the program on a dedi box and is experiencing lag on the server as statsgen
steals the cpu. The lower priority setting the less statsgen will steal cpu.

### 1.3 Full Release 3rd March 2007
**Changes / Fixes**
- Fixed some issues with the MySQL uploading
- Added speech time to the speech log
- Added some more database indexes to speed up some of the templates

### 1.2 Full Release 13th February 2007
**Changes / Fixes**
- Changed the default config to be the No Rounds version - This is a much leaner template which takes less processing time
- Awards can now be "weighted" in conjunction with the template
- Skill can now optionally have the negative component caused by deaths switched off
- Default template now has score, skill and award details information page
- MySQL support. Statsgen can now optionally upload it's internal database to a remote (webserver) MySQL database: I will be developing dynamic versions of the current templates which make use of this feature. When in place this MASSIVELY speeds up processing as transfer loads and template processing will be significantly reduced.
- Some extra awards added to use some Admiral Mod features and the associated images also added to the image pack. Thanks again to Omega for doing these - awesome job mate.

### 1.1.2 Full Release 4th February 2007
**Changes / Fixes**
- Fixed an issue where award components could not be deleted in the config file

### 1.1.1 Full Release 3rd February 2007
**Changes / Fixes**
- Made a slight mistake in the automatic installation detection. Upgraders from 1.1.0 only need replace the executable.

### 1.1.0 Full Release 28th January 2007
**Changes / Fixes**
- Extended character sets (Cyrillics etc) now read properly from the logfile
- Skill Calculation - Can now be configured to not allow negative skill (no loss from death)
- Added Free For All as MOHAA Deathmatch Mode to INI File
- Upgrade File & Procedure now supplied
- Database Structure Updated: Rounds / View Rounds (updated with server index round created in), Award Definition (Award ID added). This affects the Message Centre Key used- during stats production (no longer changes when name changes)
- Default Templates - Various Updates / Fixes
- File Downloading Multiple Files - Better handling of weird servers that don't give me a wild card directory listing
- Admiral Mod - Healing activity now detected
- COD1/COD2 Banfile - These are now read and used
- Speech File Output - You can now configure statsgen to write out all the speech in the logs in the Mandatory/Output Config
- Error Boxes - The Save Button is now hooked up so you can save descriptions of errors - this is useful to me if you get problems - you can send me the error details instead- of posting images on the forum
	
### 1.0.2 Full Release 18th January 2007
**Fixes**
- Some FTP Servers dont seem to like getting a directory listing with a wildcard and this prevented download of log files. I have made it so that if it encounters this problem it tries to blunder past it anyway - you will see a caution symbol on the download logs entry to warn you this is happening.

### 1.0.1 Full Release 6th January 2007
**Fixes**
- I chumped up the initial release. The "Quick Template" extracted upon install was and invalid testing one.

### 1.0 Full Release 1st January 2007
**Fixes**
- Server Updates did not get reflected until statsgen restarted
- Website Updates did not get reflected until statsgen restarted
- Message Center Updates did not get reflected until statsgen restarted
- Better "file not found" handling
- HTML Safe if template codes passed through ColouredText2HTML() function

**Additions**
- Schedule Editor
- Better transfer failure error logging
- Negative durations on round corruption fixed
- Banned Speech Editor
- MOHAA Support
- Enemy Territory Support
- Omega's Updated Templates
- Help Files

### 0.2 Alpha 17th November 2006
**Fixes**
- Remote File Browser fixed (doh!)
- Negative durations on round corruption fixed
- Team Swap Suicides (team balance) now separate from normal suicides
- Various bugfixes to default templates
- Message Centre GUI tidied up

**Additions**
- Punkbuster Messaging Added
- Default Templates: More configurable options for Colours, Homepage, Homepage banner, Avatars, Player Profiles (mugshots, clan position, xfire address etc)
Ratio's of Kill / Deaths. 
- Player Data: Player Data are personalised player options including Avatars, Player Real Name, Clan Position (War Arranger etc), Personal Picture (mugshot), Xfire address, Personal Homepage

**Things still to do**
- Help Pages
- Schedule Config
- MOHAA support

### 0.1 Alpha 1st November 2006
- Default Templates not 100% perfect.
- No help files yet.
- No Avatars yet.
- No Speech Filter yet.
- Speedups possible.
- More award images necessary.
- Only COD1 and COD2 supported.
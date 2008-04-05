Lakitu7's Tremulous 1.1 QVM
Current Version: 5.1
From SVN Revision 966

Downloads: 
http://www.mercenariesguild.net/patches/?do=details&task_id=122


While playing on and admining Tremulous servers, I've found a distinct lack of a server package that 
a) Contains all of the features that I want
b) Doesn't have any nonsense that ruins the game

Thus, like many server owners, I began the task of making my own for the {SGA} Server. While I originally intended to just put something together from existing patches, I quickly found that patches for many things that I wanted either didn't exist, weren't available, or weren't available separate from a ton of other things that I didn't want, or couldn't be applied together.

By the time I'd finished tracking down, separating, and creating my own patches, I'd done enough work to feel that I should distribute this QVM. Thanks to the encouragement of my friends asking for it, I've finished getting this to a releasable state.

As an overview, my QVM includes EVERYTHING in tjw's 1.1 QVM (not the balance changes tested on trem.tjw.org nowadays, but the things here: http://tjw.org/tremulous/). It also includes a number of the more popular enhancements found in other packages (avenger, risujin, *wraths') and things that I have written myself for features and bugfixes that I wanted.

If you do decide to use this, I would appreciate leaving a post here to tell me the name of your server. Then I can know to tell you about updates or bugs or whatever if I see you in game/irc. 

--------------------
Upgrade install:
Just drop the new game.qvm in place of the old one. Set new admin flags and cvars where applicable.

--------------------
Installation:

Easy way: 

Follow TJW's instructions at http://tjw.org/tremulous/SETUP.txt, but wget a URL for my QVM instead of his. My QVM DOES use TJW's Tremded executable, so get it from http://tjw.org/tremulous/

Harder way:
Download TJW's tremded, as above
Compile my qvm yourself:
Download SVN revision 966 (svn svn://svn.icculus.org/tremulous/trunk -r966)
Apply my .patch
make

If you're using linux, you should already know how to compile things. 

If you're using Windows, use either of the excellent build guides at:
http://tremulous.net/phpBB2/viewtopic.php?t=3408
http://tremulous.net/phpBB2/viewtopic.php?t=2672

--------------------

Contact Information:
Forums: The Lakitu7's 1.1 QVM thread, tremulous.net forums, mods subforum
IRC: Lakitu7 on Freenode, Dorksnet, or Synirc
Ingame: }MG{Lakitu7, probably on Beergarden or Puretrem

--------------------
FAQ:

Q: I want a QVM with features x and y, but not z. Will you compile one for me?
A: Unless you're a good friend of mine and you have a darn good reason why you can't do it yourself, no. If you're running Linux, I'm sure you know how to compile your own anyway. If you're running Windows, see either of the excellent build guides linked in "harder way." If you ask nicely and put forth the effort to learn to do things yourself, I may help you, but I won't do it for you. The only thing I put here that's really all that controversial is SD Votes and those are both controlled and disablable.

Q: I don't want your QVM, but I do want patch X from it
A: I've made EVERY patch that I have used available here: http://browsingtheinternet.com/trem/patches/
 

Q: I found a bug.
A: Well damn, tell me. Use the contact information above and I'll fix it. If it's part of a patch that I didn't make, then I'll still fix it, or I'll whine at whoever did to to fix it instead :). 

Q: I have a suggestion for a new feature.
A: Maybe. If I don't see myself using something, I won't code it for you. But if the idea is good and I'd like to have it, perhaps. BUT, I have no interest in doing any fullscale mods that make sweeping gameplay changes. I do not have any interest in making any "balance" changes. If your suggestion is nice and can be implemented with a reasonable degree of effort, I'll think about it.

Q: You should add the Protoss/Predator/MDriver Turret/M4A1 Abrams Tank/BFG9000
A: Hay man that's so cool how about you make it yourself and then post it on the forums.

Q: Update this for newer SVN revisions
A: This was already a ton of work. Maybe. It depends on how many things get into SVN that I want to include BEFORE 1.2 comes out. Then I may or may not release something for 1.2.

--------------------
The long, long list of patches/features:

I have mirrored every patch that I've used here:
http://browsingtheinternet.com/trem/patches

The ones that I have made I've also mostly submitted to http://www.mercenariesguild.net/patches

You may use any patches that I have made for whatever you wish, but as with all GPLed code, follow the license: you must keep it opensource.

Other patches are available from their given sources

This listing has no real order anymore

Patches that have multiple things are together because they don't apply together in their seperate forms. I have linked their original separated versions where applicable.

--
svn_950_to_tjw_s_tremded.patch
Description: Backports SVN code to be compatable with 1.1 clients. QVMs generated are to be used with TJW's Tremded
Author: vcxzet
Source: http://www.mercenariesguild.net/patches/?do=details&task_id=2
--
tjw-no_chomp_during_pounce.patch
tjw-poison-system.patch
Description: Balance changes from TJW's 1.1 QVM. You're likely used to all of them
Author: TJW
Source: http://tjw.org/tremulous/ (separated out by me) Info here: http://tjw.org/tremulous/FEATURES.txt
--

Lakitu7-tjw-avenger-martindoucha_sdmodes+sdvotes+poll+designate959.patch
Description: g_suddenDeath, Sudden Death Modes, Poll (formerly custom), !Designate, vote designate. Denybuild and vote mute are now in SVN and thus are also included here. 


SDModes: Adds different methods of sudden death. 
Author: TJW
Source: http://tjw.org/tremulous/ (separated out by me) Info here: http://tjw.org/tremulous/FEATURES.txt

SD Votes: Allows /callvote sudden_death
Author: Avenger
Source: https://bugzilla.icculus.org/show_bug.cgi?id=2920 (modified heavily by me)

g_suddenDeathVotePercent
Author: me
Percentage required to vote yes for Sudden Death votes to pass. This is in an effort to balance them. Set to 0 if you want to DISABLE them entirely. Defaults to 75

g_mapVotesPercent
Author: me
Percantage required to vote yet for map, nextmap, and map_restart votes to pass. 

Poll Votes: /callvote poll String goes here
Allows the calling "poll" votes. Before r858, people would use /callvote map "Is unnamedPlayer an idiot?" Since that's diabled now by a check to verify that "is unnamed player an idiot.bsp" exists before calling the vote, I replaced it with poll. They DO nothing, but can be useful for an admin to ask a question of users on their server.
Also has /callteamvote poll String goes here for things like /callteamvote poll Move to window room?  Idea for teamvote version thanks to Benmachine. Quotes no longer need to be places around the text.
Author: Me
As of 5.1 you no longer automatically vote yes to poll votes

Designate: Creates designated builders. Structures built by designed builders (or /protect -ed by them) cannot be deconned except by then or by another designated builder. Admins with flag # are perma-designated. Designate votes for players are enabled with g_designateVotes 1.
Author: Martin Doucha (ported by me)
Source: https://bugzilla.icculus.org/show_bug.cgi?id=2857

--
dretchPunt+no_pounce_trample_ff.patch

Dretchpunt: everyone knows what this is already
Author: TJW
Source: http://tjw.org/tremulous/ (separated out by me)

No Pounce and Trample FF: turn frindly fire on pounce/trample on and off by g_friendlyFireMovementAttacks. Irrelevant if g_friendlyFire is off.
Idea: Norfenstein/TJW. Rewritten for 1.1 by me
Source: trem.tjw.org server autodownload

--
mnp-share+risujin-donate.patch
Authors: Martin Doucha, Risujin
Source: http://tremulous.net/phpBB2/viewtopic.php?t=3014 , https://bugzilla.icculus.org/show_bug.cgi?id=2896
Please turn these off. They ruin the game. g_allowShare 0 will turn off BOTH of them. I just have them because they're so damn popular that I can't not do it anymore. 

--
002.tjw.www_download.patch
009.peoro.building_messages.patch
Lakitu7+peoro-change_team_messages.patch (peoro's patch, now with logging to games.log)
014.peoro.namelog_shows_prohibition.patch
017.npa.privmsg_to_console.patch
022.kristall.nojoin_warmup_time.patch
027.napkin.more_info_to_flag4.patch
034.risujin.ckit_recovers_with_blaster.patch
031.napkin.log_to_file_only.patch
032.wolfwings.tremstats_log_team_class.patch
035.pierre.realtime_stamp_at_mapstart.patch

All of these are from  http://wraths.dyndns.org/games/tremulous/patches/svn901 and their authors/functions are obvious from the filenames. Several notes:

Stickyspec: Camera doesn't change to a new person when the one you're specatating dies
I'd use my own version, but it requires client side mods too. Developers, Please, please put this into SVN.
https://bugzilla.icculus.org/show_bug.cgi?id=2792

Black Names Fix: Fixes the bug with white ^ and black names. Black names are disallowed by this fix, as was intended by the original code.

--
Lakitu7-disable-difficult-names.patch
Disallows names that are prolematic: those that begin with numbers or contain // or /*
Author: me

--
Lakitu7-listadmins_minlevel_parameter.patch
Listadmins takes a second number parameter allowing you to filter results by admin level
Author: me  Requested by: many people
Examples:
!listadmins 0 2
Lists all admins of level 2 and up, skipping none
!listadmins 21 2
Lists all admins of level 2 and up, starting at number 21.
!listadmins lak 3
Lists all admins with containing "lak," level 3 and up.

--
Lakitu7-mystats.patch
Adds the /mystats command. Players can use it once per minute and during intermission
Shows a variety of useful personal statistics that are tracked per-game such as builds, building kills, damage dealt, feeds, etc.
Author: me

--
Lakitu7-register.patch
Adds the !register command with admin flag R. When given to L0s, it allows them to become level 1 with !register. If they don't have a GUID, it tells them where to get one. 
When given to level1 and up, it allows them to update their "admin name" to their current one by !setlevel-ing them to their current admin level.
Author: me

--
Lakitu7-sayadmins3.patch
I can't think of how many times I've wanted this feature, so I went ahead and made it. Admins frequently want to talk amongst themselves to make a decision without the whole server hearing it. 
Adds commands: 
/say_admins, /a
It also works in chat and in server console/rcon
Use of the command and receipt of its messages is governed by the cflag ?, given to levels 3, 4, and 5 (via *) by default.
Server owners can choose whether or not regular players can use the command to send messages to active admins. This is set by g_publicSayadmins
Author: me

--
Lakitu7-sees-full-listplayers-flag.patch
Adds an admin flag $. People with this flag see the regular information in !listplayers. Those without only see ID, Team, mute/denybuild/etc., and Name. Makes it more difficult for random players to tell if there are admins present, removing the need for servers that have either removed !listplayers from l0s (bad idea) or given all admins the hide flag (which has the side effect of making them invisible to each other too)
Author: me

--
gate-!warn.patch: !warn user reason
Author: Gate
Displays a big ol' message to your target that tells them that an admin is warning them for reason: reason and they should stop whatever they are doing and comply. I feel this is a much less abusable alternative to !slap

--
foobar-NewbieNaming-0.90-899.patch: Auto-renaming of unnamedPlayers to your choice + numbers
Author: Foobar
http://tremulous.net/phpBB2/viewtopic.php?t=3738

--
r1ch-unnamedplayer_join_warning.patch
Prevents unnamedplayers from joining a team, instead telling them how to set a name. This does not affect players who have been renamed by newbienaming, as it is an alternative to newbienaming
Author: R1CH (ported by me)
Source: svn.quakedev.net

--
benmachine-info964-10.patch
Adds !info <topic> admin command, usable by those with flag H. It Returns the text of a file from /info/info-topic.txt. See source and docs for more info.
Docs: http://www.bandstand.org.uk/~benm/coding/done.php#buildlog-revert
Author: benmachine
Source: http://www.mercenariesguild.net/patches/?do=details&task_id=43

--
risujin-retribution.patch: Teamkillers pay their victims for the price of their equipement. 
Author: Risujin
Source: http://tremulous.net/phpBB2/viewtopic.php?t=3014  Separated out by me

--
Lakitu7-admin-true-name-in-banlog: 
Save admin's "true" name in the ban logs. "True" name is the one that shows up in !listplayers when they are using something else
Author: Me

--
lakitu7-devmapNoGod+devmapNoStructDmg.patch
g_devmapNoGod: Disables use of /god, /noclip, and /give health during /devmap. Useful during practices
g_devmapNoStructDmg: Prevents damaging structures during /devmap. Useful during practices
Author: Me

--
lakitu7-g_minLevelToSpecMM1.patch: Spectators with admin level < g_minLevelToSpecMM1 can only speak to other spectators. (/say_team, but not /say) Combine with g_privateMessages 0 to be uninterrupted by spectators during clan matches
Author: Me

--
lakitu7-g_minLevelToJoinTeam.patch: Only users with admin level > g_minLevelToJoinTeam can join a team. Otherwise, they are forced to spectate. This can be used to quickly create a private server. 
Author: Me

--
lakitu7-l1command.patch: Adds command !L1, usable by admin levels with flag S. !L1 user sets a level 0 user to level 1. Made to be used with g_minLevelToJoinTeam.
Author: Me

--
lakitu7-restart_keepteams_switchteams+lock.patch: Adds !restart keepteams, !restart switchteams, !restart keepteamslock, !restart switchteamslock
Author: Me
Useful for clan matches when you want to set things up, then !restart the round while keeping people on the teams they've just joined. Put "lock" afterward to auto-lock teams after the restart

--
avenger-mute_enhancements.patch
Muted people can't change names, vote, or send PMs (or get around that with /tell)
Author: Avenger (consolidated by me)
Source: https://bugzilla.icculus.org/show_bug.cgi?id=2931

--
avenger-say_area.patch
Adds /say_area to send text only to teammates that are nearby. Use it for binds that aren't relevant to people not near you (nade!, retreating to base! etc.)
Author: Avenger
Source: none?

--
benmachine-notarget948-1.patch
Makes the /notarget cheat work during devmap by having you be ignored by turrets and such when it's on.
Author: benmachine
source: https://bugzilla.icculus.org/show_bug.cgi?id=3243

--
risujin-svn929-admin_layout.patch
Adds !devmap and !savelayout back to g_admin. Works with the existing implementation of !layout in SVN
Author: Risujin
Source: http://v1.risujin.org/pub/tremulous/admin_layout-svn929.patch.tar.gz

--
benmachine-me-action939-4.patch
Adds IRC-like /me command. It's useless, but fun. g_allowActions disables it. g_actionPrefix changes the text that appears before an action to denote that it is an action.
Author: benmachine
Source: http://www.mercenariesguild.net/patches/?do=details&task_id=5

--
avenger-g_maxGameClients.patch
Do not allow more than g_maxGameClients to join a team. Connected clients beyond this amount can only spectate, creating spectator slots, R1Admin style.
Author: Avenger
Source: https://bugzilla.icculus.org/show_bug.cgi?id=2969

--
benmachine-antiblock940-5.patch
Forcibly bumps players out of the way when they are blocking the spawn. This patch is disabled by default, for now. Set g_antiSpawnBlock 150 to try it out.
Author: benmachine
Source: http://www.mercenariesguild.net/patches/?do=details&task_id=70

--
philbordelon-floodprotection+lakitu7-enhancements.patch
Prevents flooding of chat commands. Controled by g_floodMinTime. 0=off. Default: 2000. 
Admins with admin permission #2 (immune to flood protection & censorship) are immune. This permission was already in the admin system, but didn't do anything because it wasn't implemented yet. All forms of /say are protected by flood protection, along with PMs and admin commands. g_floodMaxDemerits lets you fine tune the harshness of the protection somewhat. See the source link for details. I defaulted it to 5000.
Author: Phil Bordelon (Updated and given a few more features by me)
Source (Original): https://bugzilla.icculus.org/show_bug.cgi?id=2811

--
lakitu7-g_deconDead.patch
Adds g_deconDead, which controls the ability to decon structures that have 0 health and are about to explode, causing damage. This was possible before g_markdeconstruct (r880), but was regardless of g_markdeconstruct's setting. g_deconDead defaults to 0 (svn behavior, cannot decon dead structures). g_deconDead reverts to the pre-880 behavior.
Author: Me

--
r1ch-team_imbalance_warnings.patch
Sends a notice every g_teamImbalanceWarnings when the playercount between teams differs by at least 3. It also allows people to switch teams without losing their credits/kills during this circumstance. 
Author: R1CH (ported by me)
Source: svn.quakedev.net

--
avenger-adjustban-r948.patch
tjw-tremulous-svn939-ban.patch
Fix the SVN 910 ban bug. !Adjustban command is now needed to change ban lengths on existing bans
Author: Avenger, TJW
Source: https://bugzilla.icculus.org/show_bug.cgi?id=3096

--
Lakitu7-admfforceteamchange_overrides_locked_teams.patch
Admin flag 5 can override locked teams
Author: me

--
Lakitu7-callvote-multiple-name-matches.patch
Makes /callvote return a list of partial name matches when there are multiple matches for a string
Author: me

--
Lakitu7-denybuild-denies-grangerclasses.patch
Patches !denybuild so that it denies targets from using the granger/advgranger class as a whole so that it can be used against battle grangers. (As of 5.1, this actually works)
Author: me

--
Lakitu7-devmapKillerHP.patch
When you die, tells you your killer's HP... but only in cheat mode (devmap) because I don't believe this should be given in real games. set g_devmapKillerHP 1 to enable.
Author: me

--
Lakitu7-g_adminSayFilter.patch
When g_adminSayFilter 1, filters display of !commands that were typed into the chat. 
Author: me. Idea: CU|Dyin

--
Lakitu7-g_clientUpgradeNotice.patch
When g_clientUpgradeNotice 1, it informs players without a GUID that they should upgrade their client to receive faster map downloads and obtain a GUID, along with a link to where to get it (tjw's site). Notice is sent at connect and once per map change.

--
kevlarman-specme999.patch: !specme
Author: Kevlarman
I modified this to !specme instead because that seems to be what most people expect it to be called. Personally, I don't see why people can't just use /team spectate, but it's requested a lot, so here it is.

--
trem-servers-prefixes for flag4.patch
Adds [H][A][S] prefixes to team chat visible to admins with flag 4
Author: www.trem-servers.com
Source: ??

--
Lakitu7-remove-boost-warning-messages.patch
Removes "must be on humans / must be alive to use this command" messages for BOOST
Author: me

--
Lakitu7-luci-exploit-fix.patch
My hacky, but working fix for Bug 3302
Author: me

--
das-iro.no-shove-for-placing-builder.patch
Prevents shoving the builder when he is trying to place a building. No effect when g_shove is off.
Author: Das-Iro
Source: http://das-iro.de/tremulous/

--
Lakitu7+Amanieu-admin_messages.patch
Sends admins (those with the say_admins flag) a notice informing them when someone attempts to callvote against an admin, or tries to use an admin command they lack access to. When a teamvote kick or denybuild is called, admins not on that team also receieve notice of it via this system. 
Author: Amanieu, Me
Idea: Collected

--
Lakitu7+benmachine-g_decolourLogfiles.patch
Strips colors from games.log and admin.log to make them more human readable, machine searchable and take up less space. Controled via g_decolourLogfiles. 0 = off. 1 = on, but adds an extra parameter to clientconnect and clientrename messages that contains the names still in color. I hope that stats script can be updated to use this to allow them to continue to have colored names. 2 = on, without the modification to those lines by 1. This is "compatability mode." Use it if your existing stats script breaks (but you don't mind it being decoloured, if applicable). Regardless of which option you use, this patch fixes color bleeding in console messages when using ttycon_ansicolor
Author: Benmachine, Me

--
Lakitu7+benmachine-pause.patch
Adds the !pause command that allows you to pause and unpause the game in progress. People can only chat and use admin commands, while the game is paused. This is mostly useful in conjunction with buildlog/revert. 
Author: Benmachine, Me
Idea: Benmachine

--
Lakitu7+tjw-teamkill_messages_with_dmg_done+logging.patch
Teamkill messages now have the amount of damage done by the killer to the killed. This is a sum over multiple hits, if applicable, and can be larger than the player's max health if it was done throughout healings/regen. This is so that admins and players alike can differentiate between "grazed your foot with a rifle" teamkills and "hit you in the head repeadly with a luci" teamkills.
Author: tjw (original tk messages patch), Me

--
Lakitu7-console-ignores-@-in-listplayers.patch
Console/rcon doing !listplayers is not affected by admins with flag @
Author: me

--
Lakitu7-ff_percentage.patch
Friendly Fire cvars (g_friendlyfire, g_friendlybuildablefire, g_friendlyfiremovementattacks, g_friendlyfirehumans, g_friendlyfirealiens) are all now floating points. 0 is still off. 1 is still on, so you do not have to change anything to upgrade. However, values between 0 and 1 will multiply the damage by that amount, allowing friendly fire to be, for instance, on but half damage. 
Author: me

--
Lakitu7-log_callvotes.patch
Lakitu7-log_stageups.patch
Lakitu7-mirror_adminlog_to_gameslog.patch
Logs stage inreases, vote (and teamvote) calling, and vote (and teamvote) final results to games.log. Individual players' votes are NOT logged. Use of admin commands are also shown in games.log 
Author: me

--
Lakitu7-teamvote_majority_wins.patch
Teamvotes as they are in svn / as they were required a majority of people ON THE TEAM to pass. This patch makes them pass if the majority of voters voted yes, instead of having votes fail 4 yes - 1 no because the majority of the team didn't vote. At least 2 yes votes are still required to prevent a vote from passing 1-0. 
Author: me

--
Lakitu7-vote2teamvote.patch
When a player uses /vote and there is a teamvote occurring, but no global vote, forward their vote as a /teamvote. I hate making and including this patch, but it's a necessary evil because most players terribly fail at using anything that's not bound to a key by default. This won't be necessary anymore in 1.2, but for now, it's so that teamvotes actually have a prayer of passing, especially when combined with the above. 
Author: Me

--
Rociante-vote_times3.patch
Players cannot call a vote until they have been connected for g_voteMinTime seconds
Players cannot call for a map change after g_mapvoteMaxTime seconds
For both, setting the cvar to 0 disables. The restrictions are ignored for admins with no_vote_limit and when the player is the only one on the server. This patch is to cut down on the number of idiots that join a game just to spam a vote for atcs. 
Author: Rocinante

--
tjw+mkp+lakitu7+r1ch-follow-teammates-when-dead+stickyspec.patch
When they are dead (and while in the spawn queue), players can spectator-follow their teammates. To activate, press button3 (by default, return or mouse3). This patch also includes sticky spectator functionality (which I had to mostly rewrite to work with dead-spec). 
Idea: tjw, R1CH
Author: tjw, Undeference, R1CH, Me

--
benmachine-cp1003M-1.patch
Console/rcon's cp command now parses \n into newline and \\ into \
Author: benmachine

--
benmachine - buildlog-17.patch
Benmachine's masterwork, !buildlog and !revert: both improved for funtionality and usability and included here officially for the first time. I suggest using these while the game is !paused, while you get the hang of them. See Ben's docs here: http://www.bandstand.org.uk/~benm/coding/done.php#buildlog-revert
Author: benmachine


--------------------
Appendex A:
COMPLETE list of admin commands and flags:
* 1 - cannot be vote kicked, vote muted
* 2 - cannot be censored or flood protected TODO
* 3 - never loses credits for changing teams
* 4 - can see team chat as a spectator
* 5 - can switch teams any time, regardless of balance
* 6 - does not need to specify a reason for a kick/ban
* 7 - can call a vote at any time (regardless of a vote being disabled or voting limitations)
* 8 - does not need to specify a duration for a ban
* 9 - can run commands from team chat
* 0 - inactivity rules do not apply to them
* ! - admin commands cannot be used on them
* @ - does not show up as an admin in !listplayers
* $ - sees all information in !listplayers
* # - permanent designated builder
* ? - sees and can use adminchat 
* B - !showbans display a (partial) list of active bans
* C - !time show the current local server time
* D - !listadmins display a list of all server admins and their levels
* G - !readconfig reloads the admin config file and refreshes permission flags
* H - !info read the servers information files
* K - !lock lock a team to prevent anyone from joining it
* K - !unlock unlock a locked team
* L - !devmap load a map with cheats (and optionally force layout)
* L - !layoutsave save a map layout
* L - !listlayouts display a list of all available layouts for a map
* M - !map load a map (and optionally force layout)
* N - !rename rename a player
* P - !spec999 move 999 pingers to the spectator team
* R - !register Registers your name to protect it from being used by others or updates your admin name to your current name.
* S - !pause Pause (or unpause) the game.
* U - !buildlog display a list of recent builds and deconstructs
* V - !passvote pass a vote currently taking place
* a - !admintest display your current admin level
* b - !ban ban a player by IP and GUID with an optional expiration time and reason.
* b - !adjustban change the length or reason of a ban. time is specified as numbers followed by units w (weeks) d (days) h (hours) or m (minutes) or seconds if no units are specified
* b - !unban unbans a player specified by the slot as seen in showbans
* c - !cancelvote cancel a vote taking place
* d - !allowbuild restore a players ability to build
* d - !denybuild take away a players ability to build
* e - !namelog display a list of names used by recently connected players
* g - !designate give the player designated builder privileges
* g - !undesignate revoke designated builder privileges
* h - !help display commands available to you or help on a specific command
* h - !specme moves you to the spectators
* i - !listplayers display a list of players their client numbers and their levels
* k - !kick kick a player with an optional reason
* l - !L1 Sets a level 0 to level 1
* m - !unmute unmute a muted player
* m - !mute mute a player
* n - !nextmap go to the next map in the cycle
* p - !putteam move a player to a specified team
* r - !restart restart the current map (optionally using named layout or keeping/switching teams)
* s - !setlevel sets the admin level of a player
* v - !revert revert one or more buildlog events
* y - !allready makes everyone ready in intermission
* w - !warn Warn a player to cease or face admin intervention


Default admin flags (only happen if you do not have an admin.dat)
L0: Unknown Player:
iahC 
L1: Server Regular: 
iahC
L2: Team Manager:
iahCpPwd
L3: Junior Admin:
iahCpPkmwd?$
L4: Senior Admin
iahCpPkmBbewd?$
L5: Server Operator
* (everything)


-------------------
Appendix B: New Cvars that were added by my patches:


g_lockTeamsAtStart: Used by !restart lock options. Don't mess with it yourself. I don't even think you can anyway
g_designateVotes: Default 0. 1 to enable /teamvote designate and /teamvote undesignate
g_clientUpgradeNotice: Default 1. 1 to enable, 0 to disable the notice
g_teamImbalanceWarnings: Default 30 (every 30 seconds)
g_adminSayFilter: Default 0 (off)
g_myStats: Enables /mystats. Default 1 (on)
g_publicSayadmins: Lets non-admins use say_admins to message active admins. Default 1 (on)
g_devmapKillerHP: Default 0 (off)
g_newbieNumbering: Default 0 (off)
g_newbieNamePrefix: Default "Newbie#"
g_suddenDeath: Used by SD votes. May break things if you modify via rcon manually
g_suddenDeathMode: Default 1 (off / default 1.1 behavior)
g_friendlyFireMovementAttacks: Default 1 for compatability, Recommended 0 (turns off pounce/trample damage)
g_retribution: Default 0 (off) 
g_suddenDeathVotePercent: Default 75. (3/4ths must vote yes to pass) Set to 0 to disable SD votes entirely. 
g_mapVotesPercent: Default 50. 
g_allowShare: Default 0. Also controls /donate. Please leave this off, it ruins the game :(
g_minLevelToJoinTeam: Default 0
g_minLevelToSpecMM1: Default 0
g_dretchPunt: Default 1
g_devmapNoGod: Default 0
g_devmapNoStructDmg: 0
g_maxGameClients: Default 0 (no limit)
g_allowActions: Default 1
g_actionPrefix: Default "***"
g_antiSpawnBlock: Default 0 (disabled). Set to 150 to turn on.
g_deconDead: Default 0 (disallow deconning dead structures)
g_floodMinTime: Default 2000. Set to 0 to turn off flood protection
g_floodMaxDemerits: Default 5000. Fine tuning of flood protection. See docs for more info.
g_decolourLogfiles: Default 0 (off). Settings: 1 (on) 2(on, stats script compatability mode). See docs for more info. 
g_voteMinTime: Players cannot call a vote until they have been connected to the server for at least this many seconds. 0 to disable. 
g_mapvoteMaxTime: Players cannot call a map vote after this many seconds of play. 0 to disable. 

-------------------
Appendix C: New Commands:

Commands: 
/me /me_team
/share
/donate
/say_area
/say_admins /a (same thing)
/protect /resign (designated builder stuff)
/mystats



----
Full Version Log: 

5.1: 
      Fixed share transfering credits incorrectly and thus breaking when deadspec was added and in use (thanks ==Troy==)
      Fixed error in g_decolourLogfiles / g_logOnlyPrintf (thanks Snapser)
      Fixed regression in clientteamclass (thanks Snapser)
      Added Lakitu7-teamvote_majority_wins.patch
      Added Lakitu7-log_stageups.patch
      Added Lakitu7-log_callvotes.patch
      Added Lakitu7+benmachine-g_decolourLogfiles.patch
      Updated Avenger-Adjustban to r1026 version (fixes not respecting some admin flags)
      Updated benmachine-info to version 11
      Added logging of the start of sudden death (Lakitu7-tjw-avenger-martindoucha_sdmodes+sdvotes+poll+designate959_4.patch)
      Added Lakitu7-vote2teamvote.patch
      Added teamchange messages to games.log (Lakitu7+peoro-change_team_messages.patch)
      Added Lakitu7+Amanieu-admin_messages.patch
      Fixed some other console messages color bleeding not covered by Ben's patch
      Added team prefixes for say_team messages in games.log
      Added Lakitu7-vote_messages_in_console.patch
      Added benmachine-cp1003M-1.patch
      Added !buildlog, !revert (benmachine)
      Added !pause (lakitu7 + benmachine)
      Fix typo in !warn (thanks Raytray)
      Added damage done to tk messages, add tks and building tks to games.log (Lakitu7+tjw-teamkill_messages_with_dmg_done4+logging.patch)
      Added Lakitu7-console-ignores-@-in-listplayers.patch
      Fixed denybuild-denies-granger-classes so that it actually works (my dumb mistake)
      Fixed !register so that console doing it doesn't crash the server anymore (thanks Das-Iro)
      Changed Poll votes so that the caller no longer automatically votes yes
      Added Rociante-vote_times2.patch g_voteMinTime g_mapvoteMaxTime
      Added follow teammates when dead + stickyspec
      Changed share/donate to be flood limited (thanks soultcer)
      Fixed flood limiting from breaking during !pause
      Fixed alphabetizing error in admin commands (thanks Benmachine)
      Changed say_area to be flood limited
      Fixed say_area to work with admf_spec_allchat
      Fixed say_area to not send redundant team prefixes

5.01: Fix !specme to remove build timer exploit (reported by Techhead)
      Fix /mystats so that headshots aren't broken as hell anymore
       (and added it back as a number in addition to the percentage)
      Add Das-Iro's patch to prevent shoving the builder when he's trying to build 
      Fix say_admins to remove the bug with spaces in the chat field (Thanks Rocinante, Mexel)
      Remove "must be on humans / must be alive to use this command" for BOOST
      Add luci exploit fix
      Prevent team imbalance warnings during intermission
      Better admin flags list in the readme (thanks Tuple)
5.00: Upgrade to SVN 966. SVN upgrade changes tons of minor things. See SVN for full information.
      	Some highlights: denybuild and mutevotes in svn now, renamed some other votes,
			 fixed playercounts in several things, fix for atcs hovel bug 
      New version of the backport patch
      Replaced logteamclass with Wraths' version to only log to the logfile (not to console)
      Add Avenger-Adjustban
      Add TJW-svn910 ban bug fix
      Remove SVN 910 ban revert(above two changes replace it)
      Add Avenger-Say_Area
      Update Benmachine-Info to latest version
      Add Benmachine-Notarget
      Add Lakitu7-admfforceteamchange_overrides_locked_teams
      Add Lakitu7-callvote-multiple-name-matches
      Add Lakitu7-g_clientUpgradeNotice
      Add Lakitu7-denybuild-denies-grangerclasses
      Add Lakitu7-devmapKillerHP
      Add Lakitu7-disable-difficult-names
      Add Lakitu7-g_adminSayFilter
      Change Lakitu7-g_deconDead to not give decon notice for buildings deconned from having it on
      Change Lakitu7-minLevelToSpecMM1 to prevent other types of /say from getting through
      Add Lakitu7-listadmins-minlevel-parameter
      Add Lakitu7-mystats
      Add Lakitu7-register
      Add !restart keepteamslock and switchteamslock
      Add Lakitu7-Say_Admins
      Add Lakitu7-sees-full-listplayers
      Update votes patch: 
	denybuild+mutevotes+allowspectatorvoting are in svn now
	Add !Designate, Designate votes
	Rename custom to poll for consistency with wolfET
        Poll votes no longer need quotes around them
	Handle spaces in target player names (instead of "reasons")
	Say vote caller's name in the vote description text
	"Fix" SD Mode 2 so that you can only rebuild one of each building (no more being lame with two armories)
	List votes' percentage to pass when that is something other than 50
      Change cvar defaults to lean less toward what I think should be done and more toward disabling things
	until you turn them on
      Change cvar properties to make a lot more of them SERVERINFO so that you can tell if a server has settings
	that are terrible and game-runining before you ever join
      Add R1CH-Team-imbalance-warnings
      Add R1CH-Unnamed-Player-Join-Warning
      Change a few admin flags due to conflicts. See admin flags appendix for full list
      Add Trem-Servers-prefixes-for-flag4 
      Remove Avenger-Enhanced-Give (it's in SVN)
      Remove black-names-fix, vote-message-in-console (covered by other patches)
      Remove my old player names patch because it's in SVN
      Remove lakitu7-name-duplication-color-fix.patch (covered by SVN)
      Remove lakitu7-PMDisabledFix.patch (covered by SVN)
4.18: Fixed SD Mode 1. It's been broken all this time and nobody noticed, I guess. Thanks Kevlarman.
4.17: Changed to Phil Bordelon's (better) flood protect system instead of mine. Covers PM spam.
4.16: Reverted g_admin_ban code to SVN909 version (see svn910revert+lakitu7-admin_true_name_in_banlog.patch notes)
      !L1 fix: Now doesn't give undesired behavior when people have numbers and spaces in their names
      Added g_deconDead
4.15: Simple chat flood protection
4.10: Removed tjw's antiwarp (too many problems, especially when unlagged is off)
      added /me
      added !info
      added g_maxGameClients
      added team customvotes
      modified the "__ called a vote" console message to say what they called a vote for
      modified vote messages to say what percent is needed to pass, if this percent != 50
      added g_antiSpawnBlock (but disabled it by default for now)
4.03: Fixed votes percentages; I'm an idiot
4.02: Fix mute-PM, add mute-tell
4.01: g_markDeconstruct default to 0
4.00: First public release


--------------------
Credits: 

The following people contributed code, created included code, or provided help to make this possible:
kevlarman, vcxzet, Napkin, Avenger, Risujin, r1ch, peoro, tjw, pierre, Khalsa, foobar, gate, Martin Doucha, Wolfwings, benmachine, Phil Bordelon, trem-servers.com, tuple, Das-Iro, Mexel, Techhead, Rocinante, Amanieu

Also all of the Beergarden and MG/Puretremulous servers and communities, who put up with my constant updating and testing and breaking of things :)
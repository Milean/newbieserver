/*
===========================================================================
Copyright (C) 2007 Amine Haddad

** This file is modified by Thomas Rinsma, I take no copyright at all

This file is part of Tremulous.

The original works of vcxzet (lamebot3) were used a guide to create TremBot.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

/* Current version: v0.02rc1 */

/*
[ChangeLog]
(v0.02)
(rc1)Rewrote botAimAtTarget, now aims at enemy head - amine
(rc1)Fixed bug where it crashes when botEnemy/botFriend are NULL and botThink executes - amine 
(rc1)Fixed bug where grenade wasn't disabled - amine

(v0.01)
Initial Release - amine

*/

#include "g_local.h"

// TODO: Create header file
qboolean botAimAtTarget( gentity_t *self, gentity_t *target );
int botFindDamagedFriendlyStructure( gentity_t *self );
int botFindClosestEnemy( gentity_t *self, qboolean includeTeam );
qboolean botTargetInRange( gentity_t *self, gentity_t *target );
int botGetDistanceBetweenPlayer( gentity_t *self, gentity_t *player );
qboolean botShootIfTargetInRange( gentity_t *self, gentity_t *target );

#define AS_OVER_RT3         ((ALIENSENSE_RANGE*0.5f)/M_ROOT3)

void G_BotAdd( char *name, int team, int skill ) {
	int i;
	int clientNum;
	char userinfo[MAX_INFO_STRING];
	int reservedSlots = 0;
	gentity_t *bot;

	reservedSlots = trap_Cvar_VariableIntegerValue( "sv_privateclients" );

	// find what clientNum to use for bot
	clientNum = -1;
	for( i = 0; i < reservedSlots; i++ ) {
		if( !g_entities[i].inuse ) {
			clientNum = i;
			break;
		}
	}

	if(clientNum < 0) {
		trap_Printf("no more slots for bot\n");
		return;
	}

	bot = &g_entities[ clientNum ];
	bot->r.svFlags |= SVF_BOT;
	bot->inuse = qtrue;

	//default bot data
	bot->botCommand = BOT_REGULAR;
	bot->botFriend = NULL;
	bot->botEnemy = NULL;
	bot->botFriendLastSeen = 0;
	bot->botEnemyLastSeen = 0;
	bot->botSkillLevel = skill;
	bot->botTeam = team;


	// register user information
	userinfo[0] = '\0';
	Info_SetValueForKey( userinfo, "name", name );
	Info_SetValueForKey( userinfo, "rate", "25000" );
	Info_SetValueForKey( userinfo, "snaps", "20" );

	trap_SetUserinfo( clientNum, userinfo );

	// have it connect to the game as a normal client
	if(ClientConnect(clientNum, qtrue) != NULL ) {
		// won't let us join
		return;
	}

	ClientBegin( clientNum );
	G_ChangeTeam( bot, team );
}

void G_BotDel( int clientNum ) {
	gentity_t *bot;

	bot = &g_entities[clientNum];
	if( !( bot->r.svFlags & SVF_BOT ) ) {
		trap_Printf( va("'^7%s^7' is not a bot\n", bot->client->pers.netname) );
		return;
	}

	ClientDisconnect(clientNum);
}

void G_BotCmd( gentity_t *master, int clientNum, char *command ) {
  gentity_t *bot;

  bot = &g_entities[clientNum];
  if( !( bot->r.svFlags & SVF_BOT ) ) {
    return;
  }

  bot->botFriend = NULL;
  bot->botEnemy = NULL;
  bot->botFriendLastSeen = 0;
  bot->botEnemyLastSeen = 0;

  if( !Q_stricmp( command, "regular" ) ) {
    bot->botCommand = BOT_REGULAR;
    //trap_SendServerCommand(-1, "print \"regular mode\n\"");
  } else if( !Q_stricmp( command, "idle" ) ) {
    bot->botCommand = BOT_IDLE;
    //trap_SendServerCommand(-1, "print \"idle mode\n\"");
  } else if( !Q_stricmp( command, "attack" ) ) {
    bot->botCommand = BOT_ATTACK;
    //trap_SendServerCommand(-1, "print \"attack mode\n\"");
  } else if( !Q_stricmp( command, "standground" ) ) {
    bot->botCommand = BOT_STAND_GROUND;
    //trap_SendServerCommand(-1, "print \"stand ground mode\n\"");
  } else if( !Q_stricmp( command, "defensive" ) ) {
    bot->botCommand = BOT_DEFENSIVE;
    //trap_SendServerCommand(-1, "print \"defensive mode\n\"");
  } else if( !Q_stricmp( command, "followprotect" ) ) {
    bot->botCommand = BOT_FOLLOW_FRIEND_PROTECT;
    bot->botFriend = master;
    //trap_SendServerCommand(-1, "print \"follow-protect mode\n\"");
  } else if( !Q_stricmp( command, "followattack" ) ) {
    bot->botCommand = BOT_FOLLOW_FRIEND_ATTACK;
    bot->botFriend = master;
    //trap_SendServerCommand(-1, "print \"follow-attack mode\n\"");
  } else if( !Q_stricmp( command, "followidle" ) ) {
    bot->botCommand = BOT_FOLLOW_FRIEND_IDLE;
    bot->botFriend = master;
    //trap_SendServerCommand(-1, "print \"follow-idle mode\n\"");
  } else if( !Q_stricmp( command, "teamkill" ) ) {
    bot->botCommand = BOT_TEAM_KILLER;
    //trap_SendServerCommand(-1, "print \"team kill mode\n\"");
  } else if( !Q_stricmp( command, "repair" ) ) {
    bot->botCommand = BOT_REPAIR;
    G_ForceWeaponChange( bot, WP_HBUILD );
    //trap_SendServerCommand(-1, "print \"repair mode\n\"");
  } else {
    bot->botCommand = BOT_REGULAR;
    //trap_SendServerCommand(-1, "print \"regular (unknown) mode\n\"");
  }
  return;
}

int G_BotBuyUpgrade ( gentity_t *ent, int upgrade )
{
    //already got this?
    if( BG_InventoryContainsUpgrade( upgrade, ent->client->ps.stats ) )
    {
      return 0;
    }

    //can afford this?
    if( BG_FindPriceForUpgrade( upgrade ) > (short)ent->client->ps.persistant[ PERS_CREDIT ] )
    {
      return 0;
    }

    //have space to carry this?
    if( BG_FindSlotsForUpgrade( upgrade ) & ent->client->ps.stats[ STAT_SLOTS ] )
    {
      return 0;
    }

    //are we /allowed/ to buy this?
    if( !BG_FindPurchasableForUpgrade( upgrade ) )
    {
      return 0;
    }

    //are we /allowed/ to buy this?
    if( !BG_FindStagesForUpgrade( upgrade, g_humanStage.integer ) || !BG_UpgradeIsAllowed( upgrade ) )
    {
      return 0;
    }

    if( upgrade == UP_AMMO )
      G_GiveClientMaxAmmo( ent, qfalse );
    else
    {
      //add to inventory
      BG_AddUpgradeToInventory( upgrade, ent->client->ps.stats );
    }

    if( upgrade == UP_BATTPACK )
      G_GiveClientMaxAmmo( ent, qtrue );

    //subtract from funds
    G_AddCreditToClient( ent->client, -(short)BG_FindPriceForUpgrade( upgrade ), qfalse );

	return 1;
}

int G_BotBuy ( gentity_t *ent, int weapon )
{
	int       maxAmmo, maxClips;

    //already got this?
    if( BG_InventoryContainsWeapon( weapon, ent->client->ps.stats ) )
    {
      return 0;
    }

    //can afford this?
    if( BG_FindPriceForWeapon( weapon ) > (short)ent->client->ps.persistant[ PERS_CREDIT ] )
    {
      return 0;
    }

    //have space to carry this?
    if( BG_FindSlotsForWeapon( weapon ) & ent->client->ps.stats[ STAT_SLOTS ] )
    {
      return 0;
    }

    //are we /allowed/ to buy this?
    if( !BG_FindPurchasableForWeapon( weapon ) )
    {
      return 0;
    }

    //are we /allowed/ to buy this?
    if( !BG_FindStagesForWeapon( weapon, g_humanStage.integer ) || !BG_WeaponIsAllowed( weapon ) )
    {
      return 0;
    }

    //add to inventory
    BG_AddWeaponToInventory( weapon, ent->client->ps.stats );
    BG_FindAmmoForWeapon( weapon, &maxAmmo, &maxClips );

    BG_PackAmmoArray( weapon, ent->client->ps.ammo, ent->client->ps.powerups,
                      maxAmmo, maxClips );

    G_ForceWeaponChange( ent, weapon );

    //set build delay/pounce etc to 0
    ent->client->ps.stats[ STAT_MISC ] = 0;

    //subtract from funds
    G_AddCreditToClient( ent->client, -(short)BG_FindPriceForWeapon( weapon ), qfalse );


	return 1;

}

int G_BotEvolve( gentity_t *ent, char *classname)
{
  char      s[ MAX_TOKEN_CHARS ];
  int       clientNum;
  int       i;
  vec3_t    infestOrigin;
  int       allowedClasses[ PCL_NUM_CLASSES ];
  int       numClasses = 0;
  pClass_t  currentClass = ent->client->ps.stats[ STAT_PCLASS ];
  pClass_t  newClass;
  int       numLevels;
  int       entityList[ MAX_GENTITIES ];
  vec3_t    range = { AS_OVER_RT3, AS_OVER_RT3, AS_OVER_RT3 };
  vec3_t    mins, maxs;
  int       num;
  gentity_t *other;

  if( ent->client->ps.stats[ STAT_HEALTH ] <= 0 )
    return 0;

	ent->client->pers.cmd.upmove = 0;

  clientNum = ent->client - level.clients;

    newClass = BG_FindClassNumForName( classname );

    //if we are not currently spectating, we are attempting evolution
    if( ent->client->pers.classSelection != PCL_NONE )
    {
      if( ( ent->client->ps.stats[ STAT_STATE ] & SS_WALLCLIMBING ) ||
          ( ent->client->ps.stats[ STAT_STATE ] & SS_WALLCLIMBINGCEILING ) )
      {
        return 0;
      }

      //check there are no humans nearby
      VectorAdd( ent->client->ps.origin, range, maxs );
      VectorSubtract( ent->client->ps.origin, range, mins );

      num = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
      for( i = 0; i < num; i++ )
      {
        other = &g_entities[ entityList[ i ] ];

        if( ( other->client && other->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS ) ||
            ( other->s.eType == ET_BUILDABLE && other->biteam == BIT_HUMANS ) )
        {
          return 0;
        }
      }

      if( !level.overmindPresent )
      {
        return 0;
      }

      numLevels = BG_ClassCanEvolveFromTo( currentClass,
                                           newClass,
                                           (short)ent->client->ps.persistant[ PERS_CREDIT ], 0 );

      if( G_RoomForClassChange( ent, newClass, infestOrigin ) )
      {
        //...check we can evolve to that class
        if( numLevels >= 0 &&
            BG_FindStagesForClass( newClass, g_alienStage.integer ) &&
            BG_ClassIsAllowed( newClass ) )
        {

          ent->client->pers.evolveHealthFraction = (float)ent->client->ps.stats[ STAT_HEALTH ] /
            (float)BG_FindHealthForClass( currentClass );

          if( ent->client->pers.evolveHealthFraction < 0.0f )
            ent->client->pers.evolveHealthFraction = 0.0f;
          else if( ent->client->pers.evolveHealthFraction > 1.0f )
            ent->client->pers.evolveHealthFraction = 1.0f;

          //remove credit
          G_AddCreditToClient( ent->client, -(short)numLevels, qtrue );
          ent->client->pers.classSelection = newClass;
          ClientUserinfoChanged( clientNum );
          VectorCopy( infestOrigin, ent->s.pos.trBase );
          ClientSpawn( ent, ent, ent->s.pos.trBase, ent->s.apos.trBase );

	  //trap_SendServerCommand( -1, va( "print \"evolved to %s\n\"", classname) );

          return 1;
        }
        else
        {
          return 0;
        }
      }
      else
      {
        return 0;
      }
    }
	return 0;
}


void G_BotThink( gentity_t *self )
{
	int distance = 0;
	int i;
	int clicksToStopChase = 30; //5 seconds
	int tooCloseDistance = 100; // about 1/3 of turret range
	int forwardMove = 127; // max speed
	int tempEntityIndex = -1;
	qboolean followFriend = qfalse;

	self->client->pers.cmd.buttons = 0;
	self->client->pers.cmd.forwardmove = 0;
	self->client->pers.cmd.upmove = 0;
	self->client->pers.cmd.rightmove = 0;

	// reset botEnemy if enemy is dead
	if(self->botEnemy && self->botEnemy->health <= 0) {
		self->botEnemy = NULL;
	}

	// if friend dies, reset status to regular
	if(self->botFriend && self->botFriend->health <= 0) {
		self->botCommand = BOT_REGULAR;
		self->botFriend = NULL;
	}

	// if health < 30, use medkit
	if(self->health <= 30) {
		BG_ActivateUpgrade( UP_MEDKIT, self->client->ps.stats );
	}

	if(self->client->pers.teamSelection == PTE_HUMANS) {
	  if(self->client->ps.weapon != WP_BLASTER &&
         self->botCommand != BOT_REPAIR ) {
	    G_ForceWeaponChange( self, WP_BLASTER );
	  }

	// armoury in range
	/*
        if(G_BuildableRange( self->client->ps.origin, 100, BA_H_ARMOURY)) {

		if((self->client->time10000 % 2000) == 0 && (short)self->client->ps.persistant[ PERS_CREDIT ] > 0) {

                            // sell current weapon
                            for( i = WP_NONE + 1; i < WP_NUM_WEAPONS; i++ )
                            {
                              if( BG_InventoryContainsWeapon( i, self->client->ps.stats ) &&
                                  BG_FindPurchasableForWeapon( i ) )
                              {
                                BG_RemoveWeaponFromInventory( i, self->client->ps.stats );

                                //add to funds
                                G_AddCreditToClient( self->client, (short)BG_FindPriceForWeapon( i ), qfalse );
                              }

                              //if we have this weapon selected, force a new selection
                              if( i == self->client->ps.weapon )
                                G_ForceWeaponChange( self, WP_NONE );
                            }


			// buy the stuff the friend has
			if(self->botFriend) {
				if( BG_InventoryContainsUpgrade( UP_JETPACK, self->botFriend->client->ps.stats ))
					G_BotBuyUpgrade( self, UP_JETPACK );

				else if( BG_InventoryContainsUpgrade( UP_BATTLESUIT, self->botFriend->client->ps.stats ))
                                        G_BotBuyUpgrade( self, UP_BATTLESUIT );

				else if( BG_InventoryContainsUpgrade( UP_LIGHTARMOUR, self->botFriend->client->ps.stats ) && random() <= 0.2)
                                        G_BotBuyUpgrade( self, UP_LIGHTARMOUR);

				else if( BG_InventoryContainsUpgrade( UP_HELMET, self->botFriend->client->ps.stats ))
                                        G_BotBuyUpgrade( self, UP_HELMET);

			}


	                // buy most expensive first, then one cheaper, etc, dirty but working way
			if( !G_BotBuy( self, WP_LUCIFER_CANNON ) ) {
				if( !G_BotBuy( self, WP_FLAMER ) ) {
					if( !G_BotBuy( self, WP_PULSE_RIFLE ) ) {
						if( !G_BotBuy( self, WP_CHAINGUN ) ) {
							if( !G_BotBuy( self, WP_MASS_DRIVER ) ) {
								if( !G_BotBuy( self, WP_LAS_GUN ) ) {
									if( !G_BotBuy( self, WP_SHOTGUN ) ) {
										G_BotBuy( self, WP_MACHINEGUN );
									}
								}
							}
						}
					}
				}
			}
		}

		G_BotBuyUpgrade( self, UP_AMMO );
        }
		*/
	}	// end humans

	/*
	if(self->client->pers.teamSelection == PTE_ALIENS) {

		if((self->client->time10000 % 2000) == 0) {
			// check if evo's > 0
			if( (short)self->client->ps.persistant[ PERS_CREDIT ] > 0) {
				//trap_SendServerCommand( -1, va( "print \"trying to evolve\n\"") );

				// very not-clean code, but hea it works
                                int res;
                                if(!G_BotEvolve(self, "level4")) {
                                        if(!G_BotEvolve(self, "level3upg")) {
						res = (random()>0.5)?G_BotEvolve(self, "level3"):G_BotEvolve(self, "level2upg");
						if(!res) {
							res = (random()>0.5)?G_BotEvolve(self, "level2"):G_BotEvolve(self, "level1upg");
							if(!res) {
                                                                if(!G_BotEvolve(self, "level1")) {
                                                                        G_BotEvolve(self, "level0");
                                                                }
                                                        }
                                                }
                                        }
                                }

			}
		}

	}	// aliens
	*/

	// what mode are we in?
	switch(self->botCommand) {
		case BOT_REGULAR:
			// if there is enemy around, rush them and attack.
			if(self->botEnemy) {
				// we already have an enemy. See if still in LOS.
				if(!botTargetInRange(self,self->botEnemy)) {
					// if it's been over clicksToStopChase clicks since we last seen him in LOS then do nothing, else follow him!
					if(self->botEnemyLastSeen > clicksToStopChase) {
						// forget him!
						self->botEnemy = NULL;
						self->botEnemyLastSeen = 0;
					} else {
						//chase him
						self->botEnemyLastSeen++;
					}
				} else {
					// we see him!
					self->botEnemyLastSeen = 0;
				}
			}

			if(!self->botEnemy) {
				// try to find closest enemy
				tempEntityIndex = botFindClosestEnemy(self, qfalse);
				if(tempEntityIndex >= 0)
					self->botEnemy = &g_entities[tempEntityIndex];
			}

			if(!self->botEnemy) {
				// no enemy
                                      if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS )
                                        self->client->pers.cmd.upmove = -1;
			} else {
				// enemy!
				self->client->ps.pm_flags &= ~PMF_CROUCH_HELD;
				// gesture
				if(random() <= 0.1) self->client->pers.cmd.buttons |= BUTTON_GESTURE;

				distance = botGetDistanceBetweenPlayer(self, self->botEnemy);
				botAimAtTarget(self, self->botEnemy);

				// enable wallwalk
				if( BG_ClassHasAbility( self->client->ps.stats[ STAT_PCLASS ], SCA_WALLCLIMBER ) ) {
					self->client->pers.cmd.upmove = -1;
				}
				// jetpack
				if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS && BG_UpgradeIsActive(BG_FindUpgradeNumForName((char *)"jetpack"), self->botFriend->client->ps.stats)) {
					BG_ActivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}
				else {
				        BG_DeactivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}

				botShootIfTargetInRange(self,self->botEnemy);
				self->client->pers.cmd.forwardmove = forwardMove;
				self->client->pers.cmd.rightmove = -100;
				if(self->client->time1000 >= 500)
					self->client->pers.cmd.rightmove = 100;
			}

			break;

		case BOT_IDLE:
			// just stand there and look ugly.
			if( random() <= 0.4 )
                             self->client->ps.pm_flags |= PMF_CROUCH_HELD;
			break;

		case BOT_ATTACK:
			// .. not sure ..
			break;

		case BOT_STAND_GROUND:
			// stand ground but attack enemies if you can reach.
			if(self->botEnemy) {
				// we already have an enemy. See if still in LOS.
				if(!botTargetInRange(self,self->botEnemy)) {
					//we are not in LOS
					self->botEnemy = NULL;
				}
			}

			if(!self->botEnemy) {
				// try to find closest enemy
				tempEntityIndex = botFindClosestEnemy(self, qfalse);
				if(tempEntityIndex >= 0)
					self->botEnemy = &g_entities[tempEntityIndex];
			}

			if(!self->botEnemy) {
				// no enemy
                                      if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS )
                                        self->client->pers.cmd.upmove = -1;
			} else {
				// enemy!
				self->client->ps.pm_flags &= ~PMF_CROUCH_HELD;
				if( random() < 0.1 ) self->client->pers.cmd.buttons |= BUTTON_GESTURE;
				distance = botGetDistanceBetweenPlayer(self, self->botEnemy);
				botAimAtTarget(self, self->botEnemy);

				// enable wallwalk
				if( BG_ClassHasAbility( self->client->ps.stats[ STAT_PCLASS ], SCA_WALLCLIMBER ) ) {
					self->client->pers.cmd.upmove = -1;
				}
                                // jetpack
				if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS && BG_UpgradeIsActive(BG_FindUpgradeNumForName((char *)"jetpack"), self->botFriend->client->ps.stats)) {
                                        BG_ActivateUpgrade( BG_FindUpgradeNumForName((char*)"jetpack"), self->client->ps.stats );
                                }
				else {
				        BG_DeactivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}

				botShootIfTargetInRange(self,self->botEnemy);
			}

			break;

		case BOT_DEFENSIVE:
			// if there is an enemy around, rush them but not too far from where you are standing when given this command
			break;

		case BOT_FOLLOW_FRIEND_PROTECT:
			// run towards friend, attack enemy
			break;

		case BOT_FOLLOW_FRIEND_ATTACK:
			// run with friend until enemy spotted, then rush enemy
			if(self->botEnemy) {
				// we already have an enemy. See if still in LOS.
				if(!botTargetInRange(self,self->botEnemy)) {
					// if it's been over clicksToStopChase clicks since we last seen him in LOS then do nothing, else follow him!
					if(self->botEnemyLastSeen > clicksToStopChase) {
						// forget him!
						self->botEnemy = NULL;
						self->botEnemyLastSeen = 0;
					} else {
						//chase him
						self->botEnemyLastSeen++;
					}
				} else {
					// we see him!
					self->botEnemyLastSeen = 0;
				}

				//if we are chasing enemy, reset counter for friend LOS .. if its true
				if(self->botEnemy) {
					if(botTargetInRange(self,self->botFriend)) {
						self->botFriendLastSeen = 0;
					} else {
						self->botFriendLastSeen++;
					}
				}
			}

			if(!self->botEnemy) {
				// try to find closest enemy
				tempEntityIndex = botFindClosestEnemy(self, qfalse);
				if(tempEntityIndex >= 0)
					self->botEnemy = &g_entities[tempEntityIndex];
			}

			if(!self->botEnemy) {
				// no enemy
				if(self->botFriend) {
					// see if our friend is in LOS
					if(botTargetInRange(self,self->botFriend)) {
						// go to him!
						followFriend = qtrue;
						self->botFriendLastSeen = 0;
					} else {
						// if it's been over clicksToStopChase clicks since we last seen him in LOS then do nothing, else follow him!
						if(self->botFriendLastSeen > clicksToStopChase) {
							// forget him!
							followFriend = qfalse;
						} else {
							self->botFriendLastSeen++;
							followFriend = qtrue;
						}
					}

					if(followFriend) {
						distance = botGetDistanceBetweenPlayer(self, self->botFriend);
						botAimAtTarget(self, self->botFriend);

						// enable wallwalk
						if( BG_ClassHasAbility( self->client->ps.stats[ STAT_PCLASS ], SCA_WALLCLIMBER ) ) {
							self->client->pers.cmd.upmove = -1;
						}
                                // jetpack
				if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS && BG_UpgradeIsActive(BG_FindUpgradeNumForName((char *)"jetpack"), self->botFriend->client->ps.stats)) {
                                        BG_ActivateUpgrade( BG_FindUpgradeNumForName((char*)"jetpack"), self->client->ps.stats );
                                }
				else {
				        BG_DeactivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}

						//botShootIfTargetInRange(self,self->botEnemy);
						if(distance>tooCloseDistance) {
							self->client->pers.cmd.forwardmove = forwardMove;
							self->client->pers.cmd.rightmove = -100;
							if(self->client->time1000 >= 500)
								self->client->pers.cmd.rightmove = 100;
						}
					}
				}
			} else {
				// enemy!
				self->client->ps.pm_flags &= ~PMF_CROUCH_HELD;
				if( random() < 0.1 ) self->client->pers.cmd.buttons |= BUTTON_GESTURE;
				distance = botGetDistanceBetweenPlayer(self, self->botEnemy);
				botAimAtTarget(self, self->botEnemy);

				// enable wallwalk
				if( BG_ClassHasAbility( self->client->ps.stats[ STAT_PCLASS ], SCA_WALLCLIMBER ) ) {
					self->client->pers.cmd.upmove = -1;
				}
				// jetpack
				if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS && BG_UpgradeIsActive(BG_FindUpgradeNumForName((char *)"jetpack"), self->botFriend->client->ps.stats)) {
                                        BG_ActivateUpgrade( BG_FindUpgradeNumForName((char*)"jetpack"), self->client->ps.stats );
                                }
				else {
				        BG_DeactivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}

				botShootIfTargetInRange(self,self->botEnemy);
				self->client->pers.cmd.forwardmove = forwardMove;
				self->client->pers.cmd.rightmove = -100;
				if(self->client->time1000 >= 500)
					self->client->pers.cmd.rightmove = 100;
			}

			break;

		case BOT_FOLLOW_FRIEND_IDLE:
			// run with friend and stick with him no matter what. no attack mode.
			if(self->botFriend) {
				// see if our friend is in LOS
				if(botTargetInRange(self,self->botFriend)) {
					// go to him!
					followFriend = qtrue;
					self->botFriendLastSeen = 0;
				} else {
					// if it's been over clicksToStopChase clicks since we last seen him in LOS then do nothing, else follow him!
					if(self->botFriendLastSeen > clicksToStopChase) {
						// forget him!
						followFriend = qfalse;
					} else {
						//chase him
						self->botFriendLastSeen++;
						followFriend = qtrue;
					}
				}

				if(followFriend) {
					distance = botGetDistanceBetweenPlayer(self, self->botFriend);
					botAimAtTarget(self, self->botFriend);

					// enable wallwalk
					if( BG_ClassHasAbility( self->client->ps.stats[ STAT_PCLASS ], SCA_WALLCLIMBER ) ) {
						self->client->pers.cmd.upmove = -1;
					}
                                // jetpack
				if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS && BG_UpgradeIsActive(BG_FindUpgradeNumForName((char *)"jetpack"), self->botFriend->client->ps.stats)) {
                                        BG_ActivateUpgrade( BG_FindUpgradeNumForName((char*)"jetpack"), self->client->ps.stats );
                                }
				else {
				        BG_DeactivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}

					//botShootIfTargetInRange(self,self->botFriend);
					if(distance>tooCloseDistance) {
						self->client->pers.cmd.forwardmove = forwardMove;
						self->client->pers.cmd.rightmove = -100;
						if(self->client->time1000 >= 500)
							self->client->pers.cmd.rightmove = 100;
					}
				}
			}

			break;

		case BOT_TEAM_KILLER:
			// attack enemies, then teammates!
			if(self->botEnemy) {
				// we already have an enemy. See if still in LOS.
				if(!botTargetInRange(self,self->botEnemy)) {
					// if it's been over clicksToStopChase clicks since we last seen him in LOS then do nothing, else follow him!
					if(self->botEnemyLastSeen > clicksToStopChase) {
						// forget him!
						self->botEnemy = NULL;
						self->botEnemyLastSeen = 0;
					} else {
						//chase him
						self->botEnemyLastSeen++;
					}
				} else {
					// we see him!
					self->botEnemyLastSeen = 0;
				}
			}

			if(!self->botEnemy) {
				// try to find closest enemy
				tempEntityIndex = botFindClosestEnemy(self, qtrue);
				if(tempEntityIndex >= 0)
					self->botEnemy = &g_entities[tempEntityIndex];
			}

			if(!self->botEnemy) {
				// no enemy, we're all alone :(
			} else {
				// enemy!
				self->client->ps.pm_flags &= ~PMF_CROUCH_HELD;
				if(random() < 0.1) self->client->pers.cmd.buttons |= BUTTON_GESTURE;
				distance = botGetDistanceBetweenPlayer(self, self->botEnemy);
				botAimAtTarget(self, self->botEnemy);

				// enable wallwalk
				if( BG_ClassHasAbility( self->client->ps.stats[ STAT_PCLASS ], SCA_WALLCLIMBER ) ) {
					self->client->pers.cmd.upmove = -1;
				}
                                // jetpack
				if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS && BG_UpgradeIsActive(BG_FindUpgradeNumForName((char *)"jetpack"), self->botFriend->client->ps.stats)) {
                                        BG_ActivateUpgrade( BG_FindUpgradeNumForName((char*)"jetpack"), self->client->ps.stats );
                                }
				else {
				        BG_DeactivateUpgrade( BG_FindUpgradeNumForName((char *)"jetpack"), self->client->ps.stats );
				}

				botShootIfTargetInRange(self,self->botEnemy);
				self->client->pers.cmd.forwardmove = forwardMove;
				self->client->pers.cmd.rightmove = -100;
				if(self->client->time1000 >= 500)
					self->client->pers.cmd.rightmove = 100;
			}

			break;

	    case BOT_REPAIR:
             // Find the entity to repair and turn in that direction
             tempEntityIndex = botFindDamagedFriendlyStructure(self);
             botAimAtTarget(self, &g_entities[tempEntityIndex]);

             if( botGetDistanceBetweenPlayer( self, &g_entities[tempEntityIndex] ) < 
                 100 )
             {
                // If we are within the distance of the structure, than we
                // start directly with repairing
                self->client->pers.cmd.buttons |= BUTTON_ATTACK2;
             }
             else
             {
                // Otherwise, we move in that direction, generally
                self->client->pers.cmd.forwardmove = forwardMove;
                self->client->pers.cmd.rightmove = -100;
                if(self->client->time1000 >= 500)
                    self->client->pers.cmd.rightmove = 100;
             }
			 break;

		default:
			// dunno.
			break;
	}
}

void G_BotSpectatorThink( gentity_t *self ) {
  if( self->client->ps.pm_flags & PMF_QUEUED) {
	  //we're queued to spawn, all good
	  return;
  }

  if( self->client->sess.sessionTeam == TEAM_SPECTATOR ) {
	int teamnum = self->client->pers.teamSelection;
	int clientNum = self->client->ps.clientNum;

    if( teamnum == PTE_HUMANS ) {
      self->client->pers.classSelection = PCL_HUMAN;
      self->client->ps.stats[ STAT_PCLASS ] = PCL_HUMAN;
      self->client->pers.humanItemSelection = WP_HBUILD;
      G_PushSpawnQueue( &level.humanSpawnQueue, clientNum );
	} else if( teamnum == PTE_ALIENS) {
		self->client->pers.classSelection = PCL_ALIEN_LEVEL0;
		self->client->ps.stats[ STAT_PCLASS ] = PCL_ALIEN_LEVEL0;
		G_PushSpawnQueue( &level.alienSpawnQueue, clientNum );
	}
  }
}

/*
 * Called when we are in intermission.
 * Just flag that we are ready to proceed.
 */
void G_BotIntermissionThink( gclient_t *client )
{
	client->readyToExit = qtrue;
}

qboolean botAimAtTarget( gentity_t *self, gentity_t *target ) {
	vec3_t dirToTarget, angleToTarget, highPoint, targetUp, targetStraight, realBase;

	// Calculate the point on top of model (head) (well, 15% lower).
	VectorCopy( target->s.pos.trBase, highPoint );
	highPoint[2] += target->r.maxs[2] * 0.85;

	// Make a vector from trBase -> highPoint
	VectorSubtract( highPoint, target->s.pos.trBase, targetUp );

	// Find the point where we actually shoot from
	VectorCopy( self->s.pos.trBase, realBase );
	realBase[2] += self->client->ps.viewheight;

	// Find the vector that points from our gun to his base
	VectorSubtract( target->s.pos.trBase, realBase, targetStraight );

	// Create our vector that points our gun to his face
	VectorAdd( targetStraight, targetUp, dirToTarget );

	// Grab the angles to use with delta_angles
	vectoangles( dirToTarget, angleToTarget );
	self->client->ps.delta_angles[ 0 ] = ANGLE2SHORT( angleToTarget[ 0 ] );
	self->client->ps.delta_angles[ 1 ] = ANGLE2SHORT( angleToTarget[ 1 ] );
	self->client->ps.delta_angles[ 2 ] = ANGLE2SHORT( angleToTarget[ 2 ] );



	        if(angleToTarget[0] > -350 && angleToTarget[0] < -180) {
			if( self->client->ps.stats[ STAT_PTEAM ] == PTE_HUMANS ) {
				// down
				self->client->pers.cmd.upmove = -1;
			}
	        }
	        else if(angleToTarget[0] < -6.0 && angleToTarget[0] > -180) {
	                // up
	                self->client->pers.cmd.upmove = 127;
	        }
	        else {
	                // don't do anything
	                self->client->pers.cmd.upmove = 0;
	        }

//	trap_SendServerCommand( -1, va( "print \"%f\n\"", angleToTarget[0]) );

	return qtrue;
}

qboolean botTargetInRange( gentity_t *self, gentity_t *target ) {
	trace_t   trace;
	gentity_t *traceEnt;
	//int myGunRange;
	//myGunRange = MGTURRET_RANGE * 3;

	if( !self || !target )
		return qfalse;

	if( !self->client || !target->client )
		if( target->s.eType != ET_BUILDABLE )
			return qfalse;

	if( target->client->ps.stats[ STAT_STATE ] & SS_HOVELING )
		return qfalse;

	if( target->health <= 0 )
		return qfalse;

	//if( Distance( self->s.pos.trBase, target->s.pos.trBase ) > myGunRange )
	//	return qfalse;

	//draw line between us and the target and see what we hit
	trap_Trace( &trace, self->s.pos.trBase, NULL, NULL, target->s.pos.trBase, self->s.number, MASK_SHOT );
	traceEnt = &g_entities[ trace.entityNum ];

	// check that we hit a human and not an object
	//if( !traceEnt->client )
	//	return qfalse;

	//check our target is in LOS
	if(!(traceEnt == target))
		return qfalse;

	return qtrue;
}

/*
 * Find the entity index of a friendly building which is in range of the bot.
 * Returns -1 if no entity is in range.
 * @TODO: refactor generic code out from botFindClosestEnemy
 */
int botFindDamagedFriendlyStructure( gentity_t *self )
{
	// The range of our scanning field.
	int vectorRange = MGTURRET_RANGE * 3;
	// vectorRange converted to a vector
	vec3_t range;
	// Lower bound vector
	vec3_t mins;
	// Upper bound vector
	vec3_t maxs;
	// Indexing field
	int i;
	// Entities located in scanning field
	int total_entities;
	// Array which contains the located entities
	int entityList[ MAX_GENTITIES ];
	// Temporary entitiy
	gentity_t *target;
	// Temporary buildable
	buildable_t inspectedBuilding;

	VectorSet( range, vectorRange, vectorRange, vectorRange );
	VectorAdd( self->client->ps.origin, range, maxs );
	VectorSubtract( self->client->ps.origin, range, mins );

	// Fetch all entities in the bounding box and iterate over them
	// to locate the structures that belong to the team of the bot and that
	// are not at full health.
	total_entities = trap_EntitiesInBox(mins, maxs, entityList, MAX_GENTITIES);
	for( i = 0; i < total_entities; ++i )
	{
		target = &g_entities[ entityList[ i ] ];
		inspectedBuilding = BG_FindBuildNumForEntityName( target->classname );
		if(target->s.eType == ET_BUILDABLE &&
		   target->biteam == self->client->ps.stats[ STAT_PTEAM ] &&
		   target->health !=  BG_FindHealthForBuildable( inspectedBuilding ) )
		{
			// Building found, return entity index
			return entityList[ i ];
		}
	}
	return -1;
}

int botFindClosestEnemy( gentity_t *self, qboolean includeTeam ) {
	// return enemy entity index, or -1
	int vectorRange = MGTURRET_RANGE * 3;
	int i;
	int total_entities;
	int entityList[ MAX_GENTITIES ];
	vec3_t    range;
	vec3_t    mins, maxs;
	gentity_t *target;

	VectorSet( range, vectorRange, vectorRange, vectorRange );
	VectorAdd( self->client->ps.origin, range, maxs );
	VectorSubtract( self->client->ps.origin, range, mins );

	total_entities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	// check list for enemies
	for( i = 0; i < total_entities; i++ ) {
		target = &g_entities[ entityList[ i ] ];

		if( (target->s.eType == ET_BUILDABLE || target->client) && self != target ) {
			if( target->s.eType == ET_BUILDABLE ) {
				if( target->biteam != self->client->ps.stats[ STAT_PTEAM ] ) {
                                        // aliens ignore if it's in LOS because they have radar
                                        if(self->client->ps.stats[ STAT_PTEAM ] == PTE_ALIENS) {
                                                return entityList[ i ];
                                        } else {
                                                if( botTargetInRange( self, target ) ) {
                                                        return entityList[ i ];
						}
                                        }
				}
			}
			else {
				if( target->client->ps.stats[ STAT_PTEAM ] != self->client->ps.stats[ STAT_PTEAM ] ) {
					// aliens ignore if it's in LOS because they have radar
					if(self->client->ps.stats[ STAT_PTEAM ] == PTE_ALIENS) {
						return entityList[ i ];
					} else {
						if( botTargetInRange( self, target ) ) {
							return entityList[ i ];
						}
					}
				}
			}
		}
	}

	if(includeTeam) {
		// check list for enemies in team
		for( i = 0; i < total_entities; i++ ) {
			target = &g_entities[ entityList[ i ] ];

			if( target->client && self !=target && target->client->ps.stats[ STAT_PTEAM ] == self->client->ps.stats[ STAT_PTEAM ] ) {
				// aliens ignore if it's in LOS because they have radar
				if(self->client->ps.stats[ STAT_PTEAM ] == PTE_ALIENS) {
					return entityList[ i ];
				} else {
					if( botTargetInRange( self, target ) ) {
						return entityList[ i ];
					}
				}
			}
		}
	}

	return -1;
}

// really an int? what if it's too long?
int botGetDistanceBetweenPlayer( gentity_t *self, gentity_t *player ) {
	return Distance( self->s.pos.trBase, player->s.pos.trBase );
}

qboolean botShootIfTargetInRange( gentity_t *self, gentity_t *target ) {
	if(botTargetInRange(self,target)) {
		if( self->client->time1000  <= 200 &&  self->client->ps.stats[ STAT_PTEAM ] == PTE_ALIENS ){
			self->client->pers.cmd.buttons |= BUTTON_ATTACK2;
		}
		else {
			if( self->client->ps.weapon == WP_LUCIFER_CANNON ) {
				self->client->pers.cmd.buttons |= BUTTON_ATTACK2;
			}
			else {
				self->client->pers.cmd.buttons |= BUTTON_ATTACK;
			}
		}

		return qtrue;
	}
	return qfalse;
}

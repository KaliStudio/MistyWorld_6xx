/*

 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2006-2014 ScriptDev2 <https://github.com/scriptdev2/scriptdev2/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "instance_zulgurub.h"

enum Yells
{
    // Jin'do the Godbreaker
    SAY_INTRO                   = 0,
    SAY_AGGRO                   = 1,
    EMOTE_SHADOWS_OF_HAKKAR     = 2, // ID - 97172 Shadows of Hakkar
    SAY_JINDO_SPIRIT_PHASE      = 3,
    //SAY_PLAYER_KILL             = 4, // missing data

    // Spirit of Hakkar
    SAY_SPIRIT_SPIRIT_PHASE     = 0,
    SAY_SPIRIT_DEFEATED         = 1,

    // Jin'do the Godbreaker - Trigger
    SAY_JINDO_DEFEATED          = 0,

    // Shadow of Hakkar
    SAY_SHADOW_DEFEATED         = 0,
};

enum Spells
{
};

enum Events
{
};

class boss_jindo_the_godbreaker : public CreatureScript
{
    public:
        boss_jindo_the_godbreaker() : CreatureScript("boss_jindo_the_godbreaker") { }

        struct boss_jindo_the_godbreakerAI : public BossAI
        {
            boss_jindo_the_godbreakerAI(Creature* creature) : BossAI(creature, DATA_JINDO) { }

            void Reset() OVERRIDE
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                _EnterCombat();
                Talk(SAY_AGGRO);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
                /*
                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                }
                */

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetZulGurubAI<boss_jindo_the_godbreakerAI>(creature);
        }
};

void AddSC_boss_jindo_the_godbreaker()
{
    new boss_jindo_the_godbreaker();
}

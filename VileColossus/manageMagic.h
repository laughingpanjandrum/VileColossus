#pragma once

#include "gamedata.h"
#include "animation.h"
#include "effects.h"
#include "messages.h"
#include "lootgen.h"
#include "manageItems.h"


//	Spell effects


void blinkInDirection(gamedataPtr gdata, creaturePtr caster, const intpair vec, const int dist);
void chainDamage(gamedataPtr gdata, creaturePtr t, const DamageType dtype, const intpair drange, int chains_left);
void radiateSpellDamage(gamedataPtr gdata, creaturePtr caster, const intpair ctr, const int r, const Spell sp, const int lvl);

creaturePtr findRandomSpellTarget(gamedataPtr gdata, creaturePtr caster);

void hitTargetWithSpell(gamedataPtr gdata, creaturePtr caster, creaturePtr target, const Spell sp, const int lvl);
void triggerSpellEffect(gamedataPtr gdata, creaturePtr caster, const Spell sp, const int lvl);


//	Player casting

void playerCastAimedSpell(gamedataPtr gdata, const intpair vec);
void playerCastSpell(gamedataPtr gdata, const int sp_idx);
void castSpellFromHotkey(gamedataPtr gdata, const int hotkey);



//	Enhance spellrunes

void openRuneEnhancer(gamedataPtr gdata);
void tryEnhanceRune(gamedataPtr gdata);


//	Learning new spells

void openSpellList(gamedataPtr gdata);

void populateCurrentItemListWithRunes(gamedataPtr gdata);
void openRuneImprinter(gamedataPtr gdata);

void removeAllSpellRunes(gamedataPtr gdata);
bool playerImprintSpellRune(gamedataPtr gdata, itemPtr it);
void imprintSelectedSpellRune(gamedataPtr gdata);
void removeSelectedRune(gamedataPtr gdata);
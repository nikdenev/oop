#pragma once

#include "Commander.h"

////////////////////////////////////////////////////////////////////////

//Prisvoqva srednoto i obshtoto nivo na boinite umeniq i zlatoto nujno za poddrujka na voiskata
void getFightSkillsAndTotalCost(const Commander* commander, double& averageFightSkills,
								int& totalFightSkills, int& totalCost);

//Prisvoqva srednoto i obshtoto nivo na magicheskata mosht na voiskata
void getMana(const Commander* commander, double& averageMana, int& totalMana);

//vrushta maksimalniqt broi magii koito moje da izpolzva maga-a
int getSpellPower(const Mage& mage);

//vrushta maksimalniqt broi zaklinaniq koito mogat da izpolzvat magovete vuv voiskata
int getTotalSpellPower(const Commander& commander);

////////////////////////////////////////////////////////////////////////

void function(const Commander* commander);






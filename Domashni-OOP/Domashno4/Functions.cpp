#include "Functions.h"

void getFightSkillsAndTotalCost(const Commander* commander, double& averageFightSkills,
								int& totalFightSkills, int& totalCost)
{
	totalFightSkills = 0;

	totalCost = 0;

	int fightersCount = 1;

	totalFightSkills += commander->getFightSkills();

	totalCost += commander->getSalary();

	for (int i = 0; i < commander->getMagesCount(); i++)
	{
		totalFightSkills += (*commander)[i].getFightSkills();

		totalCost += (*commander)[i].getSalary();

		for (int j = 0; j < (*commander)[i].getSergeantsCount(); j++)
		{
			totalFightSkills += (*commander)[i][j].getFightSkills();

			totalCost += (*commander)[i][j].getSalary();

			for (int k = 0; k < (*commander)[i][j].getSoldiersCount(); k++)
			{
				totalFightSkills += (*commander)[i][j][k].getFightSkills();

				totalCost += (*commander)[i][j][k].getSalary();

				fightersCount++;
			}

			fightersCount++;
		}

		fightersCount++;
	}

	averageFightSkills = (double)totalFightSkills / fightersCount;
}

void getMana(const Commander* commander, double& averageMana, int& totalMana)
{
	totalMana = 0;

	for (int i = 0; i < commander->getMagesCount(); i++)
		totalMana += (*commander)[i].getMana();

	averageMana = (double)totalMana / commander->getMagesCount();
}

int getSpellPower(const Mage& mage)
{
	int count = 0;

	int mana = mage.getMana();

	int spellsCount = mage.getMagicBook().getSpellsCount();

	if (spellsCount == 0)
		return 0;

	for (int i = 0; i < spellsCount && (mana -= mage.getMagicBook()[i].getCastingCost()) >= 0; i++)
		++count;

	return count;
}

int getTotalSpellPower(const Commander* commander)
{
	int count = 0;

	int magesCount = commander->getMagesCount();

	for (int i = 0; i < magesCount; i++)
	{
		count += getSpellPower((*commander)[i]);
	}

	return count;
}


void function(const Commander* commander)
{
	double averageFightSkills;
	int totalFightSkills;
	int totalCost;

	double averageMana;
	int totalMana;

	getFightSkillsAndTotalCost(commander, averageFightSkills,
		totalFightSkills, totalCost);

	getMana(commander, averageMana, totalMana);

	std::cout << "Average fight skills of the army: " << averageFightSkills << '\n'
		<< "Total fight skills of the army: " << totalFightSkills << '\n'
		<< "Average mana of the army: " << averageMana << '\n'
		<< "Total mana of the army: " << totalMana << '\n'
		<< "Total gold for a month for the army: " << totalCost << '\n'
		<< "Max count of spells that the army can cast is: " 
		<< getTotalSpellPower(commander) << '\n';
	
}
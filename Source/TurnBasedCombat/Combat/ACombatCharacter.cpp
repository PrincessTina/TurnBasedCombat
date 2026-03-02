#include "ACombatCharacter.h"

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitHealth();
	InitSpeed();
	SetSpellcastingAbility();
}

int8 ACombatCharacter::GetModifier(const int8 Ability) const
{
	return static_cast<int8>(Ability * 0.5 - 5);
}

void ACombatCharacter::InitHealth()
{
	const int8 ConstitutionModifier = GetModifier(Constitution);
	int8 HealthBase = 0;
	
	switch (Class)
	{
	case ECharacterClass::Barbarian:
		HealthBase = 12;
		break;
	case ECharacterClass::Paladin:
	case ECharacterClass::Fighter:
	case ECharacterClass::Ranger:
		HealthBase = 10;
		break;
	case ECharacterClass::Bard:
	case ECharacterClass::Cleric:
	case ECharacterClass::Druid:
	case ECharacterClass::Monk:
	case ECharacterClass::Rogue:
	case ECharacterClass::Warlock:
		HealthBase = 8;
		break;
	case ECharacterClass::Wizard:
	case ECharacterClass::Sorcerer:
		HealthBase = 6;
		break;
	}
	
	MaxHealth = HealthBase + ConstitutionModifier + (Level - 1) * ((HealthBase + 2) * 0.5 + ConstitutionModifier);
	Health = MaxHealth;
}

void ACombatCharacter::InitSpeed()
{
	switch (Race)
	{
	case ECharacterRace::Human:
	case ECharacterRace::Elf:
	case ECharacterRace::Drow:
	case ECharacterRace::HalfElf:
	case ECharacterRace::HalfOrc:
	case ECharacterRace::Tiefling:
	case ECharacterRace::Dragonborn:
		Speed = 9;
		break;
	case ECharacterRace::Halfling:
	case ECharacterRace::Dwarf:
	case ECharacterRace::Gnome:
		Speed = 7.5;
		break;
	}
}

void ACombatCharacter::SetSpellcastingAbility()
{
	switch (Class)
	{
	case ECharacterClass::Barbarian:
	case ECharacterClass::Bard:
	case ECharacterClass::Paladin:
	case ECharacterClass::Sorcerer:
	case ECharacterClass::Warlock:
		SpellcastingAbility = &Charisma;
		break;
	case ECharacterClass::Cleric:
	case ECharacterClass::Druid:
	case ECharacterClass::Monk:
	case ECharacterClass::Ranger:
		SpellcastingAbility = &Wisdom;
		break;
	case ECharacterClass::Fighter:
	case ECharacterClass::Rogue:
	case ECharacterClass::Wizard:
		SpellcastingAbility = &Intelligence;
		break;
	}
}
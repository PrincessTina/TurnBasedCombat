#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACombatCharacter.generated.h"

UENUM()
enum class ECharacterRace
{
	Human,
	Elf,
	Drow,
	HalfElf,
	HalfOrc,
	Halfling,
	Dwarf,
	Gnome,
	Tiefling,
	Dragonborn
};

UENUM()
enum class ECharacterSubRace
{
	None
};

UENUM()
enum class ECharacterClass
{
	Barbarian,
	Bard,
	Cleric,
	Druid,
	Fighter,
	Monk,
	Paladin,
	Ranger,
	Rogue,
	Sorcerer,
	Warlock,
	Wizard
};

UENUM()
enum class ECharacterSubClass
{
	None
};

UCLASS(abstract)
class ACombatCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Character")
	int8 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	int MaxHealth = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	int Health = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	double Speed = 0;
	
	UPROPERTY(EditAnywhere, Category="Character")
	ECharacterRace Race = ECharacterRace::Elf;
	UPROPERTY(EditAnywhere, Category="Character")
	ECharacterSubRace SubRace = ECharacterSubRace::None;
	
	UPROPERTY(EditAnywhere, Category="Character")
	ECharacterClass Class = ECharacterClass::Ranger;
	UPROPERTY(EditAnywhere, Category="Character")
	ECharacterSubClass SubClass = ECharacterSubClass::None;
	
	UPROPERTY(EditAnywhere, Category="Ability scores")
	int8 Strength = 10;
	UPROPERTY(EditAnywhere, Category="Ability scores")
	int8 Dexterity = 10;
	UPROPERTY(EditAnywhere, Category="Ability scores")
	int8 Constitution = 10;
	UPROPERTY(EditAnywhere, Category="Ability scores")
	int8 Intelligence = 10;
	UPROPERTY(EditAnywhere, Category="Ability scores")
	int8 Wisdom = 10;
	UPROPERTY(EditAnywhere, Category="Ability scores")
	int8 Charisma = 10;
	
	int8* SpellcastingAbility = nullptr;
	
public:
	virtual void BeginPlay() override;
	
private:
	int8 GetModifier(const int8 Ability) const;
	void InitHealth();
	void InitSpeed();
	void SetSpellcastingAbility();
};
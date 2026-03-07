// Copyright by Neon Fiction Studios

#pragma once

#include "CoreMinimal.h"
#include "HeroEnums.generated.h"

/**
 * Core Attributes that determine a Hero's overall power
 */
UENUM(BlueprintType)
enum class EAttributes : uint8
{
	STR		UMETA(DisplayName = "Strength"),
	DEX		UMETA(DisplayName = "Dexterity"),
	CON		UMETA(DisplayName = "Constitution"),
	CHA		UMETA(DisplayName = "Charisma"),
	INT		UMETA(DisplayName = "Intelligence"),
	WIS		UMETA(DisplayName = "Wisdom")
};

/**
 * Skills that a Hero can improve and excel at (0-100 range)
 * Each skill has one or more related attributes that determine its cap
 */
UENUM(BlueprintType)
enum class ESkills : uint8
{
	Smithing	UMETA(DisplayName = "Smithing"),		// STR
	Alchemy		UMETA(DisplayName = "Alchemy"),			// INT
	Fletching	UMETA(DisplayName = "Fletching"),		// DEX
	Cooking		UMETA(DisplayName = "Cooking"),			// WIS
	Farming		UMETA(DisplayName = "Farming"),			// WIS
	Fishing		UMETA(DisplayName = "Fishing"),			// DEX, WIS
	Tailoring	UMETA(DisplayName = "Tailoring"),		// DEX, INT
	Hunting		UMETA(DisplayName = "Hunting"),			// DEX, WIS
	Martial		UMETA(DisplayName = "Martial"),			// STR, DEX
	Carpentry	UMETA(DisplayName = "Carpentry"),		// STR, DEX
	Masonry		UMETA(DisplayName = "Masonry"),			// STR, CON
	Trading		UMETA(DisplayName = "Trading"),			// CHA, INT
	Brewing		UMETA(DisplayName = "Brewing"),			// WIS, INT
	Engineering	UMETA(DisplayName = "Engineering"),		// INT, DEX
	Leadership	UMETA(DisplayName = "Leadership"),		// CHA, CON
	Diplomat	UMETA(DisplayName = "Diplomat"),		// CHA, WIS
	Medicine	UMETA(DisplayName = "Medicine")			// INT, WIS
};

/**
 * Hero Tier/Rarity levels
 */
UENUM(BlueprintType)
enum class EHeroTiers : uint8
{
	Common		UMETA(DisplayName = "Common"),
	Uncommon	UMETA(DisplayName = "Uncommon"),
	Rare		UMETA(DisplayName = "Rare"),
	Epic		UMETA(DisplayName = "Epic"),
	Legendary	UMETA(DisplayName = "Legendary")
};

/**
 * Regions where heroes can originate from
 */
UENUM(BlueprintType)
enum class ERegions : uint8
{
	NorthernWilds	UMETA(DisplayName = "Northern Wilds"),
	EasternMarshes	UMETA(DisplayName = "Eastern Marshes"),
	SouthernDesert	UMETA(DisplayName = "Southern Desert"),
	WesternCoast	UMETA(DisplayName = "Western Coast"),
	CentralKingdom	UMETA(DisplayName = "Central Kingdom"),
	MountainPeaks	UMETA(DisplayName = "Mountain Peaks"),
	DarkForest		UMETA(DisplayName = "Dark Forest"),
	IslandArchipelago UMETA(DisplayName = "Island Archipelago")
};

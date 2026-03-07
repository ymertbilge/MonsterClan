// Copyright by Neon Fiction Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HeroEnums.h"
#include "HeroDataStructs.generated.h"

// Forward declarations
class UDA_TraitBase;
class UTexture2D;
class USkeletalMesh;

/**
 * Hero Data Structure - Used as DataTable rows to define hero specifications
 * Each hero inherits their base stats and properties from this struct
 */
USTRUCT(BlueprintType)
struct FHeroData : public FTableRowBase
{
	GENERATED_BODY()

	/** Unique identifier for the hero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Identity")
	FString ID;

	/** Display name shown in UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Identity")
	FName DisplayName;

	/** Starting level of the hero (default: 1) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Progression")
	int32 Level = 1;

	/** Lore and background description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Identity", meta = (MultiLine = true))
	FText Description;

	/** 2D portrait icon for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Visuals")
	TSoftObjectPtr<UTexture2D> Icon;

	/** 3D character model */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Visuals")
	TSoftObjectPtr<USkeletalMesh> CharacterMesh;

	/** Base attribute values (STR, DEX, CON, CHA, INT, WIS) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Stats")
	TMap<EAttributes, float> BaseAttributeValues;

	/** Base skill values (0-100 for each of the 16 skills) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Stats")
	TMap<ESkills, float> BaseSkillValues;

	/** Default traits this hero starts with */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Traits")
	TArray<TSoftObjectPtr<UDA_TraitBase>> DefaultTraits;

	/** Default items equipped (placeholder for now - can use BP_Item class references later) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Equipment")
	TArray<TSoftClassPtr<UObject>> DefaultItems;

	/** Daily/periodic cost to maintain this hero in the guild */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Economy")
	float Upkeep = 0.0f;

	/** One-time cost to recruit this hero */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Economy")
	float RecruitmentCost = 0.0f;

	/** Base mood/morale value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Social")
	float MoodBase = 50.0f;

	/** Hero tier/rarity with associated color */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Identity")
	EHeroTiers HeroTier = EHeroTiers::Common;

	/** Color associated with hero tier for UI display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Identity")
	FLinearColor TierColor = FLinearColor::White;

	/** Region of origin */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Identity")
	ERegions Region = ERegions::CentralKingdom;

	FHeroData()
	{
		// Initialize default attribute values
		BaseAttributeValues.Add(EAttributes::STR, 10.0f);
		BaseAttributeValues.Add(EAttributes::DEX, 10.0f);
		BaseAttributeValues.Add(EAttributes::CON, 10.0f);
		BaseAttributeValues.Add(EAttributes::CHA, 10.0f);
		BaseAttributeValues.Add(EAttributes::INT, 10.0f);
		BaseAttributeValues.Add(EAttributes::WIS, 10.0f);

		// Initialize default skill values
		BaseSkillValues.Add(ESkills::Smithing, 0.0f);
		BaseSkillValues.Add(ESkills::Alchemy, 0.0f);
		BaseSkillValues.Add(ESkills::Fletching, 0.0f);
		BaseSkillValues.Add(ESkills::Cooking, 0.0f);
		BaseSkillValues.Add(ESkills::Farming, 0.0f);
		BaseSkillValues.Add(ESkills::Fishing, 0.0f);
		BaseSkillValues.Add(ESkills::Tailoring, 0.0f);
		BaseSkillValues.Add(ESkills::Hunting, 0.0f);
		BaseSkillValues.Add(ESkills::Martial, 0.0f);
		BaseSkillValues.Add(ESkills::Carpentry, 0.0f);
		BaseSkillValues.Add(ESkills::Masonry, 0.0f);
		BaseSkillValues.Add(ESkills::Trading, 0.0f);
		BaseSkillValues.Add(ESkills::Brewing, 0.0f);
		BaseSkillValues.Add(ESkills::Engineering, 0.0f);
		BaseSkillValues.Add(ESkills::Leadership, 0.0f);
		BaseSkillValues.Add(ESkills::Diplomat, 0.0f);
		BaseSkillValues.Add(ESkills::Medicine, 0.0f);
	}
};

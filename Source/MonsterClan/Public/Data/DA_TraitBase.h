// Copyright by Neon Fiction Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "DA_TraitBase.generated.h"

/**
 * Trait Data Asset - Defines passive effects that heroes can have
 * Examples: Industrious (+15% crafting output), Ghost Hunter (+20% vs ghosts), etc.
 */
UCLASS(BlueprintType)
class MONSTERCLAN_API UDA_TraitBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Unique name/identifier for this trait */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Identity")
	FName TraitID;

	/** Display name shown in UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Identity")
	FText TraitName;

	/** Description explaining what the trait does */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Identity", meta = (MultiLine = true))
	FText TraitDescription;

	/** Icon representing this trait in UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Visuals")
	TSoftObjectPtr<UTexture2D> TraitIcon;

	/** Gameplay tags associated with this trait (for quest affixes, conditions, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Gameplay")
	FGameplayTagContainer TraitTags;

	/** Gameplay Effect to apply when this trait is active (for stat modifiers) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Effects")
	TSubclassOf<UGameplayEffect> TraitEffect;

	/** Whether this trait prevents equipping magical items */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Restrictions")
	bool bBlocksMagicalItems = false;

	/** Whether this trait prevents equipping specific item types (can be extended) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Restrictions")
	bool bBlocksCertainEquipment = false;

	/** Rarity/quality tier of this trait (for UI display) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Identity")
	int32 TraitRarity = 0;

	/** Whether this is a positive, negative, or neutral trait */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Identity")
	bool bIsPositiveTrait = true;

	/** Whether this trait can be removed or is permanent */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Rules")
	bool bIsPermanent = true;

	/** Additional custom data that can be used by specific traits */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trait|Advanced")
	TMap<FName, float> CustomTraitData;

	UDA_TraitBase()
	{
		TraitID = FName("DefaultTrait");
		TraitName = FText::FromString("Default Trait");
		TraitDescription = FText::FromString("This is a default trait description.");
		TraitRarity = 0;
		bIsPositiveTrait = true;
		bIsPermanent = true;
		bBlocksMagicalItems = false;
		bBlocksCertainEquipment = false;
	}

	/** Get the FPrimaryAssetId for this asset (required for PrimaryDataAsset) */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Trait", GetFName());
	}
};

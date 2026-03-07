// Copyright by Neon Fiction Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Data/HeroEnums.h"
#include "Data/HeroDataStructs.h"
#include "HeroCharacterBase.generated.h"

// Forward declarations
class UAbilitySystemComponent;
class UHeroAttributeSet;
class UDA_TraitBase;
class UDataTable;
class UCurveFloat;
struct FStreamableManager;

UCLASS(Abstract)
class MONSTERCLAN_API AHeroCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHeroCharacterBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ========================================
	// GAS Components
	// ========================================

	/** Ability System Component for GAS integration */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Hero Attribute Set containing all stats, skills, and progression */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	TObjectPtr<UHeroAttributeSet> AttributeSet;

	// ========================================
	// Hero Data
	// ========================================

	/** Reference to the Hero Data Table */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Data")
	TObjectPtr<UDataTable> HeroDataTable;

	/** Row name to fetch this hero's data from the data table */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hero|Data")
	FName HeroDataRowName;

	/** Cached hero data after initialization */
	UPROPERTY(BlueprintReadOnly, Category = "Hero|Data")
	FHeroData CachedHeroData;

	/** Currently active traits on this hero */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ActiveTraits, Category = "Hero|Traits")
	TArray<TObjectPtr<UDA_TraitBase>> ActiveTraits;

	/** Map of active trait effects for proper removal tracking */
	TMap<TObjectPtr<UDA_TraitBase>, FActiveGameplayEffectHandle> ActiveTraitEffects;

	/** XP curve for level progression (1-20) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Progression")
	TObjectPtr<UCurveFloat> XPCurve;

	/** Flag indicating if hero has been fully initialized */
	bool bIsInitialized = false;

	// ========================================
	// Core Functions
	// ========================================

	/**
	 * Initialize hero from data table
	 * Pulls data and sets up base attributes, skills, and traits
	 */
	UFUNCTION(BlueprintCallable, Category = "Hero")
	virtual void InitializeHeroData();

	/**
	 * Update hero stats based on modifiers
	 * Called after level ups, skill changes, item equips, trait applications, etc.
	 */
	UFUNCTION(BlueprintCallable, Category = "Hero")
	virtual void UpdateHeroStats();

	// ========================================
	// Attribute & Skill Helpers
	// ========================================

	/** Get attribute value by enum */
	UFUNCTION(BlueprintPure, Category = "Hero|Attributes")
	float GetAttributeValue(EAttributes AttributeType) const;

	/** Get skill value by enum */
	UFUNCTION(BlueprintPure, Category = "Hero|Skills")
	float GetSkillValue(ESkills SkillType) const;

	/**
	 * Get skill cap based on related attributes
	 * Each skill has 1-2 related attributes that determine its maximum value
	 */
	UFUNCTION(BlueprintPure, Category = "Hero|Skills")
	float GetSkillCap(ESkills SkillType) const;

	// ========================================
	// Trait Management
	// ========================================

	/** Add a trait to this hero and apply its effects */
	UFUNCTION(BlueprintCallable, Category = "Hero|Traits")
	void AddTrait(UDA_TraitBase* Trait);

	/** Remove a trait from this hero and remove its effects */
	UFUNCTION(BlueprintCallable, Category = "Hero|Traits")
	void RemoveTrait(UDA_TraitBase* Trait);

	/** Check if hero has a specific trait */
	UFUNCTION(BlueprintPure, Category = "Hero|Traits")
	bool HasTrait(UDA_TraitBase* Trait) const;

	// ========================================
	// Progression System
	// ========================================

	/** Add experience to the hero and handle level ups */
	UFUNCTION(BlueprintCallable, Category = "Hero|Progression")
	void AddExperience(float Amount);

	/** Level up the hero and apply bonuses */
	UFUNCTION(BlueprintCallable, Category = "Hero|Progression")
	void LevelUp();

	/** Get required XP for next level from curve */
	UFUNCTION(BlueprintPure, Category = "Hero|Progression")
	float GetRequiredXPForLevel(int32 Level) const;

	// ========================================
	// Item System (Placeholder)
	// ========================================

	/** Check if hero can equip an item (checks trait restrictions) */
	UFUNCTION(BlueprintPure, Category = "Hero|Items")
	bool CanEquipMagicalItems() const;

private:
	/** Helper to initialize base attributes from hero data */
	void InitializeAttributes(const FHeroData& HeroData);

	/** Helper to initialize base skills from hero data */
	void InitializeSkills(const FHeroData& HeroData);

	/** Helper to apply default traits from hero data (async loading) */
	void ApplyDefaultTraits(const FHeroData& HeroData);

	/** Callback when traits are loaded asynchronously */
	void OnTraitsLoaded(TArray<FSoftObjectPath> LoadedPaths);

	/** Replication callback for active traits */
	UFUNCTION()
	void OnRep_ActiveTraits();

	/** Cleanup all active trait effects */
	void CleanupTraitEffects();
};

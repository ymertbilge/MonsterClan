// Copyright by Neon Fiction Studios

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HeroAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Hero Attribute Set - Contains all attributes and skills for heroes
 * Integrated with Gameplay Ability System for networking and modifier support
 */
UCLASS()
class MONSTERCLAN_API UHeroAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UHeroAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ========================================
	// Core Attributes (6 primary stats)
	// ========================================

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Dexterity)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Constitution)
	FGameplayAttributeData Constitution;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Constitution)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Charisma)
	FGameplayAttributeData Charisma;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Charisma)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Wisdom)
	FGameplayAttributeData Wisdom;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Wisdom)

	// ========================================
	// Skills (17 total - 0 to 100 range)
	// ========================================

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Smithing)
	FGameplayAttributeData Smithing;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Smithing)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Alchemy)
	FGameplayAttributeData Alchemy;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Alchemy)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Fletching)
	FGameplayAttributeData Fletching;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Fletching)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Cooking)
	FGameplayAttributeData Cooking;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Cooking)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Farming)
	FGameplayAttributeData Farming;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Farming)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Fishing)
	FGameplayAttributeData Fishing;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Fishing)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Tailoring)
	FGameplayAttributeData Tailoring;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Tailoring)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Hunting)
	FGameplayAttributeData Hunting;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Hunting)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Martial)
	FGameplayAttributeData Martial;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Martial)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Carpentry)
	FGameplayAttributeData Carpentry;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Carpentry)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Masonry)
	FGameplayAttributeData Masonry;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Masonry)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Trading)
	FGameplayAttributeData Trading;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Trading)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Brewing)
	FGameplayAttributeData Brewing;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Brewing)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Engineering)
	FGameplayAttributeData Engineering;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Engineering)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Leadership)
	FGameplayAttributeData Leadership;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Leadership)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Diplomat)
	FGameplayAttributeData Diplomat;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Diplomat)

	UPROPERTY(BlueprintReadOnly, Category = "Skills", ReplicatedUsing = OnRep_Medicine)
	FGameplayAttributeData Medicine;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Medicine)

	// ========================================
	// Progression Attributes
	// ========================================

	UPROPERTY(BlueprintReadOnly, Category = "Progression", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Level)

	UPROPERTY(BlueprintReadOnly, Category = "Progression", ReplicatedUsing = OnRep_Experience)
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Experience)

	UPROPERTY(BlueprintReadOnly, Category = "Progression", ReplicatedUsing = OnRep_MaxExperience)
	FGameplayAttributeData MaxExperience;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, MaxExperience)

protected:
	// ========================================
	// Replication Functions - Attributes
	// ========================================

	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UFUNCTION()
	virtual void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity);

	UFUNCTION()
	virtual void OnRep_Constitution(const FGameplayAttributeData& OldConstitution);

	UFUNCTION()
	virtual void OnRep_Charisma(const FGameplayAttributeData& OldCharisma);

	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);

	UFUNCTION()
	virtual void OnRep_Wisdom(const FGameplayAttributeData& OldWisdom);

	// ========================================
	// Replication Functions - Skills
	// ========================================

	UFUNCTION()
	virtual void OnRep_Smithing(const FGameplayAttributeData& OldSmithing);

	UFUNCTION()
	virtual void OnRep_Alchemy(const FGameplayAttributeData& OldAlchemy);

	UFUNCTION()
	virtual void OnRep_Fletching(const FGameplayAttributeData& OldFletching);

	UFUNCTION()
	virtual void OnRep_Cooking(const FGameplayAttributeData& OldCooking);

	UFUNCTION()
	virtual void OnRep_Farming(const FGameplayAttributeData& OldFarming);

	UFUNCTION()
	virtual void OnRep_Fishing(const FGameplayAttributeData& OldFishing);

	UFUNCTION()
	virtual void OnRep_Tailoring(const FGameplayAttributeData& OldTailoring);

	UFUNCTION()
	virtual void OnRep_Hunting(const FGameplayAttributeData& OldHunting);

	UFUNCTION()
	virtual void OnRep_Martial(const FGameplayAttributeData& OldMartial);

	UFUNCTION()
	virtual void OnRep_Carpentry(const FGameplayAttributeData& OldCarpentry);

	UFUNCTION()
	virtual void OnRep_Masonry(const FGameplayAttributeData& OldMasonry);

	UFUNCTION()
	virtual void OnRep_Trading(const FGameplayAttributeData& OldTrading);

	UFUNCTION()
	virtual void OnRep_Brewing(const FGameplayAttributeData& OldBrewing);

	UFUNCTION()
	virtual void OnRep_Engineering(const FGameplayAttributeData& OldEngineering);

	UFUNCTION()
	virtual void OnRep_Leadership(const FGameplayAttributeData& OldLeadership);

	UFUNCTION()
	virtual void OnRep_Diplomat(const FGameplayAttributeData& OldDiplomat);

	UFUNCTION()
	virtual void OnRep_Medicine(const FGameplayAttributeData& OldMedicine);

	// ========================================
	// Replication Functions - Progression
	// ========================================

	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldLevel);

	UFUNCTION()
	virtual void OnRep_Experience(const FGameplayAttributeData& OldExperience);

	UFUNCTION()
	virtual void OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience);

private:
	/** Helper function to clamp attribute values */
	void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const;
};

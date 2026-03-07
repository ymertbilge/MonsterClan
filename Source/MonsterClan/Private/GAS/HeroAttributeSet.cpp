// Copyright by Neon Fiction Studios

#include "GAS/HeroAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UHeroAttributeSet::UHeroAttributeSet()
{
	// Initialize default values
	InitStrength(10.0f);
	InitDexterity(10.0f);
	InitConstitution(10.0f);
	InitCharisma(10.0f);
	InitIntelligence(10.0f);
	InitWisdom(10.0f);

	// Skills start at 0
	InitSmithing(0.0f);
	InitAlchemy(0.0f);
	InitFletching(0.0f);
	InitCooking(0.0f);
	InitFarming(0.0f);
	InitFishing(0.0f);
	InitTailoring(0.0f);
	InitHunting(0.0f);
	InitMartial(0.0f);
	InitCarpentry(0.0f);
	InitMasonry(0.0f);
	InitTrading(0.0f);
	InitBrewing(0.0f);
	InitEngineering(0.0f);
	InitLeadership(0.0f);
	InitDiplomat(0.0f);
	InitMedicine(0.0f);

	// Progression
	InitLevel(1.0f);
	InitExperience(0.0f);
	InitMaxExperience(100.0f);
}

void UHeroAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate all attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Constitution, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Charisma, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Wisdom, COND_None, REPNOTIFY_Always);

	// Replicate all skills
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Smithing, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Alchemy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Fletching, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Cooking, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Farming, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Fishing, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Tailoring, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Hunting, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Martial, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Carpentry, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Masonry, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Trading, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Brewing, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Engineering, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Leadership, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Diplomat, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Medicine, COND_None, REPNOTIFY_Always);

	// Replicate progression
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, Experience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHeroAttributeSet, MaxExperience, COND_None, REPNOTIFY_Always);
}

void UHeroAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttributeOnChange(Attribute, NewValue);
}

void UHeroAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Handle XP overflow into level ups (can be extended later)
	if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		// Clamp experience
		SetExperience(FMath::Clamp(GetExperience(), 0.0f, GetMaxExperience()));
	}
}

void UHeroAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// Clamp core attributes (no negative values, reasonable max)
	if (Attribute == GetStrengthAttribute() || Attribute == GetDexterityAttribute() ||
		Attribute == GetConstitutionAttribute() || Attribute == GetCharismaAttribute() ||
		Attribute == GetIntelligenceAttribute() || Attribute == GetWisdomAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 999.0f);
	}
	// Clamp skills to 0-100 range
	else if (Attribute == GetSmithingAttribute() || Attribute == GetAlchemyAttribute() ||
		Attribute == GetFletchingAttribute() || Attribute == GetCookingAttribute() ||
		Attribute == GetFarmingAttribute() || Attribute == GetFishingAttribute() ||
		Attribute == GetTailoringAttribute() || Attribute == GetHuntingAttribute() ||
		Attribute == GetMartialAttribute() || Attribute == GetCarpentryAttribute() ||
		Attribute == GetMasonryAttribute() || Attribute == GetTradingAttribute() ||
		Attribute == GetBrewingAttribute() || Attribute == GetEngineeringAttribute() ||
		Attribute == GetLeadershipAttribute() || Attribute == GetDiplomatAttribute() ||
		Attribute == GetMedicineAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
	}
	// Clamp level (1-20)
	else if (Attribute == GetLevelAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 1.0f, 20.0f);
	}
	// Clamp experience (non-negative)
	else if (Attribute == GetExperienceAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
}

// ========================================
// OnRep Functions - Attributes
// ========================================

void UHeroAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Strength, OldStrength);
}

void UHeroAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Dexterity, OldDexterity);
}

void UHeroAttributeSet::OnRep_Constitution(const FGameplayAttributeData& OldConstitution)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Constitution, OldConstitution);
}

void UHeroAttributeSet::OnRep_Charisma(const FGameplayAttributeData& OldCharisma)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Charisma, OldCharisma);
}

void UHeroAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Intelligence, OldIntelligence);
}

void UHeroAttributeSet::OnRep_Wisdom(const FGameplayAttributeData& OldWisdom)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Wisdom, OldWisdom);
}

// ========================================
// OnRep Functions - Skills
// ========================================

void UHeroAttributeSet::OnRep_Smithing(const FGameplayAttributeData& OldSmithing)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Smithing, OldSmithing);
}

void UHeroAttributeSet::OnRep_Alchemy(const FGameplayAttributeData& OldAlchemy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Alchemy, OldAlchemy);
}

void UHeroAttributeSet::OnRep_Fletching(const FGameplayAttributeData& OldFletching)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Fletching, OldFletching);
}

void UHeroAttributeSet::OnRep_Cooking(const FGameplayAttributeData& OldCooking)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Cooking, OldCooking);
}

void UHeroAttributeSet::OnRep_Farming(const FGameplayAttributeData& OldFarming)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Farming, OldFarming);
}

void UHeroAttributeSet::OnRep_Fishing(const FGameplayAttributeData& OldFishing)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Fishing, OldFishing);
}

void UHeroAttributeSet::OnRep_Tailoring(const FGameplayAttributeData& OldTailoring)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Tailoring, OldTailoring);
}

void UHeroAttributeSet::OnRep_Hunting(const FGameplayAttributeData& OldHunting)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Hunting, OldHunting);
}

void UHeroAttributeSet::OnRep_Martial(const FGameplayAttributeData& OldMartial)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Martial, OldMartial);
}

void UHeroAttributeSet::OnRep_Carpentry(const FGameplayAttributeData& OldCarpentry)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Carpentry, OldCarpentry);
}

void UHeroAttributeSet::OnRep_Masonry(const FGameplayAttributeData& OldMasonry)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Masonry, OldMasonry);
}

void UHeroAttributeSet::OnRep_Trading(const FGameplayAttributeData& OldTrading)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Trading, OldTrading);
}

void UHeroAttributeSet::OnRep_Brewing(const FGameplayAttributeData& OldBrewing)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Brewing, OldBrewing);
}

void UHeroAttributeSet::OnRep_Engineering(const FGameplayAttributeData& OldEngineering)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Engineering, OldEngineering);
}

void UHeroAttributeSet::OnRep_Leadership(const FGameplayAttributeData& OldLeadership)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Leadership, OldLeadership);
}

void UHeroAttributeSet::OnRep_Diplomat(const FGameplayAttributeData& OldDiplomat)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Diplomat, OldDiplomat);
}

void UHeroAttributeSet::OnRep_Medicine(const FGameplayAttributeData& OldMedicine)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Medicine, OldMedicine);
}

// ========================================
// OnRep Functions - Progression
// ========================================

void UHeroAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Level, OldLevel);
}

void UHeroAttributeSet::OnRep_Experience(const FGameplayAttributeData& OldExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, Experience, OldExperience);
}

void UHeroAttributeSet::OnRep_MaxExperience(const FGameplayAttributeData& OldMaxExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHeroAttributeSet, MaxExperience, OldMaxExperience);
}

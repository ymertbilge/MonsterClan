// Copyright by Neon Fiction Studios

#include "Character/HeroCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GAS/HeroAttributeSet.h"
#include "Data/DA_TraitBase.h"
#include "Data/HeroDataStructs.h"
#include "Engine/DataTable.h"
#include "Curves/CurveFloat.h"
#include "GameplayEffect.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

AHeroCharacterBase::AHeroCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create Attribute Set
	AttributeSet = CreateDefaultSubobject<UHeroAttributeSet>(TEXT("AttributeSet"));

	// Enable replication
	bReplicates = true;
}

void AHeroCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate active traits array
	DOREPLIFETIME(AHeroCharacterBase, ActiveTraits);
}

UAbilitySystemComponent* AHeroCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHeroCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialize ASC for local player
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// Initialize hero data if row name is set
	if (!HeroDataRowName.IsNone())
	{
		InitializeHeroData();
	}
}

void AHeroCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Cleanup all active trait effects
	CleanupTraitEffects();

	Super::EndPlay(EndPlayReason);
}

// ========================================
// Core Functions
// ========================================

void AHeroCharacterBase::InitializeHeroData()
{
	if (!HeroDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("HeroCharacterBase: HeroDataTable is not set!"));
		return;
	}

	if (HeroDataRowName.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("HeroCharacterBase: HeroDataRowName is not set!"));
		return;
	}

	// Fetch hero data from data table
	FHeroData* HeroData = HeroDataTable->FindRow<FHeroData>(HeroDataRowName, TEXT("InitializeHeroData"));
	if (!HeroData)
	{
		UE_LOG(LogTemp, Error, TEXT("HeroCharacterBase: Failed to find row '%s' in HeroDataTable!"), *HeroDataRowName.ToString());
		return;
	}

	// Cache hero data
	CachedHeroData = *HeroData;

	// Initialize components
	InitializeAttributes(*HeroData);
	InitializeSkills(*HeroData);
	ApplyDefaultTraits(*HeroData);

	// Set initial level and XP
	if (AttributeSet)
	{
		AttributeSet->SetLevel(HeroData->Level);
		AttributeSet->SetExperience(0.0f);

		// Set max XP from curve if available
		if (XPCurve)
		{
			float MaxXP = GetRequiredXPForLevel(HeroData->Level + 1);
			AttributeSet->SetMaxExperience(MaxXP);
		}
	}

	// Update stats to apply all modifiers
	UpdateHeroStats();

	// Mark as initialized (traits may still be loading asynchronously)
	bIsInitialized = true;

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Successfully initialized hero '%s'"), *HeroData->DisplayName.ToString());
}

void AHeroCharacterBase::UpdateHeroStats()
{
	if (!AttributeSet || !AbilitySystemComponent)
	{
		return;
	}

	// This function recalculates all modifiers and ensures skill caps are enforced
	// In a full implementation, you would:
	// 1. Recalculate skill caps based on current attributes
	// 2. Apply trait modifiers
	// 3. Apply item modifiers
	// 4. Apply global event modifiers
	// 5. Clamp skills to their caps

	// For now, we'll clamp skills to their calculated caps
	if (AttributeSet->GetSmithing() > GetSkillCap(ESkills::Smithing))
	{
		AttributeSet->SetSmithing(GetSkillCap(ESkills::Smithing));
	}
	if (AttributeSet->GetAlchemy() > GetSkillCap(ESkills::Alchemy))
	{
		AttributeSet->SetAlchemy(GetSkillCap(ESkills::Alchemy));
	}
	if (AttributeSet->GetFletching() > GetSkillCap(ESkills::Fletching))
	{
		AttributeSet->SetFletching(GetSkillCap(ESkills::Fletching));
	}
	if (AttributeSet->GetCooking() > GetSkillCap(ESkills::Cooking))
	{
		AttributeSet->SetCooking(GetSkillCap(ESkills::Cooking));
	}
	if (AttributeSet->GetFarming() > GetSkillCap(ESkills::Farming))
	{
		AttributeSet->SetFarming(GetSkillCap(ESkills::Farming));
	}
	if (AttributeSet->GetFishing() > GetSkillCap(ESkills::Fishing))
	{
		AttributeSet->SetFishing(GetSkillCap(ESkills::Fishing));
	}
	if (AttributeSet->GetTailoring() > GetSkillCap(ESkills::Tailoring))
	{
		AttributeSet->SetTailoring(GetSkillCap(ESkills::Tailoring));
	}
	if (AttributeSet->GetHunting() > GetSkillCap(ESkills::Hunting))
	{
		AttributeSet->SetHunting(GetSkillCap(ESkills::Hunting));
	}
	if (AttributeSet->GetMartial() > GetSkillCap(ESkills::Martial))
	{
		AttributeSet->SetMartial(GetSkillCap(ESkills::Martial));
	}
	if (AttributeSet->GetCarpentry() > GetSkillCap(ESkills::Carpentry))
	{
		AttributeSet->SetCarpentry(GetSkillCap(ESkills::Carpentry));
	}
	if (AttributeSet->GetMasonry() > GetSkillCap(ESkills::Masonry))
	{
		AttributeSet->SetMasonry(GetSkillCap(ESkills::Masonry));
	}
	if (AttributeSet->GetTrading() > GetSkillCap(ESkills::Trading))
	{
		AttributeSet->SetTrading(GetSkillCap(ESkills::Trading));
	}
	if (AttributeSet->GetBrewing() > GetSkillCap(ESkills::Brewing))
	{
		AttributeSet->SetBrewing(GetSkillCap(ESkills::Brewing));
	}
	if (AttributeSet->GetEngineering() > GetSkillCap(ESkills::Engineering))
	{
		AttributeSet->SetEngineering(GetSkillCap(ESkills::Engineering));
	}
	if (AttributeSet->GetLeadership() > GetSkillCap(ESkills::Leadership))
	{
		AttributeSet->SetLeadership(GetSkillCap(ESkills::Leadership));
	}
	if (AttributeSet->GetDiplomat() > GetSkillCap(ESkills::Diplomat))
	{
		AttributeSet->SetDiplomat(GetSkillCap(ESkills::Diplomat));
	}
	if (AttributeSet->GetMedicine() > GetSkillCap(ESkills::Medicine))
	{
		AttributeSet->SetMedicine(GetSkillCap(ESkills::Medicine));
	}
}

// ========================================
// Attribute & Skill Helpers
// ========================================

float AHeroCharacterBase::GetAttributeValue(EAttributes AttributeType) const
{
	if (!AttributeSet)
	{
		return 0.0f;
	}

	switch (AttributeType)
	{
	case EAttributes::STR: return AttributeSet->GetStrength();
	case EAttributes::DEX: return AttributeSet->GetDexterity();
	case EAttributes::CON: return AttributeSet->GetConstitution();
	case EAttributes::CHA: return AttributeSet->GetCharisma();
	case EAttributes::INT: return AttributeSet->GetIntelligence();
	case EAttributes::WIS: return AttributeSet->GetWisdom();
	default: return 0.0f;
	}
}

float AHeroCharacterBase::GetSkillValue(ESkills SkillType) const
{
	if (!AttributeSet)
	{
		return 0.0f;
	}

	switch (SkillType)
	{
	case ESkills::Smithing: return AttributeSet->GetSmithing();
	case ESkills::Alchemy: return AttributeSet->GetAlchemy();
	case ESkills::Fletching: return AttributeSet->GetFletching();
	case ESkills::Cooking: return AttributeSet->GetCooking();
	case ESkills::Farming: return AttributeSet->GetFarming();
	case ESkills::Fishing: return AttributeSet->GetFishing();
	case ESkills::Tailoring: return AttributeSet->GetTailoring();
	case ESkills::Hunting: return AttributeSet->GetHunting();
	case ESkills::Martial: return AttributeSet->GetMartial();
	case ESkills::Carpentry: return AttributeSet->GetCarpentry();
	case ESkills::Masonry: return AttributeSet->GetMasonry();
	case ESkills::Trading: return AttributeSet->GetTrading();
	case ESkills::Brewing: return AttributeSet->GetBrewing();
	case ESkills::Engineering: return AttributeSet->GetEngineering();
	case ESkills::Leadership: return AttributeSet->GetLeadership();
	case ESkills::Diplomat: return AttributeSet->GetDiplomat();
	case ESkills::Medicine: return AttributeSet->GetMedicine();
	default: return 0.0f;
	}
}

float AHeroCharacterBase::GetSkillCap(ESkills SkillType) const
{
	if (!AttributeSet)
	{
		return 0.0f;
	}

	// Each skill cap is based on its related attribute(s)
	// For skills with 2 attributes, we average them
	switch (SkillType)
	{
	case ESkills::Smithing: // STR
		return AttributeSet->GetStrength();

	case ESkills::Alchemy: // INT
		return AttributeSet->GetIntelligence();

	case ESkills::Fletching: // DEX
		return AttributeSet->GetDexterity();

	case ESkills::Cooking: // WIS
		return AttributeSet->GetWisdom();

	case ESkills::Farming: // WIS
		return AttributeSet->GetWisdom();

	case ESkills::Fishing: // DEX, WIS
		return (AttributeSet->GetDexterity() + AttributeSet->GetWisdom()) / 2.0f;

	case ESkills::Tailoring: // DEX, INT
		return (AttributeSet->GetDexterity() + AttributeSet->GetIntelligence()) / 2.0f;

	case ESkills::Hunting: // DEX, WIS
		return (AttributeSet->GetDexterity() + AttributeSet->GetWisdom()) / 2.0f;

	case ESkills::Martial: // STR, DEX
		return (AttributeSet->GetStrength() + AttributeSet->GetDexterity()) / 2.0f;

	case ESkills::Carpentry: // STR, DEX
		return (AttributeSet->GetStrength() + AttributeSet->GetDexterity()) / 2.0f;

	case ESkills::Masonry: // STR, CON
		return (AttributeSet->GetStrength() + AttributeSet->GetConstitution()) / 2.0f;

	case ESkills::Trading: // CHA, INT
		return (AttributeSet->GetCharisma() + AttributeSet->GetIntelligence()) / 2.0f;

	case ESkills::Brewing: // WIS, INT
		return (AttributeSet->GetWisdom() + AttributeSet->GetIntelligence()) / 2.0f;

	case ESkills::Engineering: // INT, DEX
		return (AttributeSet->GetIntelligence() + AttributeSet->GetDexterity()) / 2.0f;

	case ESkills::Leadership: // CHA, CON
		return (AttributeSet->GetCharisma() + AttributeSet->GetConstitution()) / 2.0f;

	case ESkills::Diplomat: // CHA, WIS
		return (AttributeSet->GetCharisma() + AttributeSet->GetWisdom()) / 2.0f;

	case ESkills::Medicine: // INT, WIS
		return (AttributeSet->GetIntelligence() + AttributeSet->GetWisdom()) / 2.0f;

	default:
		return 0.0f;
	}
}

// ========================================
// Trait Management
// ========================================

void AHeroCharacterBase::AddTrait(UDA_TraitBase* Trait)
{
	if (!Trait || !AbilitySystemComponent)
	{
		return;
	}

	// Check if already has trait
	if (HasTrait(Trait))
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroCharacterBase: Hero already has trait '%s'"), *Trait->TraitName.ToString());
		return;
	}

	// Add to active traits
	ActiveTraits.Add(Trait);

	// Apply trait's gameplay effect if it has one
	if (Trait->TraitEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Trait->TraitEffect, 1.0f, EffectContext);
		if (SpecHandle.IsValid())
		{
			// Apply effect and store the handle for later removal
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

			if (EffectHandle.IsValid())
			{
				ActiveTraitEffects.Add(Trait, EffectHandle);
				UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Applied effect for trait '%s' (Handle: %s)"),
					*Trait->TraitName.ToString(), *EffectHandle.ToString());
			}
		}
	}

	// Update stats to apply new modifiers
	UpdateHeroStats();

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Added trait '%s'"), *Trait->TraitName.ToString());
}

void AHeroCharacterBase::RemoveTrait(UDA_TraitBase* Trait)
{
	if (!Trait || !AbilitySystemComponent)
	{
		return;
	}

	// Check if trait is permanent
	if (Trait->bIsPermanent)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroCharacterBase: Cannot remove permanent trait '%s'"), *Trait->TraitName.ToString());
		return;
	}

	// Remove from active traits
	ActiveTraits.Remove(Trait);

	// Remove trait's gameplay effect using tracked handle
	if (FActiveGameplayEffectHandle* EffectHandle = ActiveTraitEffects.Find(Trait))
	{
		if (EffectHandle->IsValid())
		{
			bool bRemoved = AbilitySystemComponent->RemoveActiveGameplayEffect(*EffectHandle);
			if (bRemoved)
			{
				UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Removed effect for trait '%s' (Handle: %s)"),
					*Trait->TraitName.ToString(), *EffectHandle->ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("HeroCharacterBase: Failed to remove effect for trait '%s'"),
					*Trait->TraitName.ToString());
			}
		}

		// Remove from map regardless
		ActiveTraitEffects.Remove(Trait);
	}

	// Update stats
	UpdateHeroStats();

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Removed trait '%s'"), *Trait->TraitName.ToString());
}

bool AHeroCharacterBase::HasTrait(UDA_TraitBase* Trait) const
{
	return ActiveTraits.Contains(Trait);
}

// ========================================
// Progression System
// ========================================

void AHeroCharacterBase::AddExperience(float Amount)
{
	if (!AttributeSet)
	{
		return;
	}

	float CurrentXP = AttributeSet->GetExperience();
	float NewXP = CurrentXP + Amount;
	float MaxXP = AttributeSet->GetMaxExperience();

	// Check for level up
	while (NewXP >= MaxXP && AttributeSet->GetLevel() < 20.0f)
	{
		NewXP -= MaxXP;
		LevelUp();

		// Update max XP for new level
		MaxXP = GetRequiredXPForLevel(FMath::RoundToInt(AttributeSet->GetLevel()) + 1);
		AttributeSet->SetMaxExperience(MaxXP);
	}

	AttributeSet->SetExperience(NewXP);
}

void AHeroCharacterBase::LevelUp()
{
	if (!AttributeSet)
	{
		return;
	}

	float CurrentLevel = AttributeSet->GetLevel();
	if (CurrentLevel >= 20.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroCharacterBase: Hero is already max level!"));
		return;
	}

	// Increase level
	AttributeSet->SetLevel(CurrentLevel + 1.0f);

	// Apply level up bonuses (simplified - could be more complex)
	// For example, you could give attribute points to spend, increase base stats, etc.

	UpdateHeroStats();

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Hero leveled up to level %d!"), FMath::RoundToInt(AttributeSet->GetLevel()));
}

float AHeroCharacterBase::GetRequiredXPForLevel(int32 Level) const
{
	if (!XPCurve)
	{
		// Default XP curve if none is set (exponential growth)
		return 100.0f * FMath::Pow(1.5f, Level - 1);
	}

	return XPCurve->GetFloatValue(Level);
}

// ========================================
// Item System (Placeholder)
// ========================================

bool AHeroCharacterBase::CanEquipMagicalItems() const
{
	// Check if any active trait blocks magical items
	for (const UDA_TraitBase* Trait : ActiveTraits)
	{
		if (Trait && Trait->bBlocksMagicalItems)
		{
			return false;
		}
	}

	return true;
}

// ========================================
// Private Helper Functions
// ========================================

void AHeroCharacterBase::InitializeAttributes(const FHeroData& HeroData)
{
	if (!AttributeSet)
	{
		return;
	}

	// Set base attribute values from hero data
	for (const auto& AttributePair : HeroData.BaseAttributeValues)
	{
		switch (AttributePair.Key)
		{
		case EAttributes::STR:
			AttributeSet->SetStrength(AttributePair.Value);
			break;
		case EAttributes::DEX:
			AttributeSet->SetDexterity(AttributePair.Value);
			break;
		case EAttributes::CON:
			AttributeSet->SetConstitution(AttributePair.Value);
			break;
		case EAttributes::CHA:
			AttributeSet->SetCharisma(AttributePair.Value);
			break;
		case EAttributes::INT:
			AttributeSet->SetIntelligence(AttributePair.Value);
			break;
		case EAttributes::WIS:
			AttributeSet->SetWisdom(AttributePair.Value);
			break;
		}
	}
}

void AHeroCharacterBase::InitializeSkills(const FHeroData& HeroData)
{
	if (!AttributeSet)
	{
		return;
	}

	// Set base skill values from hero data
	for (const auto& SkillPair : HeroData.BaseSkillValues)
	{
		switch (SkillPair.Key)
		{
		case ESkills::Smithing:
			AttributeSet->SetSmithing(SkillPair.Value);
			break;
		case ESkills::Alchemy:
			AttributeSet->SetAlchemy(SkillPair.Value);
			break;
		case ESkills::Fletching:
			AttributeSet->SetFletching(SkillPair.Value);
			break;
		case ESkills::Cooking:
			AttributeSet->SetCooking(SkillPair.Value);
			break;
		case ESkills::Farming:
			AttributeSet->SetFarming(SkillPair.Value);
			break;
		case ESkills::Fishing:
			AttributeSet->SetFishing(SkillPair.Value);
			break;
		case ESkills::Tailoring:
			AttributeSet->SetTailoring(SkillPair.Value);
			break;
		case ESkills::Hunting:
			AttributeSet->SetHunting(SkillPair.Value);
			break;
		case ESkills::Martial:
			AttributeSet->SetMartial(SkillPair.Value);
			break;
		case ESkills::Carpentry:
			AttributeSet->SetCarpentry(SkillPair.Value);
			break;
		case ESkills::Masonry:
			AttributeSet->SetMasonry(SkillPair.Value);
			break;
		case ESkills::Trading:
			AttributeSet->SetTrading(SkillPair.Value);
			break;
		case ESkills::Brewing:
			AttributeSet->SetBrewing(SkillPair.Value);
			break;
		case ESkills::Engineering:
			AttributeSet->SetEngineering(SkillPair.Value);
			break;
		case ESkills::Leadership:
			AttributeSet->SetLeadership(SkillPair.Value);
			break;
		case ESkills::Diplomat:
			AttributeSet->SetDiplomat(SkillPair.Value);
			break;
		case ESkills::Medicine:
			AttributeSet->SetMedicine(SkillPair.Value);
			break;
		}
	}
}

void AHeroCharacterBase::ApplyDefaultTraits(const FHeroData& HeroData)
{
	// Collect trait paths for async loading
	TArray<FSoftObjectPath> TraitPaths;
	for (const TSoftObjectPtr<UDA_TraitBase>& TraitPtr : HeroData.DefaultTraits)
	{
		if (!TraitPtr.IsNull())
		{
			TraitPaths.Add(TraitPtr.ToSoftObjectPath());
		}
	}

	// If no traits to load, return early
	if (TraitPaths.Num() == 0)
	{
		return;
	}

	// Use Asset Manager's streamable manager for async loading
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	// Request async load with callback
	StreamableManager.RequestAsyncLoad(TraitPaths,
		FStreamableDelegate::CreateUObject(this, &AHeroCharacterBase::OnTraitsLoaded, TraitPaths));

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Started async loading %d traits"), TraitPaths.Num());
}

void AHeroCharacterBase::OnTraitsLoaded(TArray<FSoftObjectPath> LoadedPaths)
{
	// Verify we're still valid
	if (!IsValid(this) || !AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroCharacterBase: Trait loading completed but hero is no longer valid"));
		return;
	}

	int32 TraitsApplied = 0;

	// Load each trait and apply it
	for (const FSoftObjectPath& Path : LoadedPaths)
	{
		if (UDA_TraitBase* Trait = Cast<UDA_TraitBase>(Path.ResolveObject()))
		{
			AddTrait(Trait);
			TraitsApplied++;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HeroCharacterBase: Failed to load trait from path: %s"), *Path.ToString());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Applied %d/%d traits after async load"), TraitsApplied, LoadedPaths.Num());
}

void AHeroCharacterBase::OnRep_ActiveTraits()
{
	// When traits replicate, ensure effects are applied on clients
	if (!HasAuthority() && AbilitySystemComponent)
	{
		// Clear existing effects
		CleanupTraitEffects();

		// Reapply all traits
		TArray<TObjectPtr<UDA_TraitBase>> TraitsCopy = ActiveTraits;
		ActiveTraits.Empty();

		for (UDA_TraitBase* Trait : TraitsCopy)
		{
			if (Trait)
			{
				AddTrait(Trait);
			}
		}
	}
}

void AHeroCharacterBase::CleanupTraitEffects()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Remove all tracked trait effects
	for (const auto& Pair : ActiveTraitEffects)
	{
		if (Pair.Value.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(Pair.Value);
		}
	}

	// Clear the map
	ActiveTraitEffects.Empty();

	UE_LOG(LogTemp, Log, TEXT("HeroCharacterBase: Cleaned up all trait effects"));
}


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "TimeManager.generated.h"

/** Time speed modes */
UENUM(BlueprintType)
enum class ETimeSpeed : uint8
{
    Paused    UMETA(DisplayName = "Paused"),
    Normal    UMETA(DisplayName = "1x Speed"),
    Fast      UMETA(DisplayName = "2x Speed"),
    SuperFast UMETA(DisplayName = "3x Speed")
};

/** In-game date and time structure */
USTRUCT(BlueprintType)
struct FGameDateTime
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    int32 Year = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 1, ClampMax = 12))
    int32 Month = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 1, ClampMax = 30))
    int32 Day = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 0, ClampMax = 23))
    int32 Hour = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (ClampMin = 0, ClampMax = 59))
    int32 Minute = 0;

    FGameDateTime() {}
    FGameDateTime(int32 InYear, int32 InMonth, int32 InDay, int32 InHour, int32 InMinute)
        : Year(InYear), Month(InMonth), Day(InDay), Hour(InHour), Minute(InMinute) {}

    bool operator==(const FGameDateTime& Other) const
    {
        return Year == Other.Year && Month == Other.Month && Day == Other.Day && Hour == Other.Hour && Minute == Other.Minute;
    }

    bool operator>=(const FGameDateTime& Other) const
    {
        if (Year != Other.Year) return Year > Other.Year;
        if (Month != Other.Month) return Month > Other.Month;
        if (Day != Other.Day) return Day > Other.Day;
        if (Hour != Other.Hour) return Hour > Other.Hour;
        return Minute >= Other.Minute;
    }

    bool operator<(const FGameDateTime& Other) const
    {
        return !(*this >= Other);
    }

    FString ToString() const
    {
        return FString::Printf(TEXT("%04d-%02d-%02d %02d:%02d"), Year, Month, Day, Hour, Minute);
    }
};

/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChangedDelegate, FGameDateTime, NewDateTime);
DECLARE_DYNAMIC_DELEGATE(FTimeManagerDelegate);

USTRUCT()
struct FScheduledEvent
{
    GENERATED_BODY()

    UPROPERTY()
    FGameDateTime TargetTime;

    UPROPERTY()
    FTimeManagerDelegate Delegate;

    FScheduledEvent() {}
};

USTRUCT()
struct FScheduledIntervalEvent
{
    GENERATED_BODY()

    UPROPERTY()
    FGameDateTime StartTime;

    UPROPERTY()
    FGameDateTime EndTime;

    UPROPERTY()
    FTimeManagerDelegate OnStartDelegate;

    UPROPERTY()
    FTimeManagerDelegate OnEndDelegate;

    bool bStarted = false;
    bool bFinished = false;

    FScheduledIntervalEvent() {}
};

USTRUCT()
struct FConditionalEvent
{
    GENERATED_BODY()

    UPROPERTY()
    FTimeManagerDelegate Delegate;

    UPROPERTY()
    FString ConditionName;

    FConditionalEvent() {}
};

/**
 * ATimeManager
 * Central time manager for the game and Day/Night cycle controller.
 */
UCLASS()
class MONSTERCLAN_API ATimeManager : public AActor
{
    GENERATED_BODY()

public:
    ATimeManager();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    /** Current in-game time */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FGameDateTime CurrentDateTime;

    /** Time speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    ETimeSpeed TimeSpeed = ETimeSpeed::Normal;

    /** How many real seconds one game minute takes (At normal speed) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    float RealSecondsPerGameMinute = 1.0f;

    /** Triggered when time changes */
    UPROPERTY(BlueprintAssignable, Category = "Time")
    FOnTimeChangedDelegate OnTimeChanged;

    /** Sets the time to a specific speed */
    UFUNCTION(BlueprintCallable, Category = "Time")
    void SetTimeSpeed(ETimeSpeed NewSpeed);

    /** Schedules an event at a specific date */
    UFUNCTION(BlueprintCallable, Category = "Time")
    void ScheduleEventAtTime(FGameDateTime TargetTime, FTimeManagerDelegate Delegate);

    /** Schedules an event within a specific time interval */
    UFUNCTION(BlueprintCallable, Category = "Time")
    void ScheduleEventInInterval(FGameDateTime StartTime, FGameDateTime EndTime, FTimeManagerDelegate OnStart, FTimeManagerDelegate OnEnd);

    /** Schedules an event based on a condition */
    UFUNCTION(BlueprintCallable, Category = "Time")
    void ScheduleConditionalEvent(FTimeManagerDelegate Delegate, const FString& ConditionName);

    /** Sun (Directional Light) for Day/Night Cycle */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
    ADirectionalLight* SunLight;

private:
    float MinuteAccumulator = 0.0f;

    UPROPERTY()
    TArray<FScheduledEvent> ScheduledEvents;

    UPROPERTY()
    TArray<FScheduledIntervalEvent> IntervalEvents;

    UPROPERTY()
    TArray<FConditionalEvent> ConditionalEvents;

    void AdvanceTime();
    void UpdateSunPosition();
    void CheckEvents();
};

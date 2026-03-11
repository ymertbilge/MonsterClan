#include "Manager/TimeManager.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"

ATimeManager::ATimeManager()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentDateTime = FGameDateTime(1, 1, 1, 8, 0); // Start time: 08:00
}

void ATimeManager::BeginPlay()
{
    Super::BeginPlay();
    UpdateSunPosition();
}

void ATimeManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TimeSpeed == ETimeSpeed::Paused)
        return;

    float SpeedMultiplier = 1.0f;
    switch (TimeSpeed)
    {
    case ETimeSpeed::Fast: SpeedMultiplier = 2.0f; break;
    case ETimeSpeed::SuperFast: SpeedMultiplier = 3.0f; break;
    default: break;
    }

    MinuteAccumulator += DeltaTime * SpeedMultiplier;

    if (MinuteAccumulator >= RealSecondsPerGameMinute)
    {
        int32 MinutesToAdvance = FMath::FloorToInt(MinuteAccumulator / RealSecondsPerGameMinute);
        MinuteAccumulator -= MinutesToAdvance * RealSecondsPerGameMinute;

        for (int32 i = 0; i < MinutesToAdvance; ++i)
        {
            AdvanceTime();
        }
    }
}

void ATimeManager::SetTimeSpeed(ETimeSpeed NewSpeed)
{
    TimeSpeed = NewSpeed;
}

void ATimeManager::ScheduleEventAtTime(FGameDateTime TargetTime, FTimeManagerDelegate Delegate)
{
    FScheduledEvent NewEvent;
    NewEvent.TargetTime = TargetTime;
    NewEvent.Delegate = Delegate;
    ScheduledEvents.Add(NewEvent);
}

void ATimeManager::ScheduleEventInInterval(FGameDateTime StartTime, FGameDateTime EndTime, FTimeManagerDelegate OnStart, FTimeManagerDelegate OnEnd)
{
    FScheduledIntervalEvent NewEvent;
    NewEvent.StartTime = StartTime;
    NewEvent.EndTime = EndTime;
    NewEvent.OnStartDelegate = OnStart;
    NewEvent.OnEndDelegate = OnEnd;
    
    // If we are already within the interval, start immediately
    if (CurrentDateTime >= StartTime && CurrentDateTime < EndTime)
    {
        NewEvent.bStarted = true;
        OnStart.ExecuteIfBound();
    }

    IntervalEvents.Add(NewEvent);
}

void ATimeManager::ScheduleConditionalEvent(FTimeManagerDelegate Delegate, const FString& ConditionName)
{
    FConditionalEvent NewEvent;
    NewEvent.Delegate = Delegate;
    NewEvent.ConditionName = ConditionName;
    ConditionalEvents.Add(NewEvent);
}

void ATimeManager::AdvanceTime()
{
    CurrentDateTime.Minute++;

    if (CurrentDateTime.Minute >= 60)
    {
        CurrentDateTime.Minute = 0;
        CurrentDateTime.Hour++;

        if (CurrentDateTime.Hour >= 24)
        {
            CurrentDateTime.Hour = 0;
            CurrentDateTime.Day++;

            if (CurrentDateTime.Day > 30) // Simple calendar: 30 days per month
            {
                CurrentDateTime.Day = 1;
                CurrentDateTime.Month++;

                if (CurrentDateTime.Month > 12)
                {
                    CurrentDateTime.Month = 1;
                    CurrentDateTime.Year++;
                }
            }
        }
    }

    OnTimeChanged.Broadcast(CurrentDateTime);
    UpdateSunPosition();
    CheckEvents();
}

void ATimeManager::UpdateSunPosition()
{
    if (!SunLight)
        return;

    // Calculate sun angle based on total minutes of the day (0-1440 minutes)
    float TotalMinutes = (CurrentDateTime.Hour * 60.0f) + CurrentDateTime.Minute;
    
    // 00:00 -> -90 degrees (midnight)
    // 06:00 -> 0 degrees (sunrise)
    // 12:00 -> 90 degrees (noon)
    // 18:00 -> 180 degrees (sunset)
    
    float Pitch = (TotalMinutes / 1440.0f) * 360.0f - 90.0f;
    
    FRotator NewRotation(Pitch, -180.0f, 0.0f);
    SunLight->SetActorRotation(NewRotation);
}

void ATimeManager::CheckEvents()
{
    // Check scheduled single events
    for (int32 i = ScheduledEvents.Num() - 1; i >= 0; --i)
    {
        if (CurrentDateTime >= ScheduledEvents[i].TargetTime)
        {
            ScheduledEvents[i].Delegate.ExecuteIfBound();
            ScheduledEvents.RemoveAt(i);
        }
    }

    // Check interval events
    for (int32 i = IntervalEvents.Num() - 1; i >= 0; --i)
    {
        FScheduledIntervalEvent& Event = IntervalEvents[i];

        if (!Event.bStarted && CurrentDateTime >= Event.StartTime)
        {
            Event.bStarted = true;
            Event.OnStartDelegate.ExecuteIfBound();
        }

        if (Event.bStarted && !Event.bFinished && CurrentDateTime >= Event.EndTime)
        {
            Event.bFinished = true;
            Event.OnEndDelegate.ExecuteIfBound();
            IntervalEvents.RemoveAt(i);
        }
    }

    // Check conditional events
    for (int32 i = ConditionalEvents.Num() - 1; i >= 0; --i)
    {
        // Example logic for condition checking.
        // In a real project, this could use a specialized ConditionManager.
        bool bConditionMet = false;
        
        // Example:
        // if (ConditionalEvents[i].ConditionName == "IsNoon") bConditionMet = (CurrentDateTime.Hour == 12);
        
        if (bConditionMet)
        {
            ConditionalEvents[i].Delegate.ExecuteIfBound();
            ConditionalEvents.RemoveAt(i);
        }
    }
}

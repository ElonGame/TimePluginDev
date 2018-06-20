// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TimePlugin.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(TimePlugin);

void FTimePlugin::StartupModule()
{
	UE_LOG(TimePlugin, Warning, TEXT("TimePlugin StartupModle() Register OnWorldCreated delegate"));

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	//Create our delegate type
	FWorldDelegates::FWorldInitializationEvent::FDelegate OnWorldCreatedDelegate;
	//Declare which function we want to bind to
	OnWorldCreatedDelegate = FWorldDelegates::FWorldInitializationEvent::FDelegate::CreateRaw(this, &FTimePlugin::OnWorldCreated);
	//Declare which event we want to bind to
	FDelegateHandle OnWorldCreatedDelegateHandle = FWorldDelegates::OnPostWorldInitialization.Add(OnWorldCreatedDelegate);
}

void FTimePlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FTimePlugin::OnWorldCreated(UWorld* World, const UWorld::InitializationValues IVS)
{
	//If we already have a TimeManagerActor do not spawn another one
	//Just store it as the current TimeManagerActor for other plugins to use
	//Only pick the first instance, still need to code in single instance enforcement
	
	for (TActorIterator<ATimeManager> ActorItr(World); ActorItr; ++ActorItr)
	{
		//TimeManagerActor = *ActorItr;
		return;
	}
	FVector location = FVector(0,0,0);
	FRotator rotate = FRotator(0,0,0);
	FActorSpawnParameters SpawnInfo;
	TimeManagerActor = World->SpawnActor<ATimeManager>(ATimeManager::StaticClass(), location, rotate, SpawnInfo);
	//TimeManagerActor = NewObject TimeManagerActor;
	int32 Year, Month, Day, DayOfWeek;
	int32 Hour, Minute, Second, Millisecond;

	FPlatformTime::SystemTime(Year, Month, DayOfWeek, Day, Hour, Minute, Second, Millisecond);
	FTimeDateStruct Time = FTimeDateStruct(Year, Month, Day, Hour, Minute, Second, Millisecond);
	TimeManagerActor->InitializeCalendar(Time,0,false,0,0);
	TimeManagerActor->IncrementTime(0);
}

	
IMPLEMENT_MODULE(FTimePlugin, TimePlugin)


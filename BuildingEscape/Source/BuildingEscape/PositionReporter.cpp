// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReporter.h"
#include "BuildingEscape.h"


// Sets default values for this component's properties
UPositionReporter::UPositionReporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReporter::BeginPlay()
{
	Super::BeginPlay();

	// Creo una FString per contenere il nome dell'oggetto
	FString ObjName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Riporto la posizione dell'oggetto %s "), *ObjName);
	
	
}


// Called every frame
void UPositionReporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


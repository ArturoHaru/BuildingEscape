// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

}

void UOpenDoor::OpenDoor()
{

	if (!Owner) { return; }
	//Set rotation
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 00.f));
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate"), *(GetOwner()->GetName()));
	}

}

void UOpenDoor::CloseDoor()
{
	if (!Owner) { return; }
	//Set rotation
	Owner->SetActorRotation(FRotator(0.f, -90.f, 00.f));

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 50.f) //TODO make this parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate(){

	float totalmass = 0.f;
	TArray < AActor* > OverlappingActors;
	if (!PressurePlate) { return totalmass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Somma la massa
	for (auto& Oggetto : OverlappingActors) {

		totalmass += Oggetto->FindComponentByClass<UPrimitiveComponent>()->GetMass();

	}


	return totalmass;
}
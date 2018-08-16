// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString ObjName = GetOwner()->GetName();
	FString ObjPos = GetOwner()->GetActorTransform().GetLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s si trova a %s "), *ObjName, *ObjPos);
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ottieni la direzione in cui guarda il giocatore



	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);


	/*if (PlayerViewPointLocation != PreviousLocation || PlayerViewPointRotation != PreviousRotation) {
		UE_LOG(LogTemp, Warning, TEXT("Location: %s Position: %s"),
			*PlayerViewPointLocation.ToString(),
			*PlayerViewPointRotation.ToString()
		)
			PreviousLocation = PlayerViewPointLocation;
		    PreviousRotation = PlayerViewPointRotation;
	} */

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;


	// Spara un raggio dagli occhi e vedi cosa colpisce

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255,0,0),
		false,
		0.f,
		0.f,
		9.f
	);
}


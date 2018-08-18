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

	FindPhysicsHandleComponent();
	SetupInputComponent();

	
}

///Look for attached physics handle (only appears at runtime)
void UGrabber::FindPhysicsHandleComponent() {


	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (InputComponent) {

		UE_LOG(LogTemp, Warning, TEXT("Physics component Found"));

	}
	else {

		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));

	}


}

///Look for attached input component (only appears at runtime)
void UGrabber::SetupInputComponent() {


	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {

		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"));
		//Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else {

		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));

	}

}



void UGrabber::Grab() {

	// Ottieni la direzione in cui guarda il giocatore
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);
	

	//LINE TRACE and see if we reach any actor with physics body collision set
	auto HitResult = GetFirsTPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	///If we hit something then attach a physics handle

	if (ActorHit) {

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			InBoneNome,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			PlayerViewPointRotation
		);
	}

}

void UGrabber::Release() {

	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	//Release Physics handle
	PhysicsHandle->ReleaseComponent();
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
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	

	//If the physics handle is attached
	if (PhysicsHandle->GetGrabbedComponent()) {
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}

}

const FHitResult UGrabber::GetFirsTPhysicsBodyInReach()
{
	// Ottieni la direzione in cui guarda il giocatore
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);


	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

	///Setup parametri query
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());


	/// Spara un raggio dagli occhi e vedi cosa colpisce (Ray-cast)

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	///Guarda cosa colpisci
	AActor * ActorHit = Hit.GetActor();

	if (ActorHit) {

		UE_LOG(LogTemp, Warning, TEXT("Sto guardando: %s"), *(ActorHit->GetName()))

	}

	return Hit;
}
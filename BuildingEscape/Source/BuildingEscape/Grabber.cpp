// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
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
	if (InputComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()));
	}
}

///Look for attached input component (only appears at runtime)
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab() {

		//LINE TRACE and see if we reach any actor with physics body collision set
	auto HitResult = GetFirsTPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();



	if (!PhysicsHandle) { return;  }
	if (ActorHit) {

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			InBoneNome, //No bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			GetPlayerRotation()
		);
	}

}

void UGrabber::Release() {
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
		//If the physics handle is attached
	if (PhysicsHandle->GetGrabbedComponent()) {
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(ReachLineEnd());
		}

}

const FHitResult UGrabber::GetFirsTPhysicsBodyInReach()
{
		///Setup parametri query
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Spara un raggio dagli occhi e vedi cosa colpisce (Ray-cast)
		GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		ReachLineStart(),
		ReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}

FVector UGrabber::ReachLineStart(){
// Ottieni la direzione in cui guarda il giocatore
GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	OUT	PlayerViewPointLocation,
	OUT	PlayerViewPointRotation
);
return PlayerViewPointLocation;
}

FVector UGrabber::ReachLineEnd() {

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}
	///Ottieni la direzione in cui guarda il giocatore
FRotator UGrabber::GetPlayerRotation(){

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT	PlayerViewPointLocation,
			OUT	PlayerViewPointRotation
		);

		return PlayerViewPointRotation;

	
}


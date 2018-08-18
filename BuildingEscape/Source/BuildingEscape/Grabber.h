// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector PlayerViewPointLocation, PreviousLocation;
	FRotator PlayerViewPointRotation, PreviousRotation;
	

	FHitResult Hit;
		
	
private:
	///How far player can reach
	float Reach = 100.f;

	UPhysicsHandleComponent * PhysicsHandle = nullptr;
	UInputComponent * InputComponent = nullptr;
	FName InBoneNome;

	///Ray-Cast & Grabbing
	void Grab();
	
	///Call when grab key is released
	void Release();
	
	///Find (assumed) attached Physics handle
	void FindPhysicsHandleComponent();
	
	///Setup (assumed) input component
	void SetupInputComponent();

	///Returns current end of reach lines
	FVector ReachLineEnd();

	///Returns current start of reach lines
	FVector ReachLineStart();

	///Returns current player rotation
	FRotator GetPlayerRotation();

	//Return hit for first physic body in reach
	const FHitResult GetFirsTPhysicsBodyInReach();
};

// Copyright Jorge Luque 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//#Debug: Draws a green line to visualize the grab reach raycast
	//DrawDebugLineTrace();

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) // If the physics handle is attached
	{
		// Move the grabbed object
		PhysicsHandle->SetTargetLocation(GetGrabReachEndpoint());
	}
}

void UGrabber::FindPhysicsHandle() // Find physics handle component attached to owner
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing a physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	//UE_LOG(LogTemp, Warning, TEXT("grab key pressed"));

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (ComponentToGrab) // If a component is hit, attach the physics handle. Avoids trying to grab the component of a null component pointer
	{
		// #Debug
		UE_LOG(LogTemp, Warning, TEXT("(component) %s of (actor) %s was hit"), *HitResult.GetComponent()->GetName(), *HitResult.GetActor()->GetName());

		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			GetGrabReachEndpoint()
		);
	}
}

void UGrabber::GrabRelease()
{
	//UE_LOG(LogTemp, Warning, TEXT("grab key released"));
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::SetupInputComponent()
{
	// Find the Input component from Actor.h
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabRelease);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetGrabReachEndpoint(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	//#Debug: Log what the LineTrace hits
	//AActor* ActorHit = Hit.GetActor();
	//if (ActorHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *ActorHit->GetName());
	//}

	return Hit;
}

void UGrabber::DrawDebugLineTrace() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * GrabReachDistance);

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);

	// #Debug: log normalized rotation vector and endpoint
	//UE_LOG(LogTemp, Warning, TEXT(" \nViewPoint Location: %s \nViewPoint Rotation: %s"
	//	"\nRotationVector: %s \nLineTraceEnd: %s"),
	//	*PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString(),
	//	*PlayerViewPointRotation.Vector().ToString(), *LineTraceEnd.ToString()
	//);
}

FVector UGrabber::GetPlayerWorldPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetGrabReachEndpoint() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * GrabReachDistance);
}


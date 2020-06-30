// Copyright Jorge Luque 2020 All Rights Reserved


#include "OpenDoor.h"
#include "Engine/World.h" 
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component but no pressure plate is set!"), *GetOwner()->GetName());
	}

	// Set the actor that will trigger the opening of the door
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if PressurePlate is null before trying to dereference it to avoid crash.
	// Trying to dereference a null pointer will likely result in a crash.
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}

}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// Debug Logs:
	// UE_LOG(LogTemp, Warning, TEXT("Current Rotation: %s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Current Rotation.Yaw: %f"), GetOwner()->GetActorRotation().Yaw);
	// UE_LOG(LogTemp, Warning, TEXT("CurrentYaw: %f"), CurrentYaw);
	// UE_LOG(LogTemp, Warning, TEXT("TargetYaw: %f"), TargetYaw);
	// UE_LOG(LogTemp, Warning, TEXT("Lerp: %f"), CurrentYaw);

	// The way instructor did it:
	// CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.01f);
	// FRotator DoorRotation = GetOwner()->GetActorRotation();
	// DoorRotation.Yaw = CurrentYaw;
	// UE_LOG(LogTemp, Warning, TEXT("DoorRotation: %s"), *DoorRotation.ToString());
	// GetOwner()->SetActorRotation(DoorRotation);

	// The way I did it:
	// Without GetDenormalized() the door keeps spinning because GetActorRotation()
	// returns a normalized rotation that ranges form -180 to 180. When an actor's rotation goes above 180 degrees, 
	// such as 181, instead of returning 181 it returns -180
	CurrentYaw = GetOwner()->GetActorRotation().GetDenormalized().Yaw;
	FRotator DoorRotation(0.f, FMath::Lerp(CurrentYaw, TargetYaw, 1.f * DeltaTime), 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().GetDenormalized().Yaw;
	FRotator DoorRotation(0.f, FMath::Lerp(CurrentYaw, InitialYaw, 3.f * DeltaTime), 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}


// Copyright Jorge Luque 2020 All Rights Reserved

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h" 
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

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
	OpenAngle += InitialYaw;

	CheckIfPressurePlateIsSet();
	FindAudioComponent();

	// Set the actor that will trigger the opening of the door
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::CheckIfPressurePlateIsSet()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component but no pressure plate is set!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing audio component."), *GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has audio component."), *GetOwner()->GetName());
	}

}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if PressurePlate is null before trying to dereference it to avoid crash.
	// Trying to dereference a null pointer will likely result in a crash.
	//if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens)) // Old way of opening door when a specific actor steps on the pressure plate
	if (PressurePlate && TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();

		if (/*!AudioComponent->IsPlaying() &&*/ !DoorIsOpenOrOpening) // We don't really need to wait until the sound is not playing to play it again since it only plays once upon door movement state change
		{
			AudioComponent->Play();
			DoorIsOpenOrOpening = true;
		}
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);

			if (/*AudioComponent->IsPlaying() || */ !DoorIsOpenOrOpening) { return; }

			if(CurrentYaw < InitialYaw + 10.f) //Play sound only when door is almost closed. 10.f is an offset since the door's yaw slows down towards the end due to using exponential lerping
			{
				AudioComponent->Play();
				DoorIsOpenOrOpening = false;
			}
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// The way instructor did it:
	// CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.01f);
	// FRotator DoorRotation = GetOwner()->GetActorRotation();
	// DoorRotation.Yaw = CurrentYaw;
	// GetOwner()->SetActorRotation(DoorRotation);

	// The way I did it:
	// Without GetDenormalized() the door keeps spinning because GetActorRotation()
	// returns a normalized rotation that ranges form -180 to 180. When an actor's rotation goes above 180 degrees, 
	// such as 181, instead of returning 181 it returns -180
	CurrentYaw = GetOwner()->GetActorRotation().GetDenormalized().Yaw;
	//UE_LOG(LogTemp, Warning, TEXT("Current Door Yaw: %f"), CurrentYaw);
	FRotator DoorRotation(0.f, FMath::Lerp(CurrentYaw, OpenAngle, DoorOpenSpeed * DeltaTime), 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = GetOwner()->GetActorRotation().GetDenormalized().Yaw;
	//UE_LOG(LogTemp, Warning, TEXT("Current Door Yaw: %f"), CurrentYaw);
	FRotator DoorRotation(0.f, FMath::Lerp(CurrentYaw, InitialYaw, DoorCloseSpeed * DeltaTime), 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up all their masses
	for (AActor* ActorPointer : OverlappingActors)
	{
		TotalMass += ActorPointer->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		// #Debug: Log the mass of what's on the pressure plate
		//UE_LOG(LogTemp, Warning, TEXT("%s from %s has a mass of %f kg"),
		//	*ActorPointer->FindComponentByClass<UPrimitiveComponent>()->GetName(),
		//	*ActorPointer->GetName(),
		//	ActorPointer->FindComponentByClass<UPrimitiveComponent>()->GetMass());
	}
	//UE_LOG(LogTemp, Warning, TEXT("Total Mass: %f"), TotalMass);
	return TotalMass;
}


// Copyright Jorge Luque 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	float myFloat = 90.f;
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = 90.f;
	FRotator OpenDoorRotationValue = { 0.f, 100.f, 0.f };
	GetOwner()->SetActorRotation(CurrentRotation);
}


void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


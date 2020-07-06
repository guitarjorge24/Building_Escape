// Copyright Jorge Luque 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float GrabReachDistance = 200.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void FindPhysicsHandle();
	void Grab();
	void GrabRelease();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInReach();
	void DrawDebugLineTrace() const;

	// Get player position in world
	FVector GetPlayerWorldPosition() const;

	// Return the end point of the LineTrace used to grab objects
	FVector GetGrabReachEndpoint() const;
};

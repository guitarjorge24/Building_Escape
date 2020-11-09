// Copyright Jorge Luque 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinArea.generated.h"

UCLASS()
class BUILDING_ESCAPE_API AWinArea : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWinArea();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxColliderComponent = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ActorToCheckOverlapFor = nullptr;

	UFUNCTION()
	void OnComponentBeginOverlap_Box(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

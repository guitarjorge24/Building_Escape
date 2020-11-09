// Copyright Jorge Luque 2020


#include "WinArea.h"
#include "Components/BoxComponent.h"


// Sets default values
AWinArea::AWinArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
}

// Called when the game starts or when spawned
void AWinArea::BeginPlay()
{
	Super::BeginPlay();

	FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AWinArea::OnComponentBeginOverlap_Box);
}

// Called every frame
void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWinArea::OnComponentBeginOverlap_Box(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlap started"));

	if (ActorToCheckOverlapFor && OtherActor == ActorToCheckOverlapFor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Yes! You did it! You managed to steal the golden gargoyle!"));
	}

}


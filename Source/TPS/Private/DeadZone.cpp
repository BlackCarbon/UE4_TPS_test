// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadZone.h"
#include "Components/BoxComponent.h"
#include "TPSCharacter.h"
// Sets default values
ADeadZone::ADeadZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	deadZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DeadZone"));
	deadZone->SetBoxExtent(boxExtent);
}

// Called when the game starts or when spawned
void ADeadZone::BeginPlay()
{
	Super::BeginPlay();
	deadZone->SetCollisionResponseToAllChannels(ECR_Overlap);
	deadZone->OnComponentBeginOverlap.AddDynamic(this, &ADeadZone::OnOverlapBegin);
	this->SetActorLocation(originalPosition);
}
void ADeadZone::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ATPSCharacter* player = Cast<ATPSCharacter>(OtherActor);
	if (player) {
		FDamageEvent t;
		player->TakeDamage(damage, t, nullptr, nullptr);
	}
	else {
		OtherActor->Destroy();
	}
}

// Called every frame
void ADeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


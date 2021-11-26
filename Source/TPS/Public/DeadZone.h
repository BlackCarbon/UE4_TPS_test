// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZone.generated.h"

class  UBoxComponent;

UCLASS()
class TPS_API ADeadZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeadZone();

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	int32 damage=1000;

	UPROPERTY(EditDefaultsOnly,Category="Original Position")
	FVector originalPosition=FVector(0,0,-200);

	UPROPERTY(EditDefaultsOnly, Category = "DeadZone Box Extent")
		FVector boxExtent = FVector(1e9, 1e9, 1);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UBoxComponent* deadZone;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

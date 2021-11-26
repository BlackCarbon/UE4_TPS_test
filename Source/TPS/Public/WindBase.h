// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StoneBase.h"
#include "WindBase.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AWindBase : public AStoneBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Wind`s Strength")
	float strength = 2000;
protected:
	UFUNCTION()
		virtual void OnOverlapBegin(class UPrimitiveComponent* ThisComp,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	virtual void BeginPlay() override;
};

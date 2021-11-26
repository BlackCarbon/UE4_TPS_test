// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindBase.h"
#include "WaterBase.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AWaterBase : public AWindBase
{
	GENERATED_BODY()
protected:
		virtual void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

};

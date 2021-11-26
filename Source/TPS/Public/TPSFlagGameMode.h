// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSTDMGameMode.h"
#include "TPSFlagGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API ATPSFlagGameMode : public ATPSTDMGameMode
{
	GENERATED_BODY()
	
public:
	void HandleOnActorKilled(AActor* VictimActor, AActor* KillerActor, AController* KillerController) override;

protected:

	void Tick(float DeltaSeconds) override;

	void CheckAllFlag();

	//n个人时的速度
	float FlagSpeed(int n);

};

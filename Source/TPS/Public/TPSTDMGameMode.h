// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameState.h"
#include "TPSTDMGameMode.generated.h"
#define FIRE 0
#define WATER 1


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);



/**
 * 
 */
UCLASS()
class TPS_API ATPSTDMGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	
	void SetGameStatus(EGameStatus NewStatus);

	EGameStatus QueryGameStatus();

	void AssignNewTeamId();

	void PreparingForGame();

	void StartGame();

	void EndGame();

	void GameOver();

	void KillAllPlayer();

	//µ√∑÷«Â¡„
	void ClearScore();

	void Tick(float DeltaSeconds) override;



public:

	ATPSTDMGameMode();

	void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		float QueryTeamScore(int team);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void SetTeamScore(int team, float count);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
		void AddTeamScore(int team, float count);

	void RespawnPlayer(AController * Controller, float InTime);

	void RespawnPlayer(AController * Controller);

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled OnActorKilled;

	UFUNCTION()
		virtual void HandleOnActorKilled(AActor* VictimActor, AActor* KillerActor, AController* KillerController);


};

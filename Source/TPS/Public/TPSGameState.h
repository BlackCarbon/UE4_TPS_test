// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/Controller.h"
#include "Chaos/Pair.h"
#include "TPSGameState.generated.h"


UENUM(BlueprintType)
enum class EGameStatus : uint8
{
	Idle,
	PreparingGame,
	InGame,
	GameFinished
};

USTRUCT()
struct FIntIntPair
{
	GENERATED_BODY()

	UPROPERTY()
		int32 x;
	UPROPERTY()
		int32 y;
};

USTRUCT()
struct FAcontrollerIntPair
{
	GENERATED_BODY()

	UPROPERTY()
		AController* x;
	UPROPERTY()
		int32 y;
};

/**
 *
 */
UCLASS()
class TPS_API ATPSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ATPSGameState();

// 	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState")
// 		TMap<AController*,int> playerList;
	//����б�
	UPROPERTY(Replicated)
		TArray<FAcontrollerIntPair> playerList;

// 	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState")
// 		TMap<int,int> TeamStates;
	//�����Ŷ�����Ϣ
	UPROPERTY(Replicated)
		TArray<FIntIntPair> TeamStates;


	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
		int TeamAScore;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
		int TeamBScore;


	//���ظ���ҵĶ��飬δע����򷵻�0
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int GetTeamState(int playerIndex);

	//���ظ���ҵĶ��飬δע����򷵻�0
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int GetTeamStateByActor(AActor *player);

	//���ظ���ҵĶ��飬δע����򷵻�0
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int GetTeamStateByController(AController *player);

	//���ظö���ĳ�Ա��
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int TeamCount(int team);


	void AddNewPlayer(AController* player,int playerId);

	//�Ծ�״̬
	UPROPERTY(ReplicatedUsing=OnRep_GameStatus , BlueprintReadOnly, Category = "GameState")
		EGameStatus GameStatus;

	bool DoesPlayerAlreadyExist(AController *player);


	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void GetWidget(EGameStatus NewStatus, EGameStatus OldStatus);


protected:

	UFUNCTION()
		void OnRep_GameStatus(EGameStatus OldStatus);

	UFUNCTION()
		void OnGameStatusChanged(EGameStatus NewStatus, EGameStatus OldStatus)
	{
		GetWidget(NewStatus, OldStatus);

	}

};

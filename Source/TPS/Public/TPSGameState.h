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
	//玩家列表
	UPROPERTY(Replicated)
		TArray<FAcontrollerIntPair> playerList;

// 	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState")
// 		TMap<int,int> TeamStates;
	//储存着队伍信息
	UPROPERTY(Replicated)
		TArray<FIntIntPair> TeamStates;


	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
		int TeamAScore;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "GameState")
		int TeamBScore;


	//返回该玩家的队伍，未注册的则返回0
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int GetTeamState(int playerIndex);

	//返回该玩家的队伍，未注册的则返回0
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int GetTeamStateByActor(AActor *player);

	//返回该玩家的队伍，未注册的则返回0
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int GetTeamStateByController(AController *player);

	//返回该队伍的成员数
	UFUNCTION(BlueprintCallable, Category = "GameState")
		int TeamCount(int team);


	void AddNewPlayer(AController* player,int playerId);

	//对局状态
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

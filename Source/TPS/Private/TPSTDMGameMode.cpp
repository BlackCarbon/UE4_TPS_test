// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSTDMGameMode.h"
#include "TPSPlayerState.h"
#include "TPSGameState.h"
#include "TPSCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TPSPlayerController.h"
#include "StoneBase.h"
#include "GameFramework/PlayerStart.h"
// #define  FOX_DEBUG




ATPSTDMGameMode::ATPSTDMGameMode()
{
	PlayerStateClass = ATPSPlayerState::StaticClass();
	GameStateClass = ATPSGameState::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.15f;

}


void ATPSTDMGameMode::SetGameStatus(EGameStatus NewStatus)
{
	ATPSGameState *GS = GetGameState<ATPSGameState>();
	if (ensureAlways(GS))
	{
		GS->GameStatus = NewStatus;
	}
}




EGameStatus ATPSTDMGameMode::QueryGameStatus()
{
	ATPSGameState *GS = GetGameState<ATPSGameState>();
	if (ensureAlways(GS))
	{
		return GS->GameStatus;
	}
	UE_LOG(LogTemp, Warning, TEXT("GameState doesn't exist!"));
	return EGameStatus::Idle;
}

void ATPSTDMGameMode::StartPlay()
{
	Super::StartPlay();
	//准备游戏
	PreparingForGame();

	OnActorKilled.AddDynamic(this, &ATPSTDMGameMode::HandleOnActorKilled);
}


void ATPSTDMGameMode::RespawnPlayer(AController * Controller)
{
// 	UE_LOG(LogTemp, Log, TEXT("What the hell ?"));
	//同一个世界，同一个梦想。
	if (Controller && Controller->GetWorld() == GetWorld() && Controller->GetPawn() == nullptr)
	{
		auto GS = GetGameState<ATPSGameState>();
		if (ensureAlways(GS))
		{
			int TeamID = GS->GetTeamStateByController(Controller);
			AActor *pos;
			if (TeamID)
				pos = FindPlayerStart(Controller, FString("1"));
			else
				pos = FindPlayerStart(Controller, FString("0"));
			auto playerStart = Cast<APlayerStart>(pos);
			playerStart->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			RestartPlayerAtPlayerStart(Controller, playerStart);
			UE_LOG(LogTemp, Log, TEXT("Respawn successfully!"));
		}
	}
}

void ATPSTDMGameMode::RespawnPlayer(AController * Controller, float InTime)
{
// 	UE_LOG(LogTemp, Log, TEXT("Truly Respawning !"));
// 	if (Controller)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Real Controller !"));
// 	}
	FTimerHandle uselessHandler;
	FTimerDelegate func = FTimerDelegate::CreateLambda([=]() { RespawnPlayer(Controller); });
	GetWorldTimerManager().SetTimer(uselessHandler, func, InTime, false);
}





void ATPSTDMGameMode::HandleOnActorKilled(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	//摧毁岩石加1分，杀死地方玩家加3分
	auto world = GetWorld();
	ATPSTDMGameMode *GM = nullptr;
	ATPSGameState *GS = nullptr;
	if (ensureAlways(world))
	{
		GM = world->GetAuthGameMode<ATPSTDMGameMode>();
		GS = world->GetGameState<ATPSGameState>();
		ensureAlways(GM);
		ensureAlways(GS);
	}
	{
		auto stone = Cast<AStoneBase>(VictimActor);
		if (stone && KillerController)
		{
			auto PS = KillerController->GetPlayerState<ATPSPlayerState>();
			if (PS)
			{
				PS->IncreaseScore(1.0f);
			}
		}
	}
	{
		auto hostilePlayer = Cast<ATPSCharacter>(VictimActor);
		if (hostilePlayer && KillerController)
		{
			auto PS = KillerController->GetPlayerState<ATPSPlayerState>();
			if (PS)
			{
				//个人得分
				PS->IncreaseScore(3.0f);
				//团队得分
				int team = GS->GetTeamStateByController(KillerController);
				GM->AddTeamScore(team, 1.0f);
			}
		}

	}

}

void ATPSTDMGameMode::AssignNewTeamId()
{
	auto World = GetWorld();
	auto GS = GetGameState<ATPSGameState>();

#ifdef FOX_DEBUG
	if (World && GS)
	{
		int cnt = 0; 
		TArray<APlayerController*> t;
		for (auto It = World->GetPlayerControllerIterator(); It; ++It)
		{
			t.AddUnique(It->Get());
			cnt++;
		}
		t.Sort();
		
// 		UE_LOG(LogTemp, Log, TEXT("PC cnt is : %d , unique cnt is : %d"), cnt, t.Num());
	}


#endif // FOX_DEBUG



	if (World && GS)
	{
		for (auto It = World->GetPlayerControllerIterator(); It; ++It)
		{
			auto PC = It->Get();
			auto PS = PC->GetPlayerState<ATPSPlayerState>();
			if (PS)
			{
				GS->AddNewPlayer(PC, PS->GetPlayerId());
			}
		}
	}
		

}

void ATPSTDMGameMode::PreparingForGame()
{
	//游戏只准备一次
	if (QueryGameStatus() == EGameStatus::PreparingGame)
	{
		return;
	}
	SetGameStatus(EGameStatus::PreparingGame);

	//将设置一个不受管理的Handler，将在45s后开始游戏
	
	FTimerHandle uselessHandle;
	GetWorldTimerManager().SetTimer(uselessHandle, this, &ATPSTDMGameMode::StartGame, 45.0f, false);

}




void ATPSTDMGameMode::StartGame()
{
	//游戏只开始一次
	if (QueryGameStatus() == EGameStatus::InGame)
	{
		return;
	}
	SetGameStatus(EGameStatus::InGame);
	ClearScore();
	KillAllPlayer();
}

void ATPSTDMGameMode::EndGame()
{
	//游戏只结束一次
	if (QueryGameStatus() == EGameStatus::GameFinished)
	{
		return;
	}
	SetGameStatus(EGameStatus::GameFinished);
	GameOver();
}

void ATPSTDMGameMode::GameOver()
{
	auto world = GetWorld();
	if (ensureAlways(world))
	{
		for (auto It = world->GetPlayerControllerIterator(); It; ++It)
		{
			auto PC = It->Get();
			if(ensure(PC))
			{
				auto character = Cast<ATPSCharacter>(PC->GetCharacter());
				if (ensure(character))
				{
					//停止移动，解除控制器
					character->GetMovementComponent()->StopMovementImmediately();
					character->DetachFromControllerPendingDestroy();
				}
			}
		}
	}

	// @TODO 所有玩家将不可操控
}

void ATPSTDMGameMode::KillAllPlayer()
{
	if (ensureAlways(GetWorld()))
	{
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			auto PC = It->Get();
			if (PC)
			{
				auto character = PC->GetCharacter();
				FDamageEvent t;
				if (character)
				{
					character->TakeDamage(1000.f, t, nullptr, nullptr);
				}
			}
		}
	}
}

void ATPSTDMGameMode::ClearScore()
{
	auto world = GetWorld();
	if (ensureAlways(world))
	{
		for (auto It = world->GetPlayerControllerIterator(); It; ++It)
		{
			auto PC = It->Get();
			if (PC)
			{
				auto PS = PC->GetPlayerState<ATPSPlayerState>();
				PS->SetScore(0);
			}
		}
		auto GM = world->GetAuthGameMode<ATPSTDMGameMode>();
		if (ensureAlways(GM))
		{
			GM->SetTeamScore(0, 0);
			GM->SetTeamScore(1, 0);
		}
	}
}

float ATPSTDMGameMode::QueryTeamScore(int team)
{
	auto world = GetWorld();
	if (ensureAlways(world))
	{
		auto GS = world->GetGameState<ATPSGameState>();
		if (ensure(GS))
		{
			if (team == 0)
			{
				return GS->TeamAScore;
			}
			else
			{
				return GS->TeamBScore;
			}
		}
	}
	return 0;
}

void ATPSTDMGameMode::SetTeamScore(int team, float count)
{
	auto world = GetWorld();
	if (ensureAlways(world))
	{
		auto GS = world->GetGameState<ATPSGameState>();
		if (ensure(GS))
		{
			if (team == 0)
			{
				GS->TeamAScore = count;
			}
			else
			{
				GS->TeamBScore = count;
			}
			float eps = 1e-4;
			//有队伍获胜
			if (GS->TeamAScore >= 100.0f - eps || GS->TeamBScore >= 100.0f - eps)
			{
				EndGame();
			}
		}
	}
}

void ATPSTDMGameMode::AddTeamScore(int team, float count)
{
	SetTeamScore(team, QueryTeamScore(team) + count);
}

void ATPSTDMGameMode::Tick(float DeltaSeconds)
{
	AssignNewTeamId();
}





// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameState.h"
#include "Net\Unrealnetwork.h"
#include <ctime>
#include <utility>
#include <TPSCharacter.h>
#include "TPSPlayerController.h"
// #define FOX_DEBUG

ATPSGameState::ATPSGameState()
{
	if (HasAuthority())
	{
		srand(time(nullptr));
	}
	playerList.Empty();
	GameStatus = EGameStatus::Idle;
	TeamAScore = TeamBScore = 0;
	TeamStates.Empty();
}





int ATPSGameState::GetTeamState(int playerIndex)
{
	for (int i = 0; i < TeamStates.Num(); i++)
	{
		if (TeamStates[i].x == playerIndex)
		{
			return TeamStates[i].y;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Doesn't exist playerIndex : %d"), playerIndex);
	return 0;
}


int ATPSGameState::GetTeamStateByController(AController *player)
{
	for (int i = 0; i < playerList.Num(); i++)
	{
		if (playerList[i].x == player)
		{
			return GetTeamState(playerList[i].y);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Doesn't exist such player"));
	return 0;
}

int ATPSGameState::GetTeamStateByActor(AActor * player)
{
	auto character = Cast<ATPSCharacter>(player);
	if (ensureAlways(character))
	{
		return GetTeamStateByController(character->GetController());
	}
	UE_LOG(LogTemp, Warning, TEXT("Doesn't exist such player"));
	return 0;
}




int ATPSGameState::TeamCount(int team)
{
	int cnt = 0;
	for (auto it : TeamStates)
	{
		if (it.y == team)
		{
			cnt++;
		}
	}
	return cnt;
}


void ATPSGameState::AddNewPlayer(AController* player,int playerId)
{
#ifdef FOX_DEBUG
	if (playerList.Num() != 3)
		UE_LOG(LogTemp, Log, TEXT("Now player count is %d"), playerList.Num());

#endif // FOX_DEBUG

	//����Ѿ����ڣ����ظ����
	for (int i = 0; i < playerList.Num(); i++)
	{
		if (playerList[i].x == player)
		{
			playerList[i].y = playerId;
			return;
		}
	}
	int zerocnt = TeamCount(0);
	int onecnt = TeamCount(1);
	int newTeamID = 0;
	if (onecnt == zerocnt)
	{
		newTeamID = rand() & 1;
	}
	else
	{
		newTeamID = onecnt < zerocnt;
	}
	{
		FAcontrollerIntPair newPlayer;
		newPlayer.x = player;
		newPlayer.y = playerId;
		playerList.Emplace(newPlayer);
	}
	{
		FIntIntPair newTeamStatus;
		newTeamStatus.x = playerId;
		newTeamStatus.y = newTeamID;
		TeamStates.Emplace(newTeamStatus);
	}
	{
		auto PC = Cast<ATPSPlayerController>(player);
		if (PC)
		{
			PC->TeamID = newTeamID;
		}
	}
}

bool ATPSGameState::DoesPlayerAlreadyExist(AController *player)
{
	for (int i = 0; i < playerList.Num(); i++)
	{
		if (playerList[i].x == player)
		{
			return true;
		}
	}
	return false;
}

void ATPSGameState::OnRep_GameStatus(EGameStatus OldStatus)
{
	OnGameStatusChanged(GameStatus, OldStatus);
}



void ATPSGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSGameState, GameStatus);
	DOREPLIFETIME(ATPSGameState, TeamAScore);
	DOREPLIFETIME(ATPSGameState, TeamBScore);
	DOREPLIFETIME(ATPSGameState, playerList);
	DOREPLIFETIME(ATPSGameState, TeamStates);
}

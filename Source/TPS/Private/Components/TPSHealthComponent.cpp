// Fill out your copyright notice in the Description page of Project Settings.


#include "Components\TPSHealthComponent.h"
#include "Net\UnrealNetwork.h"
#include "TPSTDMGameMode.h"
#include <TPSCharacter.h>
#include <TPSPlayerState.h>
#include <TPSGameState.h>
#include "TPSFlagGameMode.h"
#define FOX_DEBUG
// #define GAMEMODE_TDM
#define GAMEMODE_FLAG

// Sets default values for this component's properties
UTPSHealthComponent::UTPSHealthComponent()
{
	MaxHealth = 100.0f;

	bIsDied = false;

	SetIsReplicated(true);
}


// Called when the game starts
void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor * MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UTPSHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = MaxHealth;

}

void UTPSHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDied)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	
	if (Health < 1.0f)
	{
		bIsDied = true;
	}

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

#ifdef FOX_DEBUG
	auto a1 = Cast<ATPSCharacter>(GetOwner());
	auto a2 = InstigatedBy;
	int ID1=-1, ID2=-1;
	int teamID1 = -1, teamID2 = -1;
	if (a1)
	{
		auto Cont = a1->GetController();
		if (Cont && Cont->GetPlayerState<ATPSPlayerState>())
		{
			ID1 = Cont->GetPlayerState<ATPSPlayerState>()->GetPlayerId();

		}
	}

	if (a2)
	{
		if (a2->GetPlayerState<ATPSPlayerState>())
		{
			ID2 = a2->GetPlayerState<ATPSPlayerState>()->GetPlayerId();
		}
	}
	auto world = GetWorld();
	ATPSGameState *GS = nullptr;
	float teamAscore = -1, teamBscore = -1;
	if (world)
	{
		GS = world->GetGameState<ATPSGameState>();
	}
	if (GS)
	{
		teamID1 = GS->GetTeamState(ID1);
		teamID2 = GS->GetTeamState(ID2);
		teamAscore = GS->TeamAScore;
		teamBscore = GS->TeamBScore;
	}

	UE_LOG(LogTemp, Log, TEXT("%d Hit %d!,(TEAM: %d , %d), scored(0:%f  1:%f)"), ID2, ID1, teamID2, teamID1, teamAscore, teamBscore);
#endif

#ifdef GAMEMODE_TDM

	if (bIsDied)
	{
		ATPSTDMGameMode* GM = Cast<ATPSTDMGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}

#endif
#ifdef GAMEMODE_FLAG
	if (bIsDied)
	{
		ATPSFlagGameMode* GM = Cast<ATPSFlagGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);
		}
	}

#endif

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);



}


void UTPSHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTPSHealthComponent, Health);
}
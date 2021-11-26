// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "Net\UnrealNetwork.h"


ATPSPlayerController::ATPSPlayerController()
{
	TeamID = 0;
}


void ATPSPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSPlayerController, TeamID);

}

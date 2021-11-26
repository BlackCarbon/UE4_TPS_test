// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerState.h"



void ATPSPlayerState::IncreaseScore(float DeltaScore)
{
	SetScore(GetScore() + DeltaScore);
}


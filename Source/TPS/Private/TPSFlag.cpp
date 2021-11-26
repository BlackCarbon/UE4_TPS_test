// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSFlag.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATPSFlag::ATPSFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	flag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FLAG"));

	flagteam = 0;

	flagcolor = 0;

	RootComponent = flag;

	flagcontroller.Empty();
}

// Called when the game starts or when spawned
void ATPSFlag::BeginPlay()
{
	Super::BeginPlay();
	
}

int ATPSFlag::getFlagColor()
{
	SetColor();
	return flagcolor;
}


void ATPSFlag::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSFlag, flagteam);
	DOREPLIFETIME(ATPSFlag, flagcontroller);
	DOREPLIFETIME(ATPSFlag, flagcolor);
}
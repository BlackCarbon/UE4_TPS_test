// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapLauncherLocal.generated.h"

class AStoneBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UMapLauncherLocal : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMapLauncherLocal();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TMap<FIntVector, AStoneBase*>StoneMap;

	UFUNCTION()
	AActor *CreateStone(const FString &BP_Name,const FIntVector &pos);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerCreateStone(const FString& BP_Name, const FIntVector& pos);

	void LocalCreateStone(const FString& BP_Name, const FIntVector& pos);

};

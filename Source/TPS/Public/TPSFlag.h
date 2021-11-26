// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "chaos/Array.h"
#include "TPSFlag.generated.h"



class UStaticMeshComponent;
class UBoxComponent;
class AController;

UCLASS(BlueprintType)
class TPS_API ATPSFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSFlag();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FLAG")
		UStaticMeshComponent* flag;
    
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "FLAG")
	   int flagteam;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "FLAG")
		TArray<AController*> flagcontroller;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "FLAG")
		int flagcolor;
public:	

	//查询旗子被占领的状态，1为water， 2为fire ,0为空闲状态
	UFUNCTION()
	int getFlagTeam()
	{
		return flagteam;
	}

	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
		void SetColor();

	//查询旗子的颜色，0为无，1为fire色，2为water色
	UFUNCTION()
		int getFlagColor();


	//设置旗子的速度，正为上升，负为下降
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
	void SetSpeed(float speed);

	//Fire旗子进行移动
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
	void FireFlag();

	//water旗子进行移动
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
	void WaterFlag();


	UFUNCTION()
	TArray<AController*> GetFlagController()
	{
		return flagcontroller;
	}

};

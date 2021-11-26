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

	//��ѯ���ӱ�ռ���״̬��1Ϊwater�� 2Ϊfire ,0Ϊ����״̬
	UFUNCTION()
	int getFlagTeam()
	{
		return flagteam;
	}

	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
		void SetColor();

	//��ѯ���ӵ���ɫ��0Ϊ�ޣ�1Ϊfireɫ��2Ϊwaterɫ
	UFUNCTION()
		int getFlagColor();


	//�������ӵ��ٶȣ���Ϊ��������Ϊ�½�
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
	void SetSpeed(float speed);

	//Fire���ӽ����ƶ�
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
	void FireFlag();

	//water���ӽ����ƶ�
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunc")
	void WaterFlag();


	UFUNCTION()
	TArray<AController*> GetFlagController()
	{
		return flagcontroller;
	}

};

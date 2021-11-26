// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"
class  USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
UCLASS()
class TPS_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Effect();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AActor> ProjectileClass;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		//TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	USkeletalMeshComponent* MeshComp;

	//销毁自己的事件
	FTimerHandle DestroySelf;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<UCameraShake> FireCamShake;

};

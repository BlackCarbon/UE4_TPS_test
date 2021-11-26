// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TPS_API ATPS_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPS_Projectile();
    
	//������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	uint8 ProjectileNum;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ������ײ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    USphereComponent* CollisionComponent;

	// �������˶����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
		class UParticleSystem* ExplosionEffect;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

   // Function that initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);

	// ��������������ʱ���õĺ�����
	/*UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	*/
    UFUNCTION(BlueprintCallable, Category = "Projectile")
		uint8 GetProjectileNum()
	{
		return ProjectileNum;
	}
};

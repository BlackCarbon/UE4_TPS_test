// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h" 
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TPSCharacter.h"
// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";

}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto owner = GetOwner();
	if (!IsValid(owner))
	{
		//不存在销毁事件
		if (!GetWorldTimerManager().IsTimerActive(DestroySelf))
		{
			FTimerDelegate func = FTimerDelegate::CreateLambda([=]() { Destroy(); });
			GetWorldTimerManager().SetTimer(DestroySelf, func, 3.0f, false);
		}

	}



}

void ASWeapon::Fire()
{

	AActor* MyOwner = GetOwner();
	auto character = Cast<ATPSCharacter>(MyOwner);

	if (MyOwner && ProjectileClass)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = character;
		SpawnParams.Instigator = character->GetInstigator();
		
		if (HasAuthority())
			GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);

		Effect();

		//Debug
		
/*		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		FCollisionQueryParams QueryParames;
		QueryParames.AddIgnoredActor(MyOwner);
		QueryParames.AddIgnoredActor(this);
		QueryParames.bTraceComplex = true;

		FHitResult Hit;

		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
			
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParames))
		{
			//AActor* HitActor = Hit.GetActor();
			//UGameplayStatics::ApplyPointDamage(HitActor,20.0f,Hit,MyOwner->GetInstigatorController(),this,DamageType);

		}
		if (MuzzleEffect)
		{
			//UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}
		
		
}*/

	}
}
void ASWeapon::Effect()
{
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}



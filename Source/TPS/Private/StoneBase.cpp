// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneBase.h"
#include "TPS_Projectile.h"
#include "MapLauncher.h"
#include "Components/TPSHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AStoneBase::AStoneBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("Static Mesh"));

	RootComponent = mesh;

	bDied = false;
	HealthComp = CreateDefaultSubobject<UTPSHealthComponent>(TEXT("HealthComp"));

	bReplicates = true;
	
	//SetReplicates(true);
	//this->SetRootComponent(mesh);
//	mesh->SetAttachParent(GetRootComponent());
	//	mesh->SetCollisionProfileName(TEXT("BlackAll"));
	//mesh->GetComponentToWorld().SetScale3D(FVector(50));
//	mesh=
}

// Called when the game starts or when spawned
void AStoneBase::BeginPlay()
{
	Super::BeginPlay();
	FVector scaler = this->GetActorScale3D();
	FVector ms = UMapLauncher::getInstance()->StoneScale;
	stoneScaler = ms;
	scaler.X *= ms.X;
	scaler.Y *= ms.Y;
	scaler.Z *= ms.Z;
//	UE_LOG(LogTemp, Log, TEXT("s=%f"), scaler.X);
	this->SetActorScale3D(scaler);

	mesh->OnComponentHit.AddDynamic(this, &AStoneBase::OnHit);

	HealthComp->OnHealthChanged.AddDynamic(this, &AStoneBase::OnMyHealthChanged);

}

// Called every frame
void AStoneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AStoneBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) 
{
	ATPS_Projectile *other = Cast<ATPS_Projectile>(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("hit stone"));
	if(other)
	{
		FVector loc = Hit.Location - GetTransform().GetLocation();
		
		/*
		float var = loc.Z / mesh->GetComponentTransform().GetScale3D().Z;
		if (abs(round(var) - var) < 1e-6) {
			Destroy();
		}*/
		if (loc.Z > 40*stoneScaler.Z) {//打在砖块顶面
			FIntVector np = position;
			np.Z += 1;
			if (other->GetActorLabel().StartsWith("BP_Fire")) {

				UMapLauncher::getInstance()->TryCreateStone("BP_Fire", np);
			}
			else if (other->GetActorLabel().StartsWith("BP_Witer")) {

				UMapLauncher::getInstance()->TryCreateStone("BP_Water", np);
			}
		}
		else {//打在砖块侧面
			
			
		}
		UE_LOG(LogTemp, Log, TEXT("z :%f"), loc.Z);
	}
}

void AStoneBase::OnMyHealthChanged(UTPSHealthComponent* MyHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health < 1.0f && !bDied)
	{
		bDied = true;
		UMapLauncher::getInstance()->TryCreateStone("", position);
	}
}

void AStoneBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStoneBase, bDied);
}

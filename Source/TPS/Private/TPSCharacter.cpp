// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TPS_Projectile.h"
#include "Components/TPSHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "TPSTDMGameMode.h"

class UUserWidget;

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	
	HealthComp = CreateDefaultSubobject<UTPSHealthComponent>(TEXT("HealthComp"));

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();



	HealthComp->OnHealthChanged.AddDynamic(this, &ATPSCharacter::OnMyHealthChanged);


}

void ATPSCharacter::MoveForward(float Val)
{
	AddMovementInput(GetActorForwardVector(), Val);

}

void ATPSCharacter::MoveRight(float Val)
{
	AddMovementInput(GetActorRightVector(), Val);
}

void ATPSCharacter::BeginCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Crouch!"));
	Crouch();
}

void ATPSCharacter::EndCrouch()
{
	UE_LOG(LogTemp, Warning, TEXT("End Crouch!"));
	UnCrouch();
}

void ATPSCharacter::OnMyHealthChanged(UTPSHealthComponent * MyHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	if (Health < 1.0f && !bDied)
	{
		
		bDied = true;

		if (HasAuthority())
		{
			UE_LOG(LogTemp, Log, TEXT("Respawning !"));
			ATPSTDMGameMode *GM = Cast<ATPSTDMGameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				UE_LOG(LogTemp, Log, TEXT("Really Respawning !"));
				GM->RespawnPlayer(GetController(), 5.0f);
			}
		}
// 		不在此处处理
// 		if (HasAuthority())
// 		{
// 			auto world = GetWorld();
// 			if (ensureAlways(world))
// 			{
// 				auto GM = world->GetAuthGameMode<ATPSTDMGameMode>();
// 				auto GS = world->GetGameState<ATPSGameState>();
// 				if (ensureAlways(GM) && ensureAlways(GS))
// 				{
// 					int Team1 = GS->GetTeamStateByController(InstigatedBy);
// 					int Team0 = GS->GetTeamStateByActor(this);
// 					if (ensure(Team0 != Team1))
// 					{
// 						GM->AddTeamScore(Team1, 1.0f);
// 					}
// 				}
// 			}
// 		}

		SetLifeSpan(5.0f);

		GetMovementComponent()->StopMovementImmediately();

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

	

		

	}
}


// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	if (!IsLocallyControlled())
// 	{
// 		FRotator NewRot = GetMesh()->GetRelativeRotation();
// 		NewRot.Pitch = RemoteViewPitch * 360.0f / 255.0f;
// 		GetMesh()->SetRelativeRotation(NewRot);
// 	}

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookDown", this, &ATPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ATPSCharacter::AddControllerYawInput);


	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ATPSCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ATPSCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSCharacter::Jump);

}

FVector ATPSCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
		return CameraComp->GetComponentLocation();
	return Super::GetPawnViewLocation();
}

void ATPSCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSCharacter, bDied);
}


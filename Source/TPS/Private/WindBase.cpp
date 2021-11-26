// Fill out your copyright notice in the Description page of Project Settings.


#include "WindBase.h"
#include "TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MapLauncher.h"
// Sets default values
void AWindBase::BeginPlay()
{
	FVector scaler = this->GetActorScale3D();
	FVector ms = UMapLauncher::getInstance()->StoneScale;
	scaler.X *= ms.X;
	scaler.Y *= ms.Y;
	scaler.Z *= ms.Z;
//	UE_LOG(LogTemp, Log, TEXT("s=%f"), scaler.X);
	this->SetActorScale3D(scaler);
//	mesh->OnComponentHit.
//	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	mesh->OnComponentBeginOverlap.AddDynamic(this, &AWindBase::OnOverlapBegin);
}
void AWindBase::OnOverlapBegin(class UPrimitiveComponent* ThisComp,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	ATPSCharacter *player = Cast<ATPSCharacter>(OtherActor);
	if (player) {
	//	OtherComp->
		//FVector v = OtherComp->GetPhysicsLinearVelocity();
		//v.Z = strength;
	//	OtherComp
		//OtherActor->componet
		//OtherComp->SetPhysicsLinearVelocity(v,true);
		//UE_LOG(LogTemp, Log, TEXT("vz :%f"), OtherComp->GetPhysicsLinearVelocity().Z);
		/*UCharacterMovementComponent* move=nullptr;
		OtherActor->set
		for (int i = 0;move==nullptr&&i < OtherActor->GetVelocity();i++) {
			move = Cast<UCharacterMovementComponent>(OtherActor->GetChildComponent(i));

		}
		if (move) {
			UE_LOG(LogTemp, Log, TEXT("launch"));
		}*/
	//	OtherComp->
		player->LaunchCharacter(FVector(0,0,1) * strength,false,true);
	}
}
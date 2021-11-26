// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLauncherLocal.h"
#include "MapLauncher.h"
#include "StoneBase.h"


// Sets default values for this component's properties
UMapLauncherLocal::UMapLauncherLocal()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMapLauncherLocal::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMapLauncherLocal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



AActor *UMapLauncherLocal::CreateStone(const FString& BP_Name, const FIntVector& pos) {
	if (BP_Name == "") 
	{
		AActor*x = StoneMap.FindRef(pos);
		if(x){
			UE_LOG(LogTemp, Log, TEXT("销毁"));
			x->Destroy();
			StoneMap.Remove(pos);
		}
		return nullptr;
	}
	if (StoneMap.FindRef(pos)) {
		return nullptr;
	}
	FString s = "Blueprint'/Game/Blueprints/";
	s += BP_Name + ".";
	s += BP_Name + "_C'";
	//	UE_LOG(LogTemp, Log, TEXT(&s[0]));

	UClass* BlueprintVar = StaticLoadClass(AActor::StaticClass(), nullptr, &s[0]);
	if (BlueprintVar != nullptr)
	{
		// 向场景中添加新生成的蓝图实例

		AActor* pMyActor =GetWorld()->SpawnActor<AActor>(BlueprintVar,UMapLauncher::getInstance()-> transFromDispersedToContinuous(pos), FRotator::ZeroRotator);
		if (pMyActor)
		{
			if (Cast<AStoneBase>(pMyActor)) {

				(Cast<AStoneBase>(pMyActor))->position = pos;
			}
			UE_LOG(LogTemp, Log, TEXT("!!!new obj z :%f"), pos.Z);
			if (BP_Name == "BP_Fire" || BP_Name == "BP_Water" || BP_Name == "BP_NStoneBase") {
				StoneMap.Add(TTuple<FIntVector, AStoneBase*>(pos, Cast<AStoneBase>(pMyActor)));
			}
			return pMyActor;
		}
	}
	return nullptr;
}




void UMapLauncherLocal::LocalCreateStone(const FString& BP_Name, const FIntVector& pos)
{
	AActor* x = CreateStone(BP_Name, pos);
}



void UMapLauncherLocal::ServerCreateStone_Implementation(const FString& BP_Name, const FIntVector& pos)
{
	AActor* x = CreateStone(BP_Name, pos);
}

bool UMapLauncherLocal::ServerCreateStone_Validate(const FString& BP_Name, const FIntVector& pos)
{
	return true;
}




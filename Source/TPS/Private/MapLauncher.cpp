// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLauncher.h"
#include "StoneBase.h"
#include <GameFramework/PlayerStart.h>
#include <GameFramework/PlayerStart.h>
#include "MapLauncherLocal.h"


// Sets default values for this component's properties
UMapLauncher::UMapLauncher()
{

	launcherloca = CreateDefaultSubobject<UMapLauncherLocal>(TEXT("STone"));
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	instance = this;
	// ...
}


// Called when the game starts
void UMapLauncher::BeginPlay()
{
	Super::BeginPlay();
	InitializeMap();
	// ...

	 
}

// Called every frame
void UMapLauncher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMapLauncher::InitializePlayerStart(const vector<vector<int>>&map)
{
	int32 mx = 0;
	for (int i = 0;i < map.size();i++) {
		for (int j = 0;j < map[i].size();j++) {
			mx = max(mx, map[i][j]);
		}
	}
	for (int i = 0;i < BlockSize;i++) {
		for (int j = 0;j < BlockSize;j++) {
			DispatchCreateMsg("BP_PlayerStart_0",FIntVector(i, j, map[i][j]*(BlockBlank + 1) + 4)); //CreateActor("BP_PlayerStart",FIntVector(i,j, map[i][j]+4));
		//	APlayerStart* x = Cast<APlayerStart>(obj);
		//	x->PlayerStartTag = FName("0");
		}
	}
	for (int i = 0;i < BlockSize;i++) {
		for (int j = 0;j < BlockSize;j++) {
			int32 x = map.size() - 1 - i;
			int32 y = map[x].size() - 1 - j;
			DispatchCreateMsg("BP_PlayerStart_1", FIntVector(x,y, map[x][y]*(BlockBlank + 1) + 4));
			
		}
	}
	int32 x = map.size() ;
	int32 y = map[x/2].size() ;
	DispatchCreateMsg("BP_NEWFlag", FIntVector(x/2, y/2, mx* (BlockBlank + 1) + 10));
	DispatchCreateMsg("BP_NEWFlag", FIntVector(x / 2,1, mx* (BlockBlank + 1) + 10));
	DispatchCreateMsg("BP_NEWFlag", FIntVector(x/2, y-2, mx * (BlockBlank + 1) + 10));
}
void UMapLauncher::InitializeMap() {
	vector<vector<int>>map = MapProductor(BlockSize,MapSize).getMap(1349880437);
	InitializePlayerStart(map);
	//PositionTranslator trans(StoneScale);
	for (int i = 0;i < map.size();i++) {
		for (int j = 0;j < map[i].size();j++) {
			for (int k = 0;k <= map[i][j];k++) {
				UE_LOG(LogTemp, Log,TEXT( "地图%d %d %d"), i, j, k);
				
				TryCreateStone("BP_NStoneBase", FIntVector(i, j, (BlockBlank+1)*k));
/*				UClass* BlueprintVar = StaticLoadClass(AStoneBase::StaticClass(), nullptr, TEXT("Blueprint'/Game/Blueprints/BP_NStoneBase.BP_NStoneBase_C'"));
				if (BlueprintVar != nullptr)
				{
					// 向场景中添加新生成的蓝图实例
					
					AStoneBase* pMyActor = GetWorld()->SpawnActor<AStoneBase>(BlueprintVar, transFromDispersedToContinuous(FIntVector(i, j, k)),FRotator::ZeroRotator);
					if (pMyActor)
					{
						FVector v = transFromDispersedToContinuous(FIntVector(i, j, k));
						UE_LOG(LogTemp, Log, TEXT("Product Stone%f %f %f"),v.X,v.Y,v.Z);
					//	pMyActor->GetTransform().TransformPosition(transFromDispersedToContinuous(FIntVector(i, j, k)));
				//		pMyActor->SetActorLocation(transFromDispersedToContinuous(FIntVector(i, j, k)));
						// 这样，场景中就会动态生成一个蓝图类实例
						// 我们也会得到一个蓝图类基类的实例指针，并可以调用基类"AMyActor"中的函数
					}
				}*/

			/*	FVector p = PositionTranslator::transFromDispersedToContinuous(FIntVector{
					i, j, k
				});
				
				FTransform spawnTransform;
				spawnTransform.SetLocation(transFromDispersedToContinuous(FIntVector(i, j, k)));
			//	ConstructorHelpers::FClassFinder<AStoneBase> BPClass(TEXT("/Game/Blueprints"));
				FActorSpawnParameters spawn;
				ConstructorHelpers::FClassFinder<AStoneBase> BPClass(TEXT("/Game/Blueprints"));
				AStoneBase *x = GetWorld()->SpawnActor<AStoneBase>(BPClass.Class,spawnTransform, spawn);
			//	instance->GetTransform().setLocation(transFromDispersedToContinuous(FIntVector(i, j, k)));*/
			}
		}
	}
}

FVector UMapLauncher::transFromDispersedToContinuous(FIntVector p) {
	float HEIGHT=StoneScale.Z;
//	float EDGEWIDTH=StoneScale.X;
	FVector ans;
	ans.Z = p.Z * HEIGHT + HEIGHT / 2.0;
	ans.Y = (p.Y - 0.5 * (p.X & 1)) * sqrt(3) * StoneScale.Y;
	ans.X = p.X * 1.5 * StoneScale.X;
	return ans*50;
}

bool UMapLauncher::TryCreateStone(const FString &BP_Name, const FIntVector &pos) {
	if ((BP_Name == "BP_Fire" || BP_Name == "BP_Water" || BP_Name == ""||BP_Name=="BP_NStoneBase") == false) {
		UE_LOG(LogTemp, Warning, TEXT("%s"),&BP_Name[0]);
		UE_LOG(LogTemp, Warning, TEXT("格式错误"));
		return false;
	}

	if (BP_Name == "") {
		if (StoneMap.Find(pos) !=nullptr) {
			DispatchCreateMsg("", pos);
			StoneMap.Remove(pos);
			return true;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("删除位置为空"));
			return false;
		}

		//	StoneMap.Add(TTuple<FIntVector, FString>(pos, *BP_Name));
	}
	else {

		FString* name = StoneMap.Find(pos);
		if (name == nullptr) {
			StoneMap.Add(TTuple<FIntVector, FString>(pos, BP_Name));
			return DispatchCreateMsg(BP_Name, pos);
		}else if (*name == BP_Name || *name == "BP_NStoneName") {
			return nullptr;
		}
		else
		{
			StoneMap.Remove(pos);
			DispatchCreateMsg("", pos);
			if (BP_Name == "BP_Fire") {
				StoneMap.Add(TTuple<FIntVector, FString>(pos, "BP_NStoneName"));
				return DispatchCreateMsg("BP_NStoneName", pos);
			}
			else if (BP_Name == "BP_Water") {
				return DispatchCreateMsg("BP_Wind", pos);
				//	StoneMap.Add(TTuple<FIntVector, FString>(pos, "BP_Wind"));
			}
		}
	}
	return true;

}


bool UMapLauncher::DispatchCreateMsg(const FString&BP_Name, const FIntVector& pos) 
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		launcherloca->ServerCreateStone(BP_Name, pos);
		//UE_LOG(LogTemp, Log, TEXT("服务端"));
		return true;
	}
	else
		launcherloca->LocalCreateStone(BP_Name, pos);
			return true;
}

/*AActor* UMapLauncher::CreateActor(const FString& BP_Name, const FIntVector& pos) {
	FString s = "Blueprint'/Game/Blueprints/";
	s += BP_Name + ".";
	s += BP_Name + "_C'";
	//	UE_LOG(LogTemp, Log, TEXT(&s[0]));

	UClass* BlueprintVar = StaticLoadClass(AActor::StaticClass(), nullptr, &s[0]);
	if (BlueprintVar != nullptr)
	{
		// 向场景中添加新生成的蓝图实例

		AActor* pMyActor = GetWorld()->SpawnActor<AActor>(BlueprintVar, UMapLauncher::getInstance()->transFromDispersedToContinuous(pos), FRotator::ZeroRotator);
		if (pMyActor)
		{
			return pMyActor;
		}
	}
	return nullptr;
}*/

/*AStoneBase* UMapLauncher::CreateStone(UWorld* world, FString BP_Name, FIntVector pos) {
	//AStoneBase* x =GetWorld()-> SpawnActor<AStoneBase>(AStoneBase::StaticClass(),UMapLauncher->getInstance()->transFromDispersedToContinuous(pos));
	//return x;
	FString s = "Blueprint'/Game/Blueprints/";
	s += BP_Name + ".";
	s += BP_Name + "_C'";
//	UE_LOG(LogTemp, Log, TEXT(&s[0]));

	UClass* BlueprintVar = StaticLoadClass(AStoneBase::StaticClass(), nullptr, &s[0]);
	if (BlueprintVar != nullptr)
	{
		// 向场景中添加新生成的蓝图实例

		AStoneBase* pMyActor = world->SpawnActor<AStoneBase>(BlueprintVar, transFromDispersedToContinuous(pos), FRotator::ZeroRotator);
		if (pMyActor)
		{
			pMyActor->position = pos;
			UE_LOG(LogTemp, Log, TEXT("!!!new obj z :%f"), transFromDispersedToContinuous(pos).Z);
			return pMyActor;
		}
	}
	return nullptr;
}*/

/*
template<UCLASS T>
void UMapLauncher::CreateStone<T>(FIntVector pos){

//	AActor * stone= GetWorld()->SpawnActor<AActor::StaticClass()>(Stone,pos);
	/*static ConstructorHelpers::FClassFinder<AActor> bpClass(TEXT("/Game/BluePrints/BP_stone"));
	if (bpClass.Class != NULL)
	{
		AActor *x=GetWorld()->SpawnActor(bpClass.Class);
		x->SetActorLocation(pos);
		x->SetActorScale3D(FVector(StoneScale));
	}
	StoneBase* x = GetWorld()->SpawnActor<StoneBase>(StoneBase::StaticClass);
	x->SetActorLocation(pos);
	x->SetActorScale3D(FVector(StoneScale));
}
*/

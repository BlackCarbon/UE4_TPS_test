// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSFlagGameMode.h"
#include <TPSFlag.h>
#include "TPSPlayerController.h"
#include <TPSPlayerState.h>
#include "TPSCharacter.h"
#include "StoneBase.h"


//取消了团队加分项目
void ATPSFlagGameMode::HandleOnActorKilled(AActor* VictimActor, AActor* KillerActor, AController* KillerController)
{
	//摧毁岩石加1分，杀死地方玩家加3分
	auto world = GetWorld();
	ATPSTDMGameMode *GM = nullptr;
	ATPSGameState *GS = nullptr;
	if (ensureAlways(world))
	{
		GM = world->GetAuthGameMode<ATPSTDMGameMode>();
		GS = world->GetGameState<ATPSGameState>();
		ensureAlways(GM);
		ensureAlways(GS);
	}
	{
		auto stone = Cast<AStoneBase>(VictimActor);
		if (stone && KillerController)
		{
			auto PS = KillerController->GetPlayerState<ATPSPlayerState>();
			if (PS)
			{
				PS->IncreaseScore(1.0f);
			}
		}
	}
	{
		auto hostilePlayer = Cast<ATPSCharacter>(VictimActor);
		if (hostilePlayer && KillerController)
		{
			auto PS = KillerController->GetPlayerState<ATPSPlayerState>();
			if (PS)
			{
				//个人得分
				PS->IncreaseScore(3.0f);
			}
		}
	}
}

void ATPSFlagGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckAllFlag();

}

TArray<int> CountTeam(TArray<AController*> arr)
{
	TArray<int> ret;
	ret.Empty();
	ret.SetNum(2);
	ret[0] = ret[1] = 0;
	for (auto x : arr)
	{
		auto PC = Cast<ATPSPlayerController>(x);
		if (ensureAlways(PC) && PC->TeamID <= 1 && PC->TeamID >= 0)
		{
			++ret[PC->TeamID];
		}
	}
	return ret;
}

void ATPSFlagGameMode::CheckAllFlag()
{
	for (TObjectIterator<ATPSFlag>It; It; ++It)
	{
		auto flag = *It;
		if(!ensure(flag)) continue;
		TArray<AController*> ctrls = flag->GetFlagController();
		TArray<int> cnt = CountTeam(ctrls);
		if (flag->getFlagTeam() == 0)
		{
			//一样多就无变化
			if (cnt[0] == cnt[1])
			{
				flag->SetSpeed(0);
				continue;
			}
			int winnerTeam = cnt[1] > cnt[0];
			int deltaNum = FMath::Abs(cnt[1] - cnt[0]);
			//人多的队伍，加分
			for (auto x : ctrls)
			{
				auto PC = Cast<ATPSPlayerController>(x);
				if (PC && PC->TeamID == winnerTeam)
				{
					auto PS = PC->GetPlayerState<ATPSPlayerState>();
					if (PS)
					{
						PS->IncreaseScore(1.0f);
					}
				}
			}
			flag->SetSpeed(FlagSpeed(deltaNum));
			if (winnerTeam == FIRE)
			{
				flag->FireFlag();
			}
			else
			{
				flag->WaterFlag();
			}
		}
		else
		{
			//Fire
			if (flag->getFlagTeam() == 2)
			{
				if (cnt[FIRE] >= cnt[WATER])
				{
					int d = cnt[FIRE] - cnt[WATER];
					flag->SetSpeed(FlagSpeed(d));
					continue;
				}
				else
				{
					int d = cnt[WATER] - cnt[FIRE];
					flag->SetSpeed(-FlagSpeed(d));
					for (auto x : ctrls)
					{
						auto PC = Cast<ATPSPlayerController>(x);
						if (PC && PC->TeamID == WATER)
						{
							auto PS = PC->GetPlayerState<ATPSPlayerState>();
							if (PS)
							{
								PS->IncreaseScore(1.0f);
							}
						}
					}
				}
			}
			//WATER
			else
			{
				if (cnt[WATER] >= cnt[FIRE])
				{
					int d = cnt[WATER] - cnt[FIRE];
					flag->SetSpeed(FlagSpeed(d));
					continue;
				}
				else
				{
					int d = cnt[FIRE] - cnt[WATER];
					flag->SetSpeed(-FlagSpeed(d));
					for (auto x : ctrls)
					{
						auto PC = Cast<ATPSPlayerController>(x);
						if (PC && PC->TeamID == FIRE)
						{
							auto PS = PC->GetPlayerState<ATPSPlayerState>();
							if (PS)
							{
								PS->IncreaseScore(1.0f);
							}
						}
					}
				}
			}

		}
	}

}

float ATPSFlagGameMode::FlagSpeed(int n)
{
	if (n == 0) return 0;
	static float e = FMath::Exp(1.0f);
	return FMath::Loge(e + n - 1);
}

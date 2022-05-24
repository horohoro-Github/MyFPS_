// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Fire.h"
#include "MyAIEnemy.h"
#include "UObject/ConstructorHelpers.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "MyEnemyProjectile.h"

UMyBTTask_Fire::UMyBTTask_Fire()
{
	NodeName = TEXT("Fire");
}
//작동시 플레이어에게 총 발사
EBTNodeResult::Type UMyBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	auto const AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	if (auto const player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if (player->bDead==false)
		{
			AMyAIEnemy* enemy = Cast<AMyAIEnemy>(AICon->GetPawn());

			enemy->Fire();
		
		}
		else
		{
			AICon->GetBlackboardComp()->SetValueAsBool(bb_keys::can_see_player, false);
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

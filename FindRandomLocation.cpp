// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_vector.h"
#include "MyAIEnemy.h"
#include "MyBlackboardKey.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}
//랜덤 위치 이동 BTTaskNode
EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	auto const Enemy = AICon->GetPawn();

	FVector const origin = Enemy->GetActorLocation();
	FNavLocation loc;

	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
	{
		AICon->GetBlackboardComp()->SetValueAsVector(bb_keys::Target_Location, loc.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);



	return EBTNodeResult::Succeeded;
}

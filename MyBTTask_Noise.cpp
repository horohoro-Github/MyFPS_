// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_Noise.h"
#include "MyAIController.h"
#include "MyBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
UMyBTTask_Noise::UMyBTTask_Noise()
{
	NodeName = TEXT("Noise");
}
//총소리 들리면 값 전달
EBTNodeResult::Type UMyBTTask_Noise::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	AICon->GetBlackboardComp()->SetValueAsBool(bb_keys::hearing_sound, false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

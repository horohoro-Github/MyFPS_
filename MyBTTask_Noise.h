// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MyBTTask_Noise.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYFPS__API UMyBTTask_Noise : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UMyBTTask_Noise();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};

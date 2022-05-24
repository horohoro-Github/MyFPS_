// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MyBTTask_Fire.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYFPS__API UMyBTTask_Fire : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMyBTTask_Fire();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		TSubclassOf<AActor>BulletClass;
	
};

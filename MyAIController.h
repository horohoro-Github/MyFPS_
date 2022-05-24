// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS__API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn)override;
	class UBlackboardComponent* GetBlackboardComp()const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		class UBehaviorTreeComponent* BTComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AI)
		class UBehaviorTree* BT;
	
	class UBlackboardComponent* BBComp;

	class UAISenseConfig_Sight* SightComp;
	class UAISenseConfig_Hearing* HearingComp;

	UFUNCTION()
		void on_target_detected(AActor* actor, FAIStimulus const stimulus);
	UFUNCTION()
		void on_hearing_updated(TArray<AActor*> const& updated_actors);
	void setup_perception_system();
};

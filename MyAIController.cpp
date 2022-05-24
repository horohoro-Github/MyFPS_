// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MyBlackboardKey.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.h"
#include "MyAIEnemy.h"

AMyAIController::AMyAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Tree(TEXT("BehaviorTree'/Game/Asset/AI/AIBehaviorTree.AIBehaviorTree'"));

	if (Tree.Succeeded()) BT = Tree.Object;
	
	BTComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BBComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	setup_perception_system();
}
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BT);
	BTComp->StartTree(*BT);
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BBComp) BBComp->InitializeBlackboard(*BT->BlackboardAsset);

	
}

UBlackboardComponent* AMyAIController::GetBlackboardComp() const
{
	return BBComp;
}
//플레이어를 발견했을때 반응
void AMyAIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
	if (auto const player = Cast<AMyCharacter>(actor))
	{
			GetBlackboardComp()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
			GetBlackboardComp()->SetValueAsObject(bb_keys::Enemy, player);
	}
}
//플레이어가 쏜 총의 소리에 반응
void AMyAIController::on_hearing_updated(TArray<AActor*> const& updated_actors)
{
	for (size_t i = 0; i < updated_actors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(updated_actors[i],info);
		for (size_t j = 0; j < info.LastSensedStimuli.Num(); j++)
		{
			FAIStimulus const stimulus = info.LastSensedStimuli[j];
			if (stimulus.Tag == bb_keys::noise_tag)
			{
				
				GetBlackboardComp()->SetValueAsBool(bb_keys::hearing_sound, stimulus.WasSuccessfullySensed());
				GetBlackboardComp()->SetValueAsVector(bb_keys::Target_Location,stimulus.StimulusLocation);
			}
		}
	}
}

void AMyAIController::setup_perception_system()
{
	SightComp = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightComp)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightComp->SightRadius = 1500.f;
		SightComp->LoseSightRadius = SightComp->SightRadius + 50.f;
		SightComp->PeripheralVisionAngleDegrees = 130.f;
		SightComp->SetMaxAge(5.f);
		SightComp->AutoSuccessRangeFromLastSeenLocation = 0.f;
		SightComp->DetectionByAffiliation.bDetectEnemies = true;
		SightComp->DetectionByAffiliation.bDetectFriendlies = true;
		SightComp->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightComp->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::on_target_detected);
		GetPerceptionComponent()->ConfigureSense(*SightComp);
	}

	HearingComp = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingComp)
	{
		HearingComp->HearingRange = 1000.f;
		HearingComp->DetectionByAffiliation.bDetectEnemies = true;
		HearingComp->DetectionByAffiliation.bDetectFriendlies = true;
		HearingComp->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMyAIController::on_hearing_updated);
		GetPerceptionComponent()->ConfigureSense(*HearingComp);
	}
}

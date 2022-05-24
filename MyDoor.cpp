// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDoor.h"

// Sets default values
AMyDoor::AMyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	P_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Particle1"));
	P_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Particle2"));
	P_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Particle3"));
	P_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Particle4"));
	Door_Trigger->SetGenerateOverlapEvents(true);
	Door_Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyDoor::OpenDoor);

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("audio"));
	audioComp->SetupAttachment(SM_Door);
	P_1->SetupAttachment(SM_Door);
	P_2->SetupAttachment(SM_Door);
	P_3->SetupAttachment(SM_Door);
	P_4->SetupAttachment(SM_Door);

}

void AMyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//트리거 작동시 문 열림
	if (bWorked == true)
	{
		if (bShopDoor == true) 
		{
			FVector NewLocation = SM_Door->GetComponentLocation();
			if (NewLocation.Z != NewLoc.Z)
			{
				NewLocation.Z -= 1.f;
				SM_Door->SetWorldLocation(NewLocation);
			}
			else
			{

				PrimaryActorTick.SetTickFunctionEnable(false);

				audioComp->SetPaused(true);
				Destroy();
			}
		}
		else
		{

		
			FRotator NewRotation = SM_Door->GetComponentRotation();

			if (bOnclick == true)
			{
				if (NewRotation.Roll >= NewRot.Roll)
				{
					NewRotation.Roll -= 1.f;
					SM_Door->SetWorldRotation(NewRotation);

				}
				else
				{
					audioComp->SetPaused(true);
					PrimaryActorTick.SetTickFunctionEnable(false);
				}
			}
			else
			{
				if (NewRotation.Roll <= NewRot2)
				{
					NewRotation.Roll += 1.f;
					SM_Door->SetWorldRotation(NewRotation);

				}
				else
				{
					audioComp->SetPaused(true);
					PrimaryActorTick.SetTickFunctionEnable(false);
				}
			}
			
		}
	}



}

void AMyDoor::OpenDoor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player") && bShopDoor==true)
	{
		bWorked = true;
		audioComp = UGameplayStatics::SpawnSoundAttached(S_Spark, audioComp, FName("S_Door1"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
		Working();
	}
}

void AMyDoor::Working()
{
	UGameplayStatics::SpawnEmitterAttached(P_Spark, P_1, FName("P_Door1"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f),FVector(1.5f));
	UGameplayStatics::SpawnEmitterAttached(P_Spark, P_2, FName("P_Door2"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), FVector(1.5f));
	UGameplayStatics::SpawnEmitterAttached(P_Spark, P_3, FName("P_Door3"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), FVector(1.5f));
	UGameplayStatics::SpawnEmitterAttached(P_Spark, P_4, FName("P_Door4"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), FVector(1.5f));
	
}
//버튼 상호작용
void AMyDoor::OnClicked_Implementation()
{
	if (bOnclick == false)
	{
		bOnclick = true;
	}
	else
	{
		bOnclick = false;
	}
	
	audioComp->SetPaused(true);
	if (S_Open != nullptr)
		audioComp = UGameplayStatics::SpawnSoundAttached(S_Open, SM_Door, FName("Door"), FVector(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset);
	PrimaryActorTick.SetTickFunctionEnable(true);
	bWorked = true;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonB.h"

// Sets default values
AMyButtonB::AMyButtonB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Button = CreateDefaultSubobject<UBoxComponent>(TEXT("Button"));
	RootComponent = Button;

	Button->SetGenerateOverlapEvents(true);
	Button->OnComponentBeginOverlap.AddDynamic(this, &AMyButtonB::BeginOverlapButton);
	Button->OnComponentEndOverlap.AddDynamic(this, &AMyButtonB::EndOverlapButton);

	TRComp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT"));
	TRComp->SetupAttachment(Button);

}

void AMyButtonB::Click()
{

	
	if (bWorked == false && bPlayerIsNearbyThis==true)
	{
		Execute_OnConnectButtonDoor(DoorB, this);
		
		bWorked = true;
	}
}

void AMyButtonB::OnClicked_Implementation()
{
	bWorked = false;
}

void AMyButtonB::OnConnectButtonDoor_Implementation(AActor* actor)
{
	bWorked = false;
}

void AMyButtonB::BeginOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{

		bPlayerIsNearbyThis = true;
		//EnableInput(UGameplayStatics::GetPlayerController(this, 0));

		//InputComponent->BindAction("Interaction", IE_Pressed, this, &AMyButtonB::Click);
	}
}

void AMyButtonB::EndOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		bPlayerIsNearbyThis = false;
		//DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	}
}


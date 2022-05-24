// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButton.h"

// Sets default values
AMyButton::AMyButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Button = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button"));
	C_Button = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	TR_Button = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	C_Button->SetupAttachment(SM_Button);
	C_Button->SetGenerateOverlapEvents(true);
	C_Button->OnComponentBeginOverlap.AddDynamic(this, &AMyButton::BeginOverlapButton);
	C_Button->OnComponentEndOverlap.AddDynamic(this, &AMyButton::EndOverlapButton);
	TR_Button->SetupAttachment(SM_Button);
}

// Called when the game starts or when spawned
void AMyButton::BeginPlay()
{
	Super::BeginPlay();
	
	
	GetActor();

}

void AMyButton::OnClicked_Implementation()
{
	bTransparent = false;
	TR_Button->SetVisibility(true);
	C_Button->SetGenerateOverlapEvents(true);
	bOpenDoor = true;
}

void AMyButton::Click()
{
	if (bOpenDoor == true && bPlayerIsNearbyThis==true)
	{
		bTransparent = true;
		TR_Button->SetVisibility(false);
		C_Button->SetGenerateOverlapEvents(false);
		//DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		bOpenDoor = false;
		if (button)
			Execute_OnClicked(button);
		if (door)
		{

			Execute_OnClicked(door);
		}
	}

	

}

void AMyButton::GetActor()
{	//버튼을 눌렀을때 작동할 문

	door = Cast<AMyDoor>(UGameplayStatics::GetActorOfClass(GetWorld(), Door));
	button = Cast<AMyButton>(UGameplayStatics::GetActorOfClass(GetWorld(), OtherButton));
	if (bTransparent == true)
	{
		TR_Button->SetVisibility(false);
		C_Button->SetGenerateOverlapEvents(false);
	}
}

void AMyButton::BeginOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player") && bTransparent==false)
	{
		
		//EnableInput(UGameplayStatics::GetPlayerController(this, 0));
		//InputComponent->BindAction("Interaction", IE_Pressed, this, &AMyButton::Click);
		bPlayerIsNearbyThis = true;
	}
}

void AMyButton::EndOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor->ActorHasTag("Player"))
	{
		//DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		bPlayerIsNearbyThis = false;
	}
}


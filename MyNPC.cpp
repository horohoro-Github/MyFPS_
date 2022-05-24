// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNPC.h"

// Sets default values
AMyNPC::AMyNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	InteractionZone->SetupAttachment(GetMesh());
	InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &AMyNPC::BeginOverlap);
	InteractionZone->OnComponentEndOverlap.AddDynamic(this, &AMyNPC::EndOverlap);
	TR_Comp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	TR_Comp->SetupAttachment(GetMesh());
	TR_Comp->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMyNPC::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters weaponSpawnParams;
	CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), weaponSpawnParams);
	CurrentWeapon->SetActorScale3D(CurrentWeapon->weapondata.ItemSize);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->weapondata.EquipSocketName);

}

void AMyNPC::OpenShop()
{
	if (bPressedG == false && bPlayerIsNearbyThis == true)
	{
		bPressedG = true;
		AMyFPS_GameModeBase* mode = Cast<AMyFPS_GameModeBase>(UGameplayStatics::GetGameMode(this));
		mode->ChangeWidget(mode->EWidget::Shopping);
	}
}
//인벤토리 창 닫힐때 알림
void AMyNPC::OnClicked_Implementation()
{
	bPressedG = false;
}

//NPC와 상호작용
void AMyNPC::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		//AController* con = Cast<AController>(UGameplayStatics::GetPlayerController(this, 0));
		TR_Comp->SetVisibility(true);
	//	con->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
		bPlayerIsNearbyThis = true;
		//con->InputComponent->BindAction("Interaction", IE_Pressed, this, &AMyNPC::OpenShop);
	}
}

void AMyNPC::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		
		//AController* con = Cast<AController>(UGameplayStatics::GetPlayerController(this, 0));
		TR_Comp->SetVisibility(false);
	//	con->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		bPlayerIsNearbyThis = false;
	}
}


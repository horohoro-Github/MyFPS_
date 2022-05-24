// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTransparencyWall.h"

// Sets default values
AMyTransparencyWall::AMyTransparencyWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall"));
	C_Wall = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	C_Wall->SetupAttachment(SM_Wall);
	C_Wall->SetGenerateOverlapEvents(true);

	C_Wall->OnComponentBeginOverlap.AddDynamic(this, &AMyTransparencyWall::BeginOverlaps);
	C_Wall->OnComponentEndOverlap.AddDynamic(this, &AMyTransparencyWall::EndOverlaps);
}

// Called when the game starts or when spawned
void AMyTransparencyWall::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryActorTick.SetTickFunctionEnable(false);
}

// Called every frame
void AMyTransparencyWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TransparencyProgress += DeltaTime;
	float Opacity = 0.f;
	if (bIsItTransparent)
	{
		Opacity = FMath::Lerp(1.f, 0.1f, TransparencyProgress * (1.f / 0.3f));

	}
	else
	{
		Opacity = FMath::Lerp(0.1f, 1.f, TransparencyProgress * (1.f / 0.3f));

	}
	SM_Wall->SetScalarParameterValueOnMaterials(TEXT("Opacity"), Opacity);

	if (TransparencyProgress > 0.3f)
	{
		TransparencyProgress = 0.f;
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void AMyTransparencyWall::SetShowActor(bool bShowActor)
{
	bIsItTransparent = bShowActor;
	if (TransparencyProgress != 0.f)
	{
		TransparencyProgress -= 0.3f;
	}
	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AMyTransparencyWall::BeginOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		SetShowActor(true);
	}
}

void AMyTransparencyWall::EndOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		SetShowActor(false);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIEnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AMyAIEnemySpawner::AMyAIEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spawner = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn"));
	RootComponent = Spawner;

}

// Called when the game starts or when spawned
void AMyAIEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnEnemies();
}

//특정 위치에 적군 혹은 랜덤한 위치에 폭발물 통 생성
void AMyAIEnemySpawner::SpawnEnemies()
{
	if (bSpawner_Enemy == true)
	{	
		TArray<AActor*>actor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnEnemy, actor);
		if (actor.Num() < 10)
		{
			GetWorld()->SpawnActor<AActor>(SpawnEnemy, GetActorLocation(), GetActorRotation(), SpawnEnemyParams);
		}
		GetWorld()->GetTimerManager().SetTimer(SpawnEnemyhandle, this, &AMyAIEnemySpawner::SpawnEnemies, 1.f, false);
	}
	else
	{
		TArray<AActor*>actor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnBarrel,actor);
	
		if (actor.Num() < 20)
		{
			while (true)
			{
				float LocY = UKismetMathLibrary::RandomFloatInRange(400.f, 4400.f);
				float LocX = UKismetMathLibrary::RandomFloatInRange(-200.f, 4600.f);

				if ((LocY <= 3600.f && LocY >= 1550.f) && (LocX >= 1000.f && LocX <= 3200.f))
				{

				}
				else
				{
					GetWorld()->SpawnActor<AActor>(SpawnBarrel, FVector(LocX, LocY, 2000.f), GetActorRotation(), SpawnBarrelParams);
					break;
				}
			}
		}

		GetWorld()->GetTimerManager().SetTimer(SpawnBarrelhandle, this, &AMyAIEnemySpawner::SpawnEnemies, 5.f, false);
	}
}


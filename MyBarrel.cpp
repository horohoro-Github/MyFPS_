// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBarrel.h"
#include "MyAIEnemy.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyBarrel::AMyBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Barrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Barrel"));
	SM_Barrel->SetSimulatePhysics(true);
	

	RootComponent = SM_Barrel;

	HP = 50;
	ExplosionDamage = 80;
}

// Called when the game starts or when spawned
void AMyBarrel::BeginPlay()
{
	Super::BeginPlay();

	SM_Barrel->OnComponentHit.AddDynamic(this, &AMyBarrel::DropEvent);

}

void AMyBarrel::BeAttacked(int Damage)
{
	HP -= Damage;
	if (HP <= 0)
	{
		
		Explosion();
	
	}
	
}

//일정 범위에 폭발 피해
void AMyBarrel::Explosion()
{
	if (P_Explosion != nullptr)
	{
		UParticleSystemComponent* PExplosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_Explosion, GetActorTransform());
		PExplosion->SetRelativeScale3D(FVector(5.f));
	}
	if (S_Explosion != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), S_Explosion, GetActorLocation());
	}

	TArray<FHitResult> HitActors;

	FVector StartT = GetActorLocation();
	FVector EndT = StartT;
	EndT.Z += 300.f;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(500.f);

	if (GetWorld()->SweepMultiByChannel(HitActors, StartT, EndT, FQuat::FQuat(), ECC_GameTraceChannel3, CollisionShape))
	{
		for (auto Actors = HitActors.CreateIterator(); Actors; Actors++)
		{
			if (AMyAIEnemy* enemy = Cast<AMyAIEnemy>((*Actors).GetActor())) enemy->BeAttacked(ExplosionDamage);
			if (AMyBarrel* barrel = Cast<AMyBarrel>((*Actors).GetActor()))
			{
				if(barrel->HP>0)
				barrel->BeAttacked(ExplosionDamage);
			}
			if (AMyCharacter* player = Cast<AMyCharacter>((*Actors).GetActor())) player->BeAttacked(ExplosionDamage);
		}
	}
	Destroy();
}

void AMyBarrel::DropEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FTimerHandle cooldown;

	if (OtherActor != nullptr && bDoOnce == false)
	{
		bDoOnce = true;
		if (DropSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DropSound, Hit.Location);

		GetWorld()->GetTimerManager().SetTimer(cooldown, FTimerDelegate::CreateLambda([&]()
			{
				bDoOnce = false;
			}), 0.5f, false);
	}
}


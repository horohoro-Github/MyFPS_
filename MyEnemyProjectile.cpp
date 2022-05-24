// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "MyAIEnemy.h"
#include "MyBarrel.h"
// Sets default values
AMyEnemyProjectile::AMyEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Bullet"));
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyEnemyProjectile::OnHit);

	RootComponent = CollisionComp;

	MovementComp = CreateDefaultSubobject <UProjectileMovementComponent>(TEXT("Movement"));
	MovementComp->InitialSpeed = 3000.f;
	InitialLifeSpan = 3.f;

}

void AMyEnemyProjectile::SetBulletInfo(int Value)
{
	
	Damage = Value;
}
//적군이 쏘는 발사체
void AMyEnemyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (!OtherActor->ActorHasTag("Enemy"))
	{

		if (BulletHole != nullptr)
		{
			
			UGameplayStatics::SpawnDecalAttached(BulletHole, FVector(15.f, 15.f, 15.f), OtherComp, FName("BulletHole"), HitResult.Location, UKismetMathLibrary::MakeRotFromX(HitResult.Normal), EAttachLocation::KeepWorldPosition, 5.f);
		}
		if (ParticleHitObject != nullptr && SoundHitObject!=nullptr)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleHitObject, HitResult.Location);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundHitObject, HitResult.Location);
		}
		if (OtherActor->ActorHasTag("Player"))
		{
			AMyCharacter* player = Cast<AMyCharacter>(OtherActor);
		
			player->BeAttacked(Damage);

		}
		if (OtherActor->ActorHasTag("Barrel"))
		{
			AMyBarrel* barrel = Cast<AMyBarrel>(OtherActor);
			barrel->BeAttacked(Damage);
		}
		
		Destroy();
	}
}


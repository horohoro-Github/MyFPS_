// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProjectile.h"
#include "MyCharacter.h"
#include "MyAIEnemy.h"
#include "MyBarrel.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/Skeleton.h"
// Sets default values
AMyProjectile::AMyProjectile()
{

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->OnComponentHit.AddDynamic(this, &AMyProjectile::BulletHit);
	CollisionComp->SetSimulatePhysics(true);

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	InitialLifeSpan = 3.0f;
}

//무기 데미지 가져오기
int AMyProjectile::GetBulletInfo()
{
	AMyCharacter* player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (player->CurrentWeapon != nullptr)
	{
		return player->CurrentWeapon->weapondata.Damage;
	}
	else
		return 20;
	
}

//폭발 피해
void AMyProjectile::Explosion()
{
	if (ParticleExplosion != nullptr)
	{
		UParticleSystemComponent* PExplosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleExplosion, GetActorTransform());
		PExplosion->SetRelativeScale3D(FVector(5.f));
	}
	if (SoundExplosion != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundExplosion, GetActorLocation());
	}

	TArray<FHitResult> HitActors;

	FVector StartT = GetActorLocation();
	FVector EndT = StartT;
	EndT.Z += 300.f;
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(Radius);

	if (GetWorld()->SweepMultiByChannel(HitActors, StartT, EndT, FQuat::FQuat(), ECC_GameTraceChannel3, CollisionShape))
	{
		for (auto Actors = HitActors.CreateIterator(); Actors; Actors++)
		{
				Damage = GetBulletInfo();
				if (AMyAIEnemy* enemy = Cast<AMyAIEnemy>((*Actors).GetActor()))enemy->BeAttacked(Damage);
				if (AMyBarrel* barrel = Cast<AMyBarrel>((*Actors).GetActor())) barrel->BeAttacked(Damage);
				if (AMyCharacter* player = Cast<AMyCharacter>((*Actors).GetActor())) player->BeAttacked(Damage);
		}
	}
	
}
//총알 Hit 함수 
void AMyProjectile::BulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!OtherActor->ActorHasTag("Player"))
	{
		//총알 부위별 피해
		switch (BulletType)
		{
		case EBulletType::BulletBall:
		{
			if (OtherActor->ActorHasTag("Enemy"))
			{
				Damage = GetBulletInfo();
				if (AMyAIEnemy* enemy = Cast<AMyAIEnemy>(OtherActor))
				{
					if (Hit.BoneName == "head" || Hit.BoneName == "neck_01")
					{
						enemy->BeAttacked(Damage * 4);
					}
					else if (Hit.BoneName== "spine_01" || Hit.BoneName == "spine_02" || Hit.BoneName == "spine_03")
					{
						enemy->BeAttacked(Damage);	
					}
					else
					{
						enemy->BeAttacked(Damage*0.75);
					}
				}
				
			}
			if (OtherActor->ActorHasTag("Barrel"))
			{
				Damage = GetBulletInfo();
				if (AMyBarrel* barrel = Cast<AMyBarrel>(OtherActor))
					barrel->BeAttacked(Damage);
			}
			break;
		}
		case EBulletType::Bullet:
		{
			if (BulletHole != nullptr && SoundHit!=nullptr)
			{

				UGameplayStatics::SpawnDecalAttached(BulletHole, FVector(15.f, 15.f, 15.f), OtherComp, FName("BulletHole"), Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.Normal), EAttachLocation::KeepWorldPosition, 5.f);
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundHit, Hit.Location);
			}

			if (ParticleHitObject != nullptr && !OtherActor->ActorHasTag("Enemy"))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleHitObject, Hit.Location);
			}

			if (OtherActor->ActorHasTag("Enemy"))
			{
				Damage = GetBulletInfo();
				if (AMyAIEnemy* enemy = Cast<AMyAIEnemy>(OtherActor))
				{
					if (ParticleHitEnemy)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleHitEnemy, Hit.Location);
					}
					
					if (Hit.BoneName == "head" || Hit.BoneName == "neck_01")
					{
						enemy->BeAttacked(Damage*4);
						
					}
					else if (Hit.BoneName == "spine_01" || Hit.BoneName == "spine_02" || Hit.BoneName == "spine_03")
					{
						enemy->BeAttacked(Damage);
						
					}
					else
					{
						enemy->BeAttacked(Damage*0.75);
						
					}
				}
			}
			if (OtherActor->ActorHasTag("Barrel"))
			{
				Damage = GetBulletInfo();
				if (AMyBarrel* barrel = Cast<AMyBarrel>(OtherActor))
					barrel->BeAttacked(Damage);
			}
			break;
		}
		case EBulletType::Bomb:
		{
			if (BulletHole != nullptr)
			{
				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHole, FVector(150.f, 150.f, 150.f), Hit.Location, UKismetMathLibrary::MakeRotFromX(Hit.Normal), 5.f);
			}
			Explosion();

			break;
		}
		default:
			break;
		}

		Destroy();
	}

	
	
}
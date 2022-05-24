// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyProjectile.generated.h"

UCLASS()
class MYFPS__API AMyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyProjectile();

public:	

	enum EBulletType :uint8
	{
		BulletBall,
		Bullet,
		Bomb
	};

	int Damage=0;
	float Radius = 500.f;
	UPROPERTY(EditAnywhere)
		uint8 BulletType;
	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere,Category="Effect")
		UMaterialInterface* BulletHole;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* ParticleHitObject;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* ParticleHitEnemy;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* SoundHit;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* ParticleExplosion;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* SoundExplosion;
	int GetBulletInfo();

	void Explosion();
	UFUNCTION()
		void BulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};

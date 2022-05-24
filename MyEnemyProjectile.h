// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "MyEnemyProjectile.generated.h"

UCLASS()
class MYFPS__API AMyEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemyProjectile();

public:	

	int Damage = 0;

	UPROPERTY(EditAnywhere)
		USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditAnywhere, Category = "Effect")
		UMaterialInterface* BulletHole;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* ParticleHitObject;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* SoundHitObject;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* ParticleExplosion;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* SoundExplosion;

	void SetBulletInfo(int Value);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

};

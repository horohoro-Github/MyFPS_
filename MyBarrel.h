// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "MyBarrel.generated.h"

UCLASS()
class MYFPS__API AMyBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	int HP;
	bool bDoOnce = false;
	int ExplosionDamage;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_Barrel;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* P_Explosion;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* S_Explosion;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* DropSound;
	void BeAttacked(int Damage);
	void Explosion();

	UFUNCTION()
		void DropEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MyInterface.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyWeaponDataTable.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "MyProjectile.h"
#include "MyEnemyProjectile.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundAttenuation.h"
#include "Components/SpotLightComponent.h"
#include "MyWeapon.generated.h"


class AMyCharacter;
UCLASS()
class MYFPS__API AMyWeapon : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		bool bplayerWeapon = true;
	UPROPERTY(EditAnywhere)
		bool bSilence = false;
	UPROPERTY(EditAnywhere)
		bool bAimable = true;
	bool bEquip = false;
	bool bDoOnce = false;
	FTimerHandle Gunhandle;
	bool bCurrentFired = false;
	bool bPlayerIsNearbyThis = false;
	UPROPERTY(EditAnywhere)
		bool bWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItem weapondata;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* SK_Gun;
	UPROPERTY(EditAnywhere)
		USceneComponent* Muzzle;
	UPROPERTY(EditAnywhere)
		USpotLightComponent* SpotLight;
	UPROPERTY(EditAnywhere)
		USphereComponent* S_Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UWidgetComponent* widget;
	UPROPERTY(EditAnywhere)
		UCameraComponent* ScopeCamera;

	FTimerHandle RecoilHandle;

	UPROPERTY(EditAnywhere, Category = "Bullet")
		TSubclassOf<AMyProjectile>ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Bullet")
		TSubclassOf<AMyEnemyProjectile>EnemyProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effect")
		UAnimMontage* PlayerShotMontage;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* FireSound;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundBase* distraction_sound;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* DropSound;

	void Pickup();
	void Fire(APawn* Shooter);
	void CoolDown();



	UFUNCTION()
		void SendResetRecoil(AMyCharacter* player);


	UFUNCTION()
		void HitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void BeginOverlapWeapon(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapWeapon(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

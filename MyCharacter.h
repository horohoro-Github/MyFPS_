// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimMontage.h"
#include "MyFPS_.h"
#include "MyWeaponDataTable.h"
#include "MyWeapon.h"
#include "MyProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/SpotLightComponent.h"
#include "Particles/ParticleSystem.h"
#include "Perception/AISense_Hearing.h"
#include "MyBlackboardKey.h"
#include "MyScoreSave.h"
#include "Curves/CurveVector.h"
#include "MyCharacter.generated.h"

//class UAnimMontage;


class AMyWeapon;
UCLASS()
class MYFPS__API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bAiming = false;
	bool bInventorySpace = true;
	bool bReloading = false;
	bool bSprint = false;
	bool bPressed = true;
	bool bOpenedInventory = false;
	bool bTrigger = false;
	bool bAttachWeapon = false;
	bool bInputKeyDoOnce = true;
	//탄환
	int Rifle = 0;
	int Shotgun = 0;
	int Rocket = 0;
	
	ACameraActor* CurrentCamera;

	UPROPERTY(BlueprintReadWrite)
		bool bShot = true;
	UPROPERTY(BlueprintReadWrite)
		bool bSwapping = false;
	FString Scores = "SaveScore";
	USaveGame* sg;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UMyScoreSave>ss;
	bool bDead = false;
	UPROPERTY(BlueprintReadWrite)
		bool bZoomIn = false;
	UPROPERTY(BlueprintReadWrite)
		bool bJumping = false;
	UPROPERTY(BlueprintReadWrite)
		bool bIngameProgress = false;
	UPROPERTY(BlueprintReadWrite)
		float ProgressbarVal = 0.f;
	UPROPERTY(BlueprintReadWrite)
		int HP;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* BB_Gun;
	UPROPERTY(EditAnywhere)
		USceneComponent* BB_Gun_Muzzle;
	UPROPERTY()
		TArray<bool>Inventory_Check;

	UPROPERTY()
		FItem Inventory[10];

	FItem Default;

	UPROPERTY(BlueprintReadWrite)
		FItem UsingItem;
	
	UPROPERTY(BlueprintReadWrite)
		int Money;

	UPROPERTY(EditAnywhere)
		UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere)
		UCameraComponent* MovableCamera;
	UPROPERTY(EditAnywhere)
		USpotLightComponent* SpotLight;
	UPROPERTY(EditAnywhere)
		UPointLightComponent* PointLight;

	UPROPERTY(EditAnywhere,Category="AnimMontage")
		UAnimMontage* Jump_Montage;
	//UPROPERTY(EditAnywhere, Category = "AnimMontage")
		//UAnimMontage* Aim_Montage;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* SwapWeaponMontageTypeA;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* SwapWeaponMontageTypeB;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* FireAnimMontage;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* ReloadAnimMontage;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* FireSound;
	UPROPERTY(EditAnywhere, Category = "Effect")
		UParticleSystem* P_Repair;
	UPROPERTY(EditAnywhere, Category = "Effect")
		USoundCue* S_Repair;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Death_Montage;
	UPROPERTY(EditAnywhere, Category = "Player")
		TSubclassOf<APawn> playerClass;

	UParticleSystemComponent* PComp;
	UAudioComponent* SComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMyProjectile> Projectileclass;

	AMyFPS_GameModeBase* GameMode;

	UPROPERTY(BlueprintReadWrite)
	AMyWeapon* CurrentWeapon;

	FTimerHandle handle;
	FTimerHandle cooldown;
	FTimerHandle swaphandle;

	UAIPerceptionStimuliSourceComponent* stimulus;
	void setup_stimulus();

	//입력 기능
	void MoveRight(float Value);
	void MoveForward(float Value);

	void JumpStart();
	void JumpEnd();
	void OpenInventory();
	void Interaction();
	void StartFire();
	void EndFire();
	void OnFire();
	void ZoomIn();
	void ZoomOut();
	void SlotKey(float Value);
	
	void StartSprint();
	void StopSprint();
	//

	void BeAttacked(int Damage);
	void Die();

	bool PickupWeapon(AMyWeapon* ThisWeapon);

	UFUNCTION(BlueprintCallable)
		void ChangeWeapon(FItem Item_Weapon,bool bWeapon, int Index);
	void PlayerElimination();
	
	//반동
	void Recoil();
	void ResetRecoil();
	void PlayerController_Recoil(FVector2D Value);
	
	float FiringCount = 0.f;
	float TurnValue = 0.f;
	float LookUpValue = 0.f;
	FVector2D TargetRecoilAmount = (0.f, 0.f);
	FVector2D TargetRecoveryAmount = (0.f, 0.f);
	bool bResetRecoil = false;
	bool bShouldUpdateRecoil = false;

	void LookUp(float Value);
	void Turn(float Value);
	//


	UFUNCTION(BlueprintCallable)
	void RecoveryHealth(FItem Value);
	UFUNCTION(BlueprintCallable)
		FItem GetItemsInfo();

	UFUNCTION(BlueprintCallable)
		FItem GetInventorySlot(int Slot);

	UFUNCTION(BlueprintCallable)
		bool UseItem(FItem Items, int Index);
	
	UFUNCTION(BlueprintCallable)
		void SoldBuyItem(int Index);

	UFUNCTION(BlueprintCallable)
		bool DropItem(FItem items,int Index);

	UFUNCTION(BlueprintCallable)
		int GetRemainingBullets();

	void StartReload();

	UFUNCTION(BlueprintCallable)
		void Reload();
};

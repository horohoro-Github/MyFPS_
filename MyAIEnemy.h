// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyWeapon.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MyAIEnemy.generated.h"

UCLASS()
class MYFPS__API AMyAIEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyAIEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool bDead = false;
	UPROPERTY(EditAnywhere)
		int Reward;
	UPROPERTY(EditAnywhere)
		AMyWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMyWeapon>WeaponClass;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* FireAnimMontage;
	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* DyingAnimMontage;
	void Fire();

	int HP;
	void BeAttacked(int Damage);
	void Die();
	void Destroing();
};

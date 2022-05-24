// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AMyAIEnemy::AMyAIEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	HP = 100;
}

void AMyAIEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AI생성과 동시에 무기 장착

	FActorSpawnParameters weaponSpawnParams;
	CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), weaponSpawnParams);
	CurrentWeapon->SetActorScale3D(CurrentWeapon->weapondata.ItemSize);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->weapondata.EquipSocketName);
	
}

void AMyAIEnemy::Fire()
{
	if (FireAnimMontage != nullptr)
	{
		UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
		if (AnimIns != nullptr)
		{
			AnimIns->Montage_Play(FireAnimMontage, 1.f);	
		}
	}
	CurrentWeapon->Fire(this);
}

void AMyAIEnemy::BeAttacked(int Damage)
{
	HP -= Damage;
	if (HP <= 0) Die();
}

void AMyAIEnemy::Die()
{
	if (bDead == false)
	{
		bDead = true;
		if (FireAnimMontage != nullptr)
		{
			UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
			if (AnimIns != nullptr)
			{
				AnimIns->Montage_Play(DyingAnimMontage, 1.f);
			}
			AMyAIController* AICon = Cast<AMyAIController>(this->GetController());
			AICon->BTComp->StopTree();
		}

		AMyCharacter* player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		player->Money += UKismetMathLibrary::RandomIntegerInRange(Reward / 2, Reward * 2);
		AMyFPS_GameModeBase* GameMode = Cast<AMyFPS_GameModeBase>(UGameplayStatics::GetGameMode(this));
		GameMode->Score += 1;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		FTimerHandle timerhandle;
		GetWorld()->GetTimerManager().SetTimer(timerhandle, this, &AMyAIEnemy::Destroing, 10.f, false);
	}
}

void AMyAIEnemy::Destroing()
{
	CurrentWeapon->Destroy();
	Destroy();
}


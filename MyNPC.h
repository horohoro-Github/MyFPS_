// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyFPS_.h"
#include "MyCharacter.h"
#include "MyWeapon.h"
#include "Components/TextRenderComponent.h"
#include "MyNPC.generated.h"

UCLASS()
class MYFPS__API AMyNPC : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	bool bPressedG = false;
	bool bPlayerIsNearbyThis = false;
	UPROPERTY(EditAnywhere)
		UBoxComponent* InteractionZone;
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* TR_Comp;
	UPROPERTY(EditAnywhere)
		AMyWeapon* CurrentWeapon;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AMyWeapon>WeaponClass;
	
	void OpenShop();

	virtual void OnClicked_Implementation();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

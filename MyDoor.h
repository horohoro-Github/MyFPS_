// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "MyFPS_.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyDoor.generated.h"

UCLASS()
class MYFPS__API AMyDoor : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDoor();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bWorked = false;
	UPROPERTY(EditAnywhere)
	bool bOnclick = false;
	UPROPERTY(EditAnywhere)
		bool bShopDoor = true;
	UPROPERTY(EditAnywhere)
		FVector NewLoc;
	UPROPERTY(EditAnywhere)
		FRotator NewRot;
	UPROPERTY(EditAnywhere)
		float NewRot2;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_Door;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Door_Trigger;

	UPROPERTY(EditAnywhere)
		UAudioComponent* audioComp;

	UPROPERTY(EditAnywhere,Category="Particle")
		USceneComponent* P_1;
	UPROPERTY(EditAnywhere, Category = "Particle")
		USceneComponent* P_2;
	UPROPERTY(EditAnywhere, Category = "Particle")
		USceneComponent* P_3;
	UPROPERTY(EditAnywhere, Category = "Particle")
		USceneComponent* P_4;
	UPROPERTY(EditAnywhere, Category = "Particle")
		UParticleSystem* P_Spark;
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue* S_Spark;
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue* S_Open;
	
	UFUNCTION()
		void OpenDoor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void Working();

	virtual void OnClicked_Implementation();
};

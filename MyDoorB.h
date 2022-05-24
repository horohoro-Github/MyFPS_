// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyFPS_.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "MyDoorB.generated.h"

UCLASS()
class MYFPS__API AMyDoorB : public AActor,public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDoorB();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bOpenDoor = true;

	UPROPERTY(EditAnywhere)
		bool bDoorTypePitch = true;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SMDoor;
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComp;
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue* S_Open;
	UPROPERTY(EditAnywhere)
		FVector OpenLocation;
	UPROPERTY(EditAnywhere)
		FVector CloseLocation;
	UPROPERTY(EditAnywhere)
		AActor* button;
	//virtual void OnClicked_Implementation();

	virtual void OnConnectButtonDoor_Implementation(AActor* actor);
};

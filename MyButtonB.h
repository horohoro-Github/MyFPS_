// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyFPS_.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "MyCharacter.h"
#include "MyDoorB.h"
#include "Components/BoxComponent.h"
#include "MyButtonB.generated.h"

UCLASS()
class MYFPS__API AMyButtonB : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyButtonB();

public:	

	bool bWorked = false;
	bool bPlayerIsNearbyThis = false;

	UPROPERTY(EditAnywhere)
		AMyDoorB* DoorB;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Button;

	UPROPERTY(EditAnywhere)
		UTextRenderComponent* TRComp;

	void Click();

	virtual void OnClicked_Implementation();

	virtual void OnConnectButtonDoor_Implementation(AActor* actor);

	UFUNCTION()
		void BeginOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

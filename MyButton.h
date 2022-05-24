// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"
#include "MyDoor.h"
#include "MyFPS_.h"
#include "Components/TextRenderComponent.h"
#include "MyButton.generated.h"

UCLASS()
class MYFPS__API AMyButton : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool bOpenDoor = true;
	bool bPlayerIsNearbyThis = false;
	UPROPERTY(EditAnywhere)
		bool bTransparent = false;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_Button;

	UPROPERTY(EditAnywhere)
		UBoxComponent* C_Button;
	UPROPERTY(EditAnywhere)
		UTextRenderComponent* TR_Button;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> Door;
	AMyDoor* door;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> OtherButton;
	AMyButton* button;

	virtual void OnClicked_Implementation();
	void Click();
	void GetActor();
	UFUNCTION()
		void BeginOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void EndOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MyTransparencyWall.generated.h"

UCLASS()
class MYFPS__API AMyTransparencyWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTransparencyWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsItTransparent = false;
	float TransparencyProgress=0.f;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* SM_Wall;
	UPROPERTY(EditAnywhere)
		UBoxComponent* C_Wall;

	void SetShowActor(bool bShowActor);
	UFUNCTION()
		void BeginOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

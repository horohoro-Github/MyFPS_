// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYFPS__API IMyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//상호작용에 사용될 인터페이스 함수들
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnClicked();
	virtual void OnClicked_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnConnectButtonDoor(AActor* actor);
	virtual void OnConnectButtonDoor_Implementation(AActor* actor);
	//
};

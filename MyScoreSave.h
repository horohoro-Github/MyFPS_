// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyScoreSave.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS__API UMyScoreSave : public USaveGame
{
	GENERATED_BODY()
	
public:

	UMyScoreSave();

		void SetScoreIndex(int index);

		void ResetScore();

	UPROPERTY(EditAnywhere)
		int scoreIndex[10] = { 0,0,0,0,0,0,0,0,0,0 };
};

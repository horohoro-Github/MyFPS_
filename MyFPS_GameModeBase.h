// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyWeaponDataTable.h"
#include "Blueprint/UserWidget.h"
#include "MyScoreSave.h"
#include "MyWeapon.h"
#include "MyFPS_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYFPS__API AMyFPS_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	
	AMyFPS_GameModeBase();



	enum EWidget : uint8
	{
		Ingame,
		Inventory,
		Shopping,
		Loading,
		Menu
	};

	uint8 EWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPlayerDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> IngameWClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> InventoryWClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> ShoppingWClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> LoadingWClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> MenuWClass;
	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY(EditAnywhere, Category = "Shop_Item")	
		FItem Shop_Items[13];

	UPROPERTY(EditAnywhere, Category = "Player")
		TSubclassOf<APawn> playerClass;
	
	ACameraActor* MenuCamera;
	APlayerController* playerController;
	class AMyCharacter* CurrentPlayer;

	virtual void BeginPlay() override;

	void ApplyWidgetChange();

	void ApplyWidget(TSubclassOf<class UUserWidget>WidgetToApply, bool bShowMouseCursor, bool EnableClickEvent);
	UFUNCTION(BlueprintCallable)
	void ChangeWidget(uint8 Widget);

	UFUNCTION(BlueprintCallable)
		FItem GetShopItems(int index);

	void PlayerDead(AActor* player);
	UFUNCTION(BlueprintCallable)
		void RespawnPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int highScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int>ScoreTop10;

	USaveGame* save;
	FString saveString = "SaveScore";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		TSubclassOf<UMyScoreSave> ScoreClass;

	UFUNCTION(BlueprintCallable)
		void ResetKillPoint();

	void QuitGames();
};

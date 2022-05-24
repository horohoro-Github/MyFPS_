// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyFPS_GameModeBase.h"
#include "MyCharacter.h"

AMyFPS_GameModeBase::AMyFPS_GameModeBase()
{
	EWidget = EWidget::Menu;

	ScoreTop10.SetNum(10);
}

void AMyFPS_GameModeBase::BeginPlay()
{

	//게임 스코어
	if (UGameplayStatics::DoesSaveGameExist(saveString, 0))
	{
		save = UGameplayStatics::LoadGameFromSlot(saveString, 0);

		UMyScoreSave* MyScore = Cast<UMyScoreSave>(save);
		if (Score >= MyScore->scoreIndex[0])
		{
			MyScore->scoreIndex[0] = Score;
			highScore = MyScore->scoreIndex[0];
		}
		else
		{
			highScore = MyScore->scoreIndex[0];
		}

		UGameplayStatics::SaveGameToSlot(save, saveString, 0);

		for (int i = 0; i < 10; i++)
		{
			ScoreTop10[i] = MyScore->scoreIndex[i];
		}

	}
	else
	{
		save = UGameplayStatics::CreateSaveGameObject(ScoreClass);
		UMyScoreSave* MyScore = Cast<UMyScoreSave>(save);
		if (Score >= MyScore->scoreIndex[0])
		{
			MyScore->scoreIndex[0] = Score;
			highScore = MyScore->scoreIndex[0];
		}
		UGameplayStatics::SaveGameToSlot(save, saveString, 0);

		for (int i = 0; i < 10; i++)
		{
			ScoreTop10[i] = MyScore->scoreIndex[i];
		}
	}

	

	playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	EnableInput(playerController);
	InputComponent->BindAction("EscapeGame", IE_Pressed, this, &AMyFPS_GameModeBase::QuitGames); //게임 종료
	ApplyWidgetChange();
}

void AMyFPS_GameModeBase::ApplyWidgetChange()
{
	if (CurrentWidget != nullptr)
		CurrentWidget->RemoveFromParent();
	
	switch (EWidget)
	{

	case EWidget::Ingame:
	{
		ApplyWidget(IngameWClass, false, false);
		FInputModeGameOnly GameUI;
		playerController->SetInputMode(GameUI);
		break;
	}
	case EWidget::Inventory:
	{
		ApplyWidget(InventoryWClass, true, true);
		FInputModeGameAndUI InventoryUI;
		playerController->SetInputMode(InventoryUI);
		break;
	}
	case EWidget::Shopping:
	{
		ApplyWidget(ShoppingWClass, true, true);
		FInputModeUIOnly ShopUI;
		playerController->SetInputMode(ShopUI);
		break;
	}
	case EWidget::Loading:
	{
		ApplyWidget(LoadingWClass, false, false);
		FInputModeGameOnly LoadUI;
		playerController->SetInputMode(LoadUI);
		break;
	}
	case EWidget::Menu:
	{
		ApplyWidget(MenuWClass, true, true);
		FInputModeUIOnly MenuUI;
		playerController->SetInputMode(MenuUI);
		break;
	}
	}
}

void AMyFPS_GameModeBase::ApplyWidget(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvent)
{
	if (WidgetToApply != nullptr)
	{
		playerController->bShowMouseCursor = bShowMouseCursor;
		playerController->bEnableClickEvents = EnableClickEvent;
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AMyFPS_GameModeBase::ChangeWidget(uint8 Widget)
{
	//같은 위젯으로 변경되면 디폴트위젯 출력
	if (EWidget == Widget)
		EWidget = EWidget::Ingame;
	else
		EWidget = Widget;

		ApplyWidgetChange();
}
//상점 아이템 정보 반환
FItem AMyFPS_GameModeBase::GetShopItems(int index)
{

	return Shop_Items[index];
}
//플레이어 사망 점수 기록
void AMyFPS_GameModeBase::PlayerDead(AActor* player)
{

	bPlayerDead = true;

	CurrentPlayer = Cast<AMyCharacter>(player);
	
	if (UGameplayStatics::DoesSaveGameExist(saveString, 0))
	{
		save = UGameplayStatics::LoadGameFromSlot(saveString, 0);

		UMyScoreSave* MyScore = Cast<UMyScoreSave>(save);

		MyScore->SetScoreIndex(Score);
		if (Score >= MyScore->scoreIndex[0])
		{
			highScore = MyScore->scoreIndex[0];
		}
		UGameplayStatics::SaveGameToSlot(save, saveString, 0);

		for (int i = 0; i < 10; i++)
		{
			ScoreTop10[i] = MyScore->scoreIndex[i];
		}
	}
	else
	{
		save = UGameplayStatics::CreateSaveGameObject(ScoreClass);
		UMyScoreSave* MyScore = Cast<UMyScoreSave>(save);
		if (Score >= MyScore->scoreIndex[0])
		{
			MyScore->scoreIndex[0] = Score;
			highScore = MyScore->scoreIndex[0];
		}
		UGameplayStatics::SaveGameToSlot(save, saveString, 0);

		for (int i = 0; i < 10; i++)
		{
			ScoreTop10[i] = MyScore->scoreIndex[i];
		}
	}

	ChangeWidget(EWidget::Loading);
}

//죽은 플레이어 제거하고 새로 생성
void AMyFPS_GameModeBase::RespawnPlayer()
{
	bool bPlayerStartMoney = false;

	if (bPlayerDead == false) bPlayerStartMoney=true;

	bPlayerDead = false;
	const FVector SpawnLoc = FVector(220.f, 30.f, 120.f);
	const FRotator SpawnRot = FRotator(0.f, 0.f, 0.f);

	if(CurrentPlayer)
	CurrentPlayer->PlayerElimination();

	FActorSpawnParameters spawnparam;

	AMyCharacter* player = GetWorld()->SpawnActor<AMyCharacter>(playerClass, SpawnLoc, SpawnRot, spawnparam);

	if (bPlayerStartMoney == true) player->Money = 2000;

	playerController->Possess(player);
	

	ChangeWidget(EWidget::Ingame);
}

//스코어 보드 리셋
void AMyFPS_GameModeBase::ResetKillPoint()
{
	if (UGameplayStatics::DoesSaveGameExist(saveString, 0))
	{
		save = UGameplayStatics::LoadGameFromSlot(saveString, 0);
		UMyScoreSave* MyScore = Cast<UMyScoreSave>(save);
		MyScore->ResetScore();

		UGameplayStatics::SaveGameToSlot(save, saveString, 0);

		for (int i = 0; i < 10; i++)
		{
			ScoreTop10[i] = MyScore->scoreIndex[i];
		}
		highScore = MyScore->scoreIndex[0];
	}
	
}
//메인 메뉴로 이동
void AMyFPS_GameModeBase::QuitGames()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMap"));
}

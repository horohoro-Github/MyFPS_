// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyWeapon.h"
#include "MyButton.h"
#include "MyButtonB.h"
#include "MyNPC.h"
#include "Perception/AISense_Sight.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(GetMesh(), FName("CameraSocket"));
	CameraComp->bUsePawnControlRotation = true;
	MovableCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MovableCamera"));
	MovableCamera->SetupAttachment(GetMesh());
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(GetMesh(), FName("CameraSocket"));
	BB_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BBGun"));
	BB_Gun->SetupAttachment(GetMesh(), FName("Grid_Point"));
	BB_Gun->CastShadow = false;
	BB_Gun_Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	BB_Gun_Muzzle->SetupAttachment(BB_Gun, FName("bulletSocket"));
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(BB_Gun_Muzzle);
	Money = 0;
	
	setup_stimulus();

	HP = 100;
	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	GameMode = Cast<AMyFPS_GameModeBase>(UGameplayStatics::GetGameMode(this));
	BB_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Grid_Point"));
	
	//인벤토리의 아이템 유무
	Inventory_Check.Init(false, 10);

	//아이템칸을 기본값으로 설정
	for (int i = 0; i < 10; i++)
	{
		Inventory[i] = Default;
	}

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDead == false)
	{
		
		for (int i = 1; i < 10; i++)
		{
			if (Inventory[i].Itemtype == EDataNum::NoWeapon)
			{
				bInventorySpace = true;
				break;
			}
			if (i == 9)
			{
				bInventorySpace = false;
			}
		}
			
		if (bIngameProgress == true)	
		{
			ProgressbarVal += 0.002f;
			if (ProgressbarVal >= 1.f)
			{
				RecoveryHealth(GetItemsInfo());
			}
		}
		if (bShouldUpdateRecoil == true)	//반동
		{
			float TurnMin = (TurnValue > 0.f) ? 0.f : TurnValue;
			float LookUpMin = (-LookUpValue > 0.f) ? 0.f : -LookUpValue;

			FVector2D SetRecovery = TargetRecoveryAmount - FVector2D(TurnMin, LookUpMin);
			float XMin = (SetRecovery.X > 0.f) ? 0.f : SetRecovery.X;
			float YMin = (SetRecovery.Y > 0.f) ? 0.f : SetRecovery.Y;

			TargetRecoveryAmount = FVector2D(XMin, YMin);

			if (bResetRecoil == true)
			{
				FVector2D ChangeValueRecovery = UKismetMathLibrary::Vector2DInterpTo(TargetRecoveryAmount, FVector2D(0.f, 0.f), DeltaTime, 4.f);
				PlayerController_Recoil(TargetRecoveryAmount - ChangeValueRecovery);
				TargetRecoveryAmount = ChangeValueRecovery;
				if (UKismetMathLibrary::IsZero2D(TargetRecoveryAmount))
					bShouldUpdateRecoil = false;
			}
			else
			{
				FVector2D ChangeValueRecoil = UKismetMathLibrary::Vector2DInterpTo(TargetRecoilAmount, FVector2D(0.f, 0.f), DeltaTime, 12.f);
				PlayerController_Recoil(TargetRecoilAmount - ChangeValueRecoil);
				TargetRecoveryAmount = TargetRecoveryAmount + UKismetMathLibrary::Negated2D(TargetRecoilAmount - ChangeValueRecoil);
				TargetRecoilAmount = ChangeValueRecoil;

				if (UKismetMathLibrary::IsZero2D(TargetRecoilAmount))
					bShouldUpdateRecoil = false;
			}

		}

		if (bZoomIn == true)	//조준
		{
			CameraComp->Deactivate();
			MovableCamera->Activate();
			MovableCamera->SetWorldLocationAndRotation(UKismetMathLibrary::VInterpTo(MovableCamera->GetComponentLocation(), CurrentWeapon->ScopeCamera->GetComponentLocation(), DeltaTime, 15.f), UKismetMathLibrary::RInterpTo(MovableCamera->GetComponentRotation(), CurrentWeapon->ScopeCamera->GetComponentRotation(), DeltaTime, 15.f), false);

		}
		else
		{
			MovableCamera->SetWorldLocationAndRotation(UKismetMathLibrary::VInterpTo(MovableCamera->GetComponentLocation(), CameraComp->GetComponentTransform().GetLocation(), DeltaTime, 15.f), UKismetMathLibrary::RInterpTo(MovableCamera->GetComponentRotation(), CameraComp->GetComponentTransform().Rotator(), DeltaTime, 15.f), false);

			if (UKismetMathLibrary::NearlyEqual_TransformTransform(MovableCamera->GetComponentTransform(), CameraComp->GetComponentTransform(), 0.5f, 0.5f))
			{

				MovableCamera->Deactivate();
				CameraComp->Activate();
				bAiming = false;
			}
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("Right", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Forward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis("Lookup", this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis("ItemSlotKey", this, &AMyCharacter::SlotKey);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::JumpEnd);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AMyCharacter::OpenInventory);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMyCharacter::Interaction);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::EndFire);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMyCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomIn", IE_Released, this, &AMyCharacter::ZoomOut);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::StartReload);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::StopSprint);
	//PlayerInputComponent->BindAction("ItemSlotKey", IE_Pressed, this, &AMyCharacter::SlotKey);
	//if(GetInputAxisKeyValue(,))
	//if(IsInputKey)
}
//AIPerception 감지
void AMyCharacter::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

//캐릭터 이동
void AMyCharacter::MoveRight(float Value)
{
	if (bPressed == true)
	{
		
		if (bSprint == true && bReloading==false)
		{
			GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, 600.f, GetWorld()->GetDeltaSeconds(), 2.f);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, 300.f, GetWorld()->GetDeltaSeconds(), 2.f);
		}
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMyCharacter::MoveForward(float Value)
{
	if (bPressed == true)
	{
		if (bSprint == true&& bReloading==false)
		{
			GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, 600.f, GetWorld()->GetDeltaSeconds(), 2.f);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = UKismetMathLibrary::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, 300.f, GetWorld()->GetDeltaSeconds(), 2.f);
		}
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}


//점프
void AMyCharacter::JumpStart()
{
	if (bReloading == false && bAiming==false)
	{
		bPressedJump = true;
		bJumping = true;
		if (!GetCharacterMovement()->IsFalling())
		{
			PlayAnimMontage(Jump_Montage, 1.f);
		}
	}
}

void AMyCharacter::JumpEnd()
{
	bPressedJump = false;
	
}

void AMyCharacter::OpenInventory()
{
	GameMode->ChangeWidget(GameMode->EWidget::Inventory);
}

//무기 줍기, 문열기, 상점 이용
void AMyCharacter::Interaction()
{
	FHitResult hit;
	FVector StartTrace;
	FVector EndTrace;
	FCollisionShape CollisionShape;
	CollisionShape.LineShape;
	
	if (CameraComp->IsActive() == true)
	{
		StartTrace = CameraComp->GetComponentLocation();
		EndTrace = CameraComp->GetForwardVector() * 500.f + StartTrace;
	}
	else
	{
		StartTrace = MovableCamera->GetComponentLocation();
		EndTrace = MovableCamera->GetForwardVector() * 500.f + StartTrace;
	}
	
	
	GetWorld()->SweepSingleByChannel(hit, StartTrace, EndTrace, FQuat::FQuat(), ECC_Visibility, CollisionShape);

	if (AMyWeapon* weapon = Cast<AMyWeapon>(hit.Actor.Get()))
	{
		if(weapon->bPlayerIsNearbyThis==true) weapon->Pickup();
	}
	if (AMyButton* button = Cast<AMyButton>(hit.Actor.Get()))
	{
		if (button->bPlayerIsNearbyThis == true) button->Click();
	}
	if (AMyNPC* npc = Cast<AMyNPC>(hit.Actor.Get()))
	{
		if (npc->bPlayerIsNearbyThis == true) npc->OpenShop();
	}
	if (AMyButtonB* buttonB = Cast<AMyButtonB>(hit.Actor.Get()))
	{
		if (buttonB->bPlayerIsNearbyThis == true) buttonB->Click();
	}
}
void AMyCharacter::StartFire()
{
	bTrigger = true;

	OnFire();
	
}

void AMyCharacter::EndFire()
{
	bTrigger = false;

}
//총 발사
void AMyCharacter::OnFire()
{
	if (bShot==true && bReloading==false && bJumping==false && bSwapping==false)
	{
		if (Projectileclass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				//CurrentWeapon은 현재 장착중인 무기의 MyWeapon 클래스
				if (CurrentWeapon != NULL)
				{
					CurrentWeapon->Fire(this);
					if (bTrigger == true)
						GetWorld()->GetTimerManager().SetTimer(handle, this, &AMyCharacter::OnFire, CurrentWeapon->weapondata.AttackSpeed / 20, false);
				}
				else
				{
					const FRotator SpawnRotation = GetControlRotation();
					const FVector SpawnLocation = BB_Gun_Muzzle->GetComponentLocation();
					
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					World->SpawnActor<AMyProjectile>(Projectileclass, SpawnLocation, SpawnRotation, ActorSpawnParams);


					if (FireAnimMontage != nullptr && bDead == false)
					{
						UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
						if (AnimIns != nullptr)
						{

							UGameplayStatics::SpawnSoundAttached(FireSound, BB_Gun_Muzzle, FName("FireSound"));
							AnimIns->Montage_Play(FireAnimMontage, 1.f);
						}

					}

				}
			}

		}

	}
}

void AMyCharacter::ZoomIn()
{
	if (CurrentWeapon && bShot==true && bJumping==false && bReloading==false && bSwapping==false && CurrentWeapon->bAimable==true)
	{
		bZoomIn = true;
		bAiming = true;
	}
}

void AMyCharacter::ZoomOut()
{
	if (CurrentWeapon)
	{
		bZoomIn = false;
	}
	
}

void AMyCharacter::SlotKey(float Value)
{
	//BindAxis 값 각각 인벤토리 넘버에 대응. 
	if (bInputKeyDoOnce == true && Value!=0)
	{
		FTimerHandle Slotcooldown;
		int value = (int)Value;
		bInputKeyDoOnce = false;
		if (value == 10)
		{
			UseItem(Default, 0);
		}
		else
		{
			UseItem(Default, value);
		}
		GetWorld()->GetTimerManager().SetTimer(Slotcooldown, FTimerDelegate::CreateLambda([&]()
			{
				bInputKeyDoOnce = true;
			}), 0.2f, false);
	}
}
//장전 함수
void AMyCharacter::Reload()
{
	if (bSwapping == false)
	{
		switch (Inventory[0].Itemtype)
		{
		case EDataNum::Rifle:
		{
			if (Inventory[0].CurrentAmmo < Inventory[0].MaxAmmo)
			{
				int Decrease = Inventory[0].MaxAmmo - Inventory[0].CurrentAmmo;

				if (Rifle >= Decrease)
				{
					Inventory[0].CurrentAmmo = Inventory[0].MaxAmmo;
					Rifle -= Decrease;
					bReloading = false;
				}
				else
				{
					if (Rifle > 0)
					{
						Inventory[0].CurrentAmmo += Rifle;
						Rifle = 0;
						bReloading = false;
					}
				}
			}
			break;
		}
		case EDataNum::Shotgun:
		{
			if (Inventory[0].CurrentAmmo < Inventory[0].MaxAmmo)
			{
				int Decrease = Inventory[0].MaxAmmo - Inventory[0].CurrentAmmo;
				if (Shotgun >= Decrease)
				{
					Inventory[0].CurrentAmmo = Inventory[0].MaxAmmo;
					Shotgun -= Decrease;
					bReloading = false;
				}
				else
				{
					if (Shotgun > 0)
					{
						Inventory[0].CurrentAmmo += Shotgun;
						Shotgun = 0;
						bReloading = false;
					}
				}
			}
			break;
		}
		case EDataNum::Rocket:
		{
			if (Inventory[0].CurrentAmmo < Inventory[0].MaxAmmo)
			{
				int Decrease = Inventory[0].MaxAmmo - Inventory[0].CurrentAmmo;
				if (Rocket >= Decrease)
				{
					Inventory[0].CurrentAmmo = Inventory[0].MaxAmmo;
					Rocket -= Decrease;
					bReloading = false;
				}
				else
				{
					if (Rocket > 0)
					{
						Inventory[0].CurrentAmmo += Rocket;
						Rocket = 0;
						bReloading = false;
					}
				}
			}
			break;
		}
		default:
			break;
		}
	}
}

void AMyCharacter::StartSprint()
{
	bSprint = true;
	ZoomOut();
}

void AMyCharacter::StopSprint()
{
	bSprint = false;
}

void AMyCharacter::BeAttacked(int Damage)
{
	HP -= Damage;
	if (HP <= 0)Die();
}
//플레이어 사망 및 아이템 전부 드랍
void AMyCharacter::Die()
{
	if (bDead == false)
	{
		bDead = true;
		bPressed = false;
		BB_Gun->SetVisibility(false);
		if (SComp != nullptr && PComp != nullptr)
		{
			SComp->SetActive(false);
			PComp->SetActive(false);
		}
		MovableCamera->Deactivate();
		CameraComp->Activate();
		CameraComp->bUsePawnControlRotation = false;
		
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		
		UAnimInstance* animIns = GetMesh()->GetAnimInstance();
		if (animIns)
			animIns->Montage_Play(Death_Montage, 1.f);

		FActorSpawnParameters fspawn;
		for (int i = 0; i < 10; i++)
		{
			if (Inventory_Check[i] == true)
			{

				DropItem(Inventory[i], i);
				
			}
		}

		GameMode->PlayerDead(this);
	
	}
}

//무기 줍기
bool AMyCharacter::PickupWeapon(AMyWeapon* ThisWeapon)
{	
	//얻은 아이템 인벤토리에 적재
	for (int i = 1; i < 10; i++)
	{
		if (Inventory_Check[i]==false)
		{
			Inventory_Check[i] = true;
			Inventory[i] = ThisWeapon->weapondata;
			
			return true;
			//break;
		}
	}
	return false;
}

//장비 장착
void AMyCharacter::ChangeWeapon(FItem Item_Weapon, bool bWeapon, int Index)
{


	if (bWeapon == true)
	{
		if (CurrentWeapon!=nullptr)
			GetWorld()->DestroyActor(CurrentWeapon);
	
		bAttachWeapon = true;
		FActorSpawnParameters SpawnParams;
		CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(Item_Weapon.ItemClass, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnParams);
		CurrentWeapon->SK_Gun->SetSimulatePhysics(false);
		CurrentWeapon->SetActorEnableCollision(false);
		CurrentWeapon->SetActorScale3D(Item_Weapon.ItemSize);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrentWeapon->weapondata.EquipSocketName);
		CurrentWeapon->SpotLight->SetVisibility(true);
		USoundCue* sound = CurrentWeapon->weapondata.SwapSound;
		UGameplayStatics::SpawnSoundAttached(sound, GetMesh(), FName("None"), GetMesh()->GetComponentLocation(), EAttachLocation::KeepWorldPosition);
		//bSwapping = false;

		BB_Gun->SetVisibility(false);
		SpotLight->SetVisibility(false);
	}
	else
	{
		if (CurrentWeapon != nullptr)
			GetWorld()->DestroyActor(CurrentWeapon);

		CurrentWeapon = NULL;
		//bSwapping = false;
		BB_Gun->SetVisibility(true);
		SpotLight->SetVisibility(true);
	}
}

//플레이어 제거
void AMyCharacter::PlayerElimination()
{
	bDead = false;
	const FVector SpawnLoc = FVector(220.f, 30.f, 120.f);
	const FRotator SpawnRot = FRotator(0.f, 0.f, 0.f);
	SetActorLocation(SpawnLoc);
	Destroy();
}

void AMyCharacter::Recoil()
{
	bResetRecoil = false;
	FiringCount += 1.f;

	if(CurrentWeapon->weapondata.Recoil_Curve)
	TargetRecoilAmount = TargetRecoilAmount + FVector2D(CurrentWeapon->weapondata.Recoil_Curve->GetVectorValue(FiringCount).X, CurrentWeapon->weapondata.Recoil_Curve->GetVectorValue(FiringCount).Y);
	
	bShouldUpdateRecoil = true;
}

void AMyCharacter::ResetRecoil()
{
	bResetRecoil = true;
	FiringCount = 0;
	bShouldUpdateRecoil = true;
}

void AMyCharacter::PlayerController_Recoil(FVector2D Value)
{
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y*-1.f);
}

void AMyCharacter::LookUp(float Value)
{
	LookUpValue = Value;
	AddControllerPitchInput(LookUpValue);
}

void AMyCharacter::Turn(float Value)
{
	TurnValue = Value;
	AddControllerYawInput(TurnValue);

}

//체력 회복
void AMyCharacter::RecoveryHealth(FItem Value)
{
	bIngameProgress = false;
	ProgressbarVal = 0.f;
	PComp->DestroyComponent();
	SComp->SetPaused(true);
	SComp->DestroyComponent();
	HP += Value.Damage;
	if (HP > 100)HP = 100;
}
FItem AMyCharacter::GetItemsInfo()
{
	return UsingItem;
}
//블루프린트에 인벤토리 정보 전달하기
FItem AMyCharacter::GetInventorySlot(int Slot)
{
	if (Slot >= 0)
	{
		if(Inventory[Slot].Itemtype!=EDataNum::NoWeapon)
		return Inventory[Slot];
		else
		{
			return Default;
		}
	}
	else
	{
		//상점 정보
		Slot = abs(Slot);
		return GameMode->GetShopItems(Slot);
	}
	
}

//인벤토리의 아이템 사용
bool AMyCharacter::UseItem(FItem Items, int Index) //매개변수 Items는 사용중이지 않음
{
	bool bRightWork = false;
	FItem temp;
	if (bReloading == false && bAiming==false)
	{
		if (Index != 0)
		{
			switch (Inventory[Index].Itemtype)
			{
			case EDataNum::NoWeapon:
			{
				break;
			}
			case EDataNum::Heal:
			{
				if (bIngameProgress == false)
				{

					bRightWork = true;
					GameMode->ChangeWidget(GameMode->EWidget::Ingame);
					bIngameProgress = true;
					UsingItem = Inventory[Index];

					Inventory_Check[Index] = false;
					Inventory[Index] = Default;
					if (P_Repair != nullptr && S_Repair != nullptr)
					{
						PComp = UGameplayStatics::SpawnEmitterAttached(P_Repair, GetMesh(), FName("head"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), EAttachLocation::SnapToTargetIncludingScale);
						SComp = UGameplayStatics::SpawnSoundAttached(S_Repair, GetMesh(), FName("head"), FVector(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset);
					}
				}
				break;
			}
			default:
			{
				if (Inventory[0].Itemtype == EDataNum::NoWeapon)
				{
					Inventory_Check[Index] = false;
					Inventory_Check[0] = true;
					temp = Inventory[0];
					Inventory[0] = Inventory[Index];
					Inventory[Index] = temp;
					bRightWork = true;
					UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
					AnimIns->Montage_Play(SwapWeaponMontageTypeA, 1.f);
					bSwapping = true;
					//ChangeWeapon(Inventory[0], true, 0);
				}
				else
				{
					for (int i = 1; i <= Index; i++)
					{
						if (Inventory_Check[i] == false && i != Index)
						{
							Inventory_Check[i] = true;
							temp = Inventory[0];
							Inventory[0] = Inventory[Index];
							Inventory[i] = temp;
							Inventory[Index] = Default;
							Inventory_Check[Index] = false;
							bRightWork = true;
							UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
							AnimIns->Montage_Play(SwapWeaponMontageTypeA, 1.f);
							bSwapping = true;

							//ChangeWeapon(Inventory[0], true, 0);
							break;
						}
						if (i == Index)
						{
							Inventory_Check[0] = true;
							Inventory_Check[Index] = true;
							temp = Inventory[0];
							Inventory[0] = Inventory[Index];
							Inventory[Index] = temp;
							bRightWork = true;
							UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
							AnimIns->Montage_Play(SwapWeaponMontageTypeA, 1.f);
							bSwapping = true;
							//ChangeWeapon(Inventory[0], true, 0);
							break;
						}
					}
				}

		    }
			}
		
		}
		else if (Index == 0 && Inventory[0].Itemtype != EDataNum::NoWeapon)
		{

			for (int i = 1; i < 10; i++)
			{
				if (Inventory_Check[i] == false)
				{
					Inventory_Check[i] = true;
					temp = Inventory[i];
					Inventory[i] = Inventory[0];
					Inventory[0] = temp;
					//ChangeWeapon(Inventory[0], false, 0);
					bRightWork = true;
					UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
					AnimIns->Montage_Play(SwapWeaponMontageTypeB, 1.f);
					bSwapping = true;

					break;
				}
			}

		}
	}
	return bRightWork;
}

//아이템의 판매, 구매
void AMyCharacter::SoldBuyItem(int Index)
{
	//판매
	if (Index > 0)
	{
		
		Money += Inventory[Index].Price/10;
		
		Inventory[Index]= Default;
		
		Inventory_Check[Index] = false;
		
	}//구매
	else
	{
		
		Index = abs(Index);
		

		if (Money >= GameMode->Shop_Items[Index].Price)
		{
			

			if (GameMode->Shop_Items[Index].Itemtype == EDataNum::Bullet)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GameMode->Shop_Items[Index].SwapSound, GetActorLocation());
				Money -= GameMode->Shop_Items[Index].Price;
				Index %= 10;
				switch (Index)
				{
				case 0:
				{
					Rifle += GameMode->Shop_Items[10 + Index].CurrentAmmo;
					
					break;
				}
				case 1:
				{
					Shotgun += GameMode->Shop_Items[10 + Index].CurrentAmmo;
					break;
				}
				case 2:
				{
					Rocket += GameMode->Shop_Items[10 + Index].CurrentAmmo;
					break;
				}
				}
			
			}
			else
			{
				for (int i = 1; i < 10; i++)
				{
					if (Inventory_Check[i] == false)
					{
						Inventory_Check[i] = true;
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GameMode->Shop_Items[Index].SwapSound, GetActorLocation());
						Money -= GameMode->Shop_Items[Index].Price;
						Inventory[i] = GameMode->GetShopItems(Index);
						
						break;
					}

				}
			}
		}
		
	}
}
//인벤토리 아이템 버리기
bool AMyCharacter::DropItem(FItem items, int Index) //매개변수 items는 현재 사용중이지 않음
{
	FActorSpawnParameters FSpawn;
	if (Index == 0)
	{
		if(Inventory[0].Itemtype==EDataNum::Rifle || Inventory[0].Itemtype == EDataNum::Shotgun || Inventory[0].Itemtype == EDataNum::Rocket)
		{
		
			AMyWeapon* weapon = GetWorld()->SpawnActor<AMyWeapon>(Inventory[0].ItemClass, GetActorLocation(), GetActorRotation(), FSpawn);
			weapon->weapondata = Inventory[0];
		
			Inventory[0] = Default;
			ChangeWeapon(Default, false, 0);

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		switch (Inventory[Index].Itemtype)
		{
		case EDataNum::NoWeapon:
		{
			return false;
		}
			
		case EDataNum::Heal:
		{
			Inventory[Index] = Default;
			Inventory_Check[Index] = false;

			return true;
		}
		default:
		{
			if (Index > 0 && Inventory_Check[Index]==true)
			{
			
				AMyWeapon* weapon = GetWorld()->SpawnActor<AMyWeapon>(Inventory[Index].ItemClass, GetActorLocation(), GetActorRotation(), FSpawn);
				weapon->weapondata = Inventory[Index];

				Inventory[Index] = Default;
				Inventory_Check[Index] = false;


				return true;
			}
			else
			{
				return false;
			}
		}
		}
	}
	
}

//남은 총알 반환
int AMyCharacter::GetRemainingBullets()
{
	if (CurrentWeapon != nullptr)
	{
		switch (Inventory[0].Itemtype)
		{
		case EDataNum::Rifle:
		{
			return Rifle;
			break;
		}
		case EDataNum::Shotgun:
		{
			return Shotgun;
			break;
		}
		case EDataNum::Rocket:
		{
			return Rocket;
			break;
		}
		}
		return -1;
	}
	else
	{
		return -1;
	}
	
}


void AMyCharacter::StartReload()
{
	if (bShot == true && bReloading==false && bSwapping==false)
	{
		UAnimInstance* AnimIns = GetMesh()->GetAnimInstance();
		switch (Inventory[0].Itemtype)
		{
		case EDataNum::Rifle:
		{
			if (Inventory[0].CurrentAmmo < Inventory[0].MaxAmmo && Rifle>=1)
			{
				bReloading = true;
				AnimIns->Montage_Play(ReloadAnimMontage, 1.f);
			
		
			}
			break;
		}
		case EDataNum::Shotgun:
		{
			if (Inventory[0].CurrentAmmo < Inventory[0].MaxAmmo && Shotgun>=1)
			{
				bReloading = true;
				AnimIns->Montage_Play(ReloadAnimMontage, 1.f);
				
			}
			break;
		}
		case EDataNum::Rocket:
		{
			if (Inventory[0].CurrentAmmo < Inventory[0].MaxAmmo && Rocket>=1)
			{
				bReloading = true;
				AnimIns->Montage_Play(ReloadAnimMontage, 1.f);
			
			}
			break;
		}
		default:
			break;
		}
	}
}


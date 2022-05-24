// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
#include "MyAIEnemy.h"
// Sets default values
AMyWeapon::AMyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GUN"));
	SK_Gun->SetNotifyRigidBodyCollision(true);
	SK_Gun->SetUseCCD(true);
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(SK_Gun, FName("Muzzle"));
	S_Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	S_Collision->SetupAttachment(SK_Gun);
	S_Collision->SetGenerateOverlapEvents(true);
	S_Collision->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::BeginOverlapWeapon);
	S_Collision->OnComponentEndOverlap.AddDynamic(this, &AMyWeapon::EndOverlapWeapon);
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(SK_Gun, FName("Muzzle"));
	SpotLight->SetVisibility(false);
	SpotLight->AttenuationRadius = 5000.f;
	SpotLight->Intensity = 50000.f;
	ScopeCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Scope"));
	ScopeCamera->SetupAttachment(SK_Gun, FName("ScopeSocket"));
	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("widget"));
	widget->SetupAttachment(SK_Gun);
	widget->SetVisibility(false);

	SK_Gun->CastShadow = false;

}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();

	SK_Gun->OnComponentHit.AddDynamic(this, &AMyWeapon::HitEvent);
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bWeapon == true)
	{
		S_Collision->DestroyComponent();
		widget->DestroyComponent();
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void AMyWeapon::Pickup()
{
	AMyCharacter* player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (bEquip == false && player->PickupWeapon(this) == true && bPlayerIsNearbyThis==true)
	{
		bEquip = true;
	
		SK_Gun->SetSimulatePhysics(false);
		ECollisionEnabled::NoCollision;
		//DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		S_Collision->SetGenerateOverlapEvents(false);
		
		Destroy();
	}
}
//무기타입별 총알 발사 
void AMyWeapon::Fire(APawn* Shooter)
{
	if (bCurrentFired == false && ProjectileClass!=nullptr)
	{

		UWorld* const World = GetWorld();

		if (AMyAIEnemy* enemy = Cast<AMyAIEnemy>(Shooter))
		{
			const FRotator SpawnRotation = Shooter->GetControlRotation();
			const FVector SpawnLocation = Muzzle->GetComponentLocation();
			const FRotator Rot = Muzzle->GetComponentRotation();

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AMyEnemyProjectile* enemyProjectile = World->SpawnActor<AMyEnemyProjectile>(EnemyProjectileClass, SpawnLocation, Rot, ActorSpawnParams);
			if (enemyProjectile != nullptr)
				enemyProjectile->SetBulletInfo(weapondata.Damage);
			if (FireParticle != nullptr && FireSound != nullptr)
			{
				UParticleSystemComponent* particle = UGameplayStatics::SpawnEmitterAttached(FireParticle, Muzzle, FName("None"), FVector(0.f), FRotator(90.f), FVector(0.5f));
				UGameplayStatics::SpawnSoundAttached(FireSound, Muzzle, FName("FireSound"), FVector(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset);
			}
		}

		if (AMyCharacter* player = Cast<AMyCharacter>(Shooter))
		{
			if (player->Inventory[0].CurrentAmmo > 0)
			{
				GetWorld()->GetTimerManager().ClearTimer(RecoilHandle);
				player->Inventory[0].CurrentAmmo -= 1;
				
				switch (weapondata.Itemtype)
				{
				case EDataNum::NoWeapon:
					break;

				case EDataNum::Rifle:
				{

					const FRotator SpawnRotation = Shooter->GetControlRotation();
					const FVector SpawnLocation = Muzzle->GetComponentLocation();
					const FRotator Rot = Muzzle->GetComponentRotation();
					FActorSpawnParameters ActorSpawnParams;

					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					break;
					
				}
				case EDataNum::Shotgun:
				{
					for (int i = 0; i < 6; i++)
					{
						float randomX = FMath::RandRange(-3.f, 3.f);
						float randomY = FMath::RandRange(-3.f, 3.f);
						float randomZ = FMath::RandRange(-3.f, 3.f);


						const FRotator SpawnRotation = Shooter->GetControlRotation();
						const FVector SpawnLocation = Muzzle->GetComponentLocation();
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation + FRotator(randomX, randomY, randomZ), ActorSpawnParams);
					}
					break;
				}
				case EDataNum::Rocket:
				{
					const FRotator SpawnRotation = Shooter->GetControlRotation();
					const FVector SpawnLocation = Muzzle->GetComponentLocation();
					const FRotator Rot = Muzzle->GetComponentRotation();
					FActorSpawnParameters ActorSpawnParams;

					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					World->SpawnActor<AMyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					break;
				}
				}

			if (FireParticle != nullptr && FireSound != nullptr)
			{
				UParticleSystemComponent* particle = UGameplayStatics::SpawnEmitterAttached(FireParticle, Muzzle, FName("None"), FVector(0.f), FRotator(90.f), FVector(0.5f));

				UGameplayStatics::SpawnSoundAttached(FireSound, Muzzle, FName("FireSound"), FVector(0.f, 0.f, 0.f), EAttachLocation::KeepRelativeOffset);

				if (bSilence == false)
					UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.f, this, 0.f, bb_keys::noise_tag);

				UAnimInstance* AnimIns = player->GetMesh()->GetAnimInstance();
				if (PlayerShotMontage != nullptr && player->bDead == false)
					AnimIns->Montage_Play(PlayerShotMontage, 1.f);
				player->Recoil();

				FTimerDelegate recovDelegate = FTimerDelegate::CreateUObject(this, &AMyWeapon::SendResetRecoil, player);

				GetWorld()->GetTimerManager().SetTimer(RecoilHandle, recovDelegate, weapondata.Recoil_RecoveryTimer, false);

				bCurrentFired = true;
				if (player->bDead == false)
					GetWorld()->GetTimerManager().SetTimer(Gunhandle, this, &AMyWeapon::CoolDown, weapondata.AttackSpeed, false);
			}
		}		
	}
		
}

}

void AMyWeapon::CoolDown()
{
	bCurrentFired = false;
}
//총기 반동
void AMyWeapon::SendResetRecoil(AMyCharacter* player)
{
	player->ResetRecoil();
}

//무기가 버려졌을때 소리
void AMyWeapon::HitEvent(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	FTimerHandle cooldown;
	
	if (OtherActor != nullptr && bDoOnce == false)
	{
		bDoOnce = true;
		if(DropSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DropSound, Hit.Location);
		
		GetWorld()->GetTimerManager().SetTimer(cooldown, FTimerDelegate::CreateLambda([&]()
			{
				bDoOnce = false;
			}), 0.5f, false);
	}
	
}

//무기 가까이서 G키를 입력시 무기 획득
void AMyWeapon::BeginOverlapWeapon(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		
		widget->SetVisibility(true);
		bPlayerIsNearbyThis = true;
		/*
		AMyCharacter* player = Cast<AMyCharacter>(OtherActor);
		
		if (player != nullptr && player->bInventorySpace==true)
		{
			EnableInput(UGameplayStatics::GetPlayerController(this, 0));
			InputComponent->BindAction("Interaction", IE_Pressed, this, &AMyWeapon::Pickup);
		}
		*/
	}
}

void AMyWeapon::EndOverlapWeapon(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		widget->SetVisibility(false);
		bPlayerIsNearbyThis = false;
		//DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	}
}


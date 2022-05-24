#pragma once

#include "Curves/CurveVector.h"
#include "MyWeaponDataTable.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class EDataNum : uint8
{
	NoWeapon,
	Rifle,
	Shotgun,
	Rocket,
	Heal,
	Bullet
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()
public:
	//무기의 세부사항들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDataNum Itemtype;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName EquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* SwapSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ItemSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveVector* Recoil_Curve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Recoil_RecoveryTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Price;
	
};

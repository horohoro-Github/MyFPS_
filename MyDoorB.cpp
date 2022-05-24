// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDoorB.h"
#include "MyButtonB.h"
// Sets default values
AMyDoorB::AMyDoorB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SMDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	RootComponent = SMDoor;
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComp->SetupAttachment(SMDoor);
}

// Called every frame
void AMyDoorB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurLocation = GetActorLocation();

	if (bOpenDoor == false)
	{
		
		if (CurLocation.Z >= CloseLocation.Z)
		{
			CurLocation.Z -= 1.f;
			SetActorLocation(CurLocation);
		}
		else
		{
			
			if (bDoorTypePitch == true)
			{
				if (CurLocation.Y > CloseLocation.Y)
				{
					CurLocation.Y -= 1.f;
					SetActorLocation(CurLocation);
					if (CurLocation.Y <= CloseLocation.Y)
					{
						AudioComp->SetPaused(true);
						Execute_OnConnectButtonDoor(button, this);
					}
				}
				if (CurLocation.Y < CloseLocation.Y)
				{
					CurLocation.Y += 1.f;
					SetActorLocation(CurLocation);
					if (CurLocation.Y >= CloseLocation.Y)
					{
						AudioComp->SetPaused(true);
						Execute_OnConnectButtonDoor(button, this);
					}
				}
			}
			else
			{
				if (CurLocation.X > CloseLocation.X)
				{
					CurLocation.X -= 1.f;
					SetActorLocation(CurLocation);
					if (CurLocation.X <= CloseLocation.X)
					{
						AudioComp->SetPaused(true);
						Execute_OnConnectButtonDoor(button, this);
					}
				}
				if (CurLocation.X < CloseLocation.X)
				{
					CurLocation.X += 1.f;
					SetActorLocation(CurLocation);
					if (CurLocation.X >= CloseLocation.X)
					{
						AudioComp->SetPaused(true);
						Execute_OnConnectButtonDoor(button, this);
					}
				}
			}

		}
	}
	else
	{
			if (bDoorTypePitch == true)
			{
				if (CurLocation.Y > OpenLocation.Y || CurLocation.Y < OpenLocation.Y)
				{
					if (CurLocation.Y > OpenLocation.Y)
					{
						CurLocation.Y -= 1.f;
						SetActorLocation(CurLocation);

					}
					if (CurLocation.Y < OpenLocation.Y)
					{
						CurLocation.Y += 1.f;
						SetActorLocation(CurLocation);

					}
				}
				else
				{
					if (CurLocation.Z < OpenLocation.Z)
					{
						CurLocation.Z += 1.f;
						SetActorLocation(CurLocation);
						if (CurLocation.Z >= OpenLocation.Z)
						{
							AudioComp->SetPaused(true);
							Execute_OnConnectButtonDoor(button, this);
						}
					}
				}
			}
			else
			{
				if (CurLocation.X > OpenLocation.X || CurLocation.X < OpenLocation.X)
				{
					if (CurLocation.X > OpenLocation.X)
					{
						CurLocation.X -=1.f;
						SetActorLocation(CurLocation);
					}
					if (CurLocation.X < OpenLocation.X)
					{
						CurLocation.X += 1.f;
						SetActorLocation(CurLocation);
					}
				}
				else
				{
					if (CurLocation.Z < OpenLocation.Z)
					{
						CurLocation.Z += 1.f;
						SetActorLocation(CurLocation);
						if (CurLocation.Z >= OpenLocation.Z)
						{
							AudioComp->SetPaused(true);
							Execute_OnConnectButtonDoor(button, this);
						}
					}
				}

			}
	}
}
/*
void AMyDoorB::OnClicked_Implementation()
{
	
	if (bOpenDoor == true)
	{
		bOpenDoor = false;
	}
	else
	{
		bOpenDoor = true;
	}
	AudioComp = UGameplayStatics::SpawnSoundAttached(S_Open, AudioComp, FName("Door"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	
}
*/
void AMyDoorB::OnConnectButtonDoor_Implementation(AActor* actor)
{
	button = actor;

	if (bOpenDoor == true)
	{
		bOpenDoor = false;
	}
	else
	{
		bOpenDoor = true;
	}
	AudioComp = UGameplayStatics::SpawnSoundAttached(S_Open, AudioComp, FName("Door"), FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
}


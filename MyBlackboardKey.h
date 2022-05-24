// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

//bb_keys�� BTTaskNode�� �� ���޿� ���
namespace bb_keys
{
	TCHAR const* const Target_Location = TEXT("TargetLocation");
	TCHAR const* const can_see_player = TEXT("CanSeePlayer");
	TCHAR const* const Enemy = TEXT("Enemy");
	TCHAR const* const killed_player = TEXT("KilledPlayer");

	//������ �±�
	TCHAR const* const noise_tag = TEXT("Noise");
	TCHAR const* const hearing_sound = TEXT("HearingSound");
}
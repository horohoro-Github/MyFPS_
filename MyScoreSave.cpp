// Fill out your copyright notice in the Description page of Project Settings.


#include "MyScoreSave.h"
#include "Engine.h"

UMyScoreSave::UMyScoreSave()
{

}
//점수 내림차순 정렬
void UMyScoreSave::SetScoreIndex(int index)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = i+1; j < 10; j++)
		{
			if (scoreIndex[i] < scoreIndex[j])
			{
				int temp = scoreIndex[i];
				scoreIndex[i] = scoreIndex[j];
				scoreIndex[j] = temp;
			}
		}
	}
	
	for (int i = 0; i < 10; i++)
	{
		if (index > scoreIndex[i])
		{
			
			for (int j = 9; j >= i+1; j--)
			{
				scoreIndex[j] = scoreIndex[j - 1];
			}

			scoreIndex[i] = index;

			break;
		}
	}
	
}
//점수 초기화
void UMyScoreSave::ResetScore()
{
	for (int i = 0; i < 10; i++)
	{
		scoreIndex[i] = 0;
	}
}

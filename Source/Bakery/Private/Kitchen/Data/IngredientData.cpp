// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/Data/IngredientData.h"
#include "UObject/ObjectSaveContext.h"

#include "Kitchen/KitchenDefines.h"

UIngredientData::UIngredientData()
{
	
}

void UIngredientData::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// 에셋 저장 시 재료 이름을 에셋의 파일명으로 지정
	Name = GetFName();
}

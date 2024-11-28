// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IngredientMeshData.generated.h"

/**
 * 
 */
UCLASS()
class BAKERY_API UIngredientMeshData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	bool IsUsingStaticMesh() const { return bIsUsingStaticMesh; }

	UStaticMesh* GetBottomStaticMesh() const { return BottomStaticMesh; }
	UStaticMesh* GetBodyStaticMesh() const { return BodyStaticMesh; }
	UStaticMesh* GetLidStaticMesh() const { return LidStaticMesh; }

	USkeletalMesh* GetBottomSkeletalMesh() const { return BottomSkeletalMesh; }
	USkeletalMesh* GetBodySkeletalMesh() const { return BodySkeletalMesh; }
	USkeletalMesh* GetLidSkeletalMesh() const { return LidSkeletalMesh; }

private:
	// true일 경우, Static Mesh Component를 사용
	// false일 경우, Skeletal Mesh Component를 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient", meta = (AllowPrivateAccess = "true"))
	bool bIsUsingStaticMesh = true;

	/*
	 * Static Mesh 버전
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient|StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BottomStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient|StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* BodyStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient|StaticMesh", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* LidStaticMesh;

	/*
	 * Skeletal Mesh 버전
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient|SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* BottomSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient|SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* BodySkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ingredient|SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* LidSkeletalMesh;
};

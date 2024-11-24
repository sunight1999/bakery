// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dish.generated.h"

class UBoxComponent;
class AIngredient;

UCLASS()
class BAKERY_API ADish : public APawn
{
	GENERATED_BODY()
	
public:	
	ADish();

	bool IsClean() const { return bIsClean; }
	bool IsSettable(AIngredient* Ingredient);
	bool SetDessert(AIngredient* Ingredient);
	AIngredient* PeekDessert();

	void Eat();
	void Wash();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category="Kitchen|Dish")
	UStaticMeshComponent* DishMesh;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Dish")
	float DishThickness = 10.f;

	UPROPERTY(EditAnywhere, Category = "Kitchen|Dish")
	int HandWashingTime = 5;
	
	AIngredient* Dessert = nullptr;

	bool bIsClean = true;
	int CurrentWashingTime = 0;
};

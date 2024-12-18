// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "General/Tools/Spawner.h"
#include "CustomerSpawner.generated.h"

class UBoxComponent;
class AHallManager;
class URecipeSubsystem;
class UAnimInstance;

/**
 * 
 */
UCLASS()
class BAKERY_API ACustomerSpawner : public ASpawner
{
	GENERATED_BODY()
	
public:
	ACustomerSpawner();

	void PostSpawn(AActor* Actor) override;

	UFUNCTION()
	void OnDespawnBoxOverlap(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "CustomerSpawner")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category="CustomerSpawner")
	UBoxComponent* DespawnBox;

	UPROPERTY(EditAnywhere, Category = "CustomerSpawner")
	TArray<USkeletalMesh*> CustomerMeshes;

	UPROPERTY(EditAnywhere, Category = "CustomerSpawner")
	TArray<TSubclassOf<UAnimInstance>> CustomerAnimClasses;

	UPROPERTY(EditAnywhere, Category = "CustomerSpawner")
	TArray<UAnimMontage*> CustomerAnimMontages;

	AHallManager* HallManager;
	URecipeSubsystem* RecipeSubsystem;
};

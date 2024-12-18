// Fill out your copyright notice in the Description page of Project Settings.


#include "Kitchen/ExtraCountertop.h"

#include "Kitchen/Ingredient.h"
#include "Kitchen/Data/IngredientData.h"
#include "Interactions/InteractionDefines.h"
#include "Interactions/InteractorComponent.h"
#include "Interactions/GrabberComponent.h"
#include "Widgets/Menu/QuickSelectMenuWidget.h"

AExtraCountertop::AExtraCountertop()
{
	QuickSelectMenuType = EQuickSelectMenu::ExtraIngredients;
}

void AExtraCountertop::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	Super::OnEnterInteract(InteractionInfo);

	PendingGrabber = InteractionInfo.Interactor->GetGrabber();
}

void AExtraCountertop::OnExitInteract()
{
	Super::OnExitInteract();

	if (!bQuickSelectMenuOpened)
	{
		UPrimitiveComponent* Grabbed = PendingGrabber->GetGrabbed();
		AIngredient* GrabbedIngredient = nullptr;
		if (Grabbed)
		{
			GrabbedIngredient = Cast<AIngredient>(Grabbed->GetOwner());
		}

		if (GrabbedIngredient)
		{
			if (CurrentQuickSelectIndex >= ExtrasQuickMenuValues.Num())
			{
				CurrentQuickSelectIndex = 0;
			}

			const UIngredientData* MergedIngredientData = GrabbedIngredient->TryMergeIngredient(ExtrasQuickMenuValues[CurrentQuickSelectIndex]);
			if (MergedIngredientData)
			{
				GrabbedIngredient->ChangeIngredient(MergedIngredientData);
			}
		}
	}

	PendingGrabber = nullptr;
}

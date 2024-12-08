#include "Abnormality/InteractableObject/FirePlug.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"

#include "Widgets/Progress/ProgressWidget.h"
#include "Indicators/CookingStateIndicator.h"
#include "Abnormality/AbnormalityEvents/SprinklerAbnormalityEvent.h"

AFirePlug::AFirePlug()
{
	FirePlugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirePlugMesh"));
	FirePlugMesh->SetupAttachment(RootComponent);

	ProgressComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidgetComponent"));
	ProgressComponent->SetupAttachment(RootComponent);
	ProgressComponent->SetVisibility(false);

	StateIndicatorPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StateIndicatorPoint"));
	StateIndicatorPoint->SetupAttachment(RootComponent);
}

void AFirePlug::BeginPlay()
{
	ProgressComponent->InitWidget();
	ActivationProgress = Cast<UProgressWidget>(ProgressComponent->GetUserWidgetObject());

	FVector Location = StateIndicatorPoint->GetComponentLocation();
	FVector Scale = StateIndicatorPoint->GetComponentScale();
	StateIndicator = Cast<ACookingStateIndicator>(GetWorld()->SpawnActor(StateIndicatorClass));
	StateIndicator->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	StateIndicator->Initailize(Location, Scale);
	StateIndicator->SetActorRotation(StateFixedRotation);
	StateIndicator->Hide();
}

void AFirePlug::OnEnterInteract(const FInteractionInfo& InteractionInfo)
{
	
}

void AFirePlug::OnInteract(float deltatime)
{
	if (EventFlag) {
		CurrentGauge += deltatime;
		ActivationProgress->SetPercentage(CurrentGauge);

		if (CurrentGauge >= MaxGauge) {
			EventFlag = false;
			ProgressComponent->SetVisibility(false);
			StateIndicator->Hide();

			for (TActorIterator<ASprinklerAbnormalityEvent> It(GetWorld()); It; ++It)
			{
				(*It)->Finish();
				break;
			}
		}
	}
}

void AFirePlug::OnExitInteract()
{
	if (CurrentGauge < MaxGauge) {
		CurrentGauge = 0;
		ActivationProgress->SetPercentage(0);
	}
}

void AFirePlug::SetEventFlag(bool Flag)
{
	if (Flag)
	{
		StateIndicator->Show(ECookingIndicator::Target);
	}

	ProgressComponent->SetVisibility(Flag);
	EventFlag = Flag;
}

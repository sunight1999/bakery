
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"

AAbnormalityEvent::AAbnormalityEvent()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAbnormalityEvent::BeginPlay()
{
    Super::BeginPlay();
    
}

void AAbnormalityEvent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

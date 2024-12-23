
#include "Abnormality/AbnormalityEvents/AbnormalityEvent.h"
#include "EngineUtils.h"

#include "Characters/Customer.h"

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

    if (!bIsFearingAllEvent || !bIsCaused)
    {
        return;
    }

    for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
    {
        if (!(*It)->IsFeared())
        {
            (*It)->SetFeared(true);
        }
    }
}

void AAbnormalityEvent::Cause()
{
    bIsCaused = true;
}

void AAbnormalityEvent::Finish()
{
    bIsCaused = false;

    if (bIsFearingAllEvent)
    {
        for (TActorIterator<ACustomer>It(GetWorld()); It; ++It)
        {
            (*It)->SetFeared(false);
        }
    }
}

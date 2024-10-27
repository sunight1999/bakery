#include "Widgets/AbnormalitiesReport/FadeImageWidget.h"

void UFadeImageWidget::OnFadeImage()
{
    PlayAnimation(OnFadeImageAnim);
}

void UFadeImageWidget::OffFadeImage()
{
    PlayAnimation(OffFadeImageAnim);
}

void UFadeImageWidget::OnImage()
{

}

void UFadeImageWidget::OffImage()
{
}

bool UFadeImageWidget::Initialize()
{
    Super::Initialize();
    return true;
}

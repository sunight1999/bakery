#include "Widgets/AbnormalitiesReport/FadeImageWidget.h"
#include "Components/CanvasPanelSlot.h"

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
    FLinearColor Opacity = BlurImage->ColorAndOpacity;
    Opacity.A = 1.0;
    //BlurImage->SetRenderOpacity(0);
    BlurImage->SetColorAndOpacity(Opacity);
}
void UFadeImageWidget::OffImage()
{
    FLinearColor Opacity = BlurImage->ColorAndOpacity;
    Opacity.A = 0;
    //BlurImage->SetRenderOpacity(0);
    BlurImage->SetColorAndOpacity(Opacity);
}
void UFadeImageWidget::VisibleImage()
{
    SetVisibility(ESlateVisibility::Visible);  // 인스턴스를 통해 함수 호출
}
void UFadeImageWidget::HiddenImage() {
    SetVisibility(ESlateVisibility::Hidden);  // 인스턴스를 통해 함수 호출
}
void UFadeImageWidget::RandomChoiceImage(int32 Min, int32 Max)
{
    ChoiceImage = FMath::RandRange(Min, Max);
}
bool UFadeImageWidget::Initialize()
{
    Super::Initialize();
    return true;
}
FVector2D UFadeImageWidget::GetrandomPercentage(float MinPercentage, float MaxPercentage)
{
    FVector2D ViewportSize;
    if (GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }
    float RandomX = FMath::FRandRange(MinPercentage, MaxPercentage) * ViewportSize.X;
    float RandomY = FMath::FRandRange(MinPercentage, MaxPercentage) * ViewportSize.Y;

    return FVector2D(RandomX, RandomY);
}
void UFadeImageWidget::SetRandomImagePosition(float RandomPosMin, float RandomPosMax)
{
    // 1. 이 함수 잘 동작하는지 확인
    // 2. 안 되면 캔버스 바인딩해서 걔 쓰기
    FVector2D RandomPosition = GetrandomPercentage(RandomPosMin, RandomPosMax);
    //BlurImageCanvas->RenderTransform.Translation = RandomPosition;
    BlurImage->SetRenderTranslation(RandomPosition);
}
void UFadeImageWidget::SetRandomImageScale(float RandomSizeMin, float RandomSizeMax)
{
    // 3. CanvasSlot->GetSize()로 현재 캔버스 사이즈 가져오기
    // 4. 원하는 범위 내 랜덤 float 값 2개 추출 ex) .7f ~ 1.3f 범위 내 랜덤 float 2개
    // 5. 현재 캔버스 사이즈 x, y에 각각 뽑은 랜덤 float 값 곱하기
    float RandomSize = FMath::FRandRange(RandomSizeMin, RandomSizeMax);
    FVector2D ChoiceSize;
    ChoiceSize = FVector2D(RandomSize, RandomSize);
    if (BlurImage) {
        /*if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(BlurImage->Slot))
        {
            CanvasSlot->SetSize(FVector2D(ChangeSize, ChangeSize));
        }*/
        BlurImage->SetRenderScale(ChoiceSize);
    }
}

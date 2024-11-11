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
void UFadeImageWidget::SetSetting() {
    SetRandomImagePosition();
    SetRandomImageScale();
    PlayAnimation(LoopFadeImageAnim);
}
void UFadeImageWidget::OnImage()
{
    FLinearColor Opacity = BlurImage->ColorAndOpacity;
    Opacity.A = 1.0;
    BlurImage->SetColorAndOpacity(Opacity);
}
void UFadeImageWidget::OffImage()
{
    FLinearColor Opacity = BlurImage->ColorAndOpacity;
    Opacity.A = 0;
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
void UFadeImageWidget::SetRandomPosMinMax(float PosMin, float PosMax)
{
    RandomPosMin = PosMin;
    RandomPosMax = PosMax;
}
void UFadeImageWidget::SetRandomSizeMinMax(float SizeMin, float SizeMax)
{
    RandomSizeMin = SizeMin;
    RandomSizeMax = SizeMax;
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
void UFadeImageWidget::SetRandomImagePosition()
{
    if ((RandomPosMin && RandomPosMax) != 0) {
        FVector2D RandomPosition = GetrandomPercentage(RandomPosMin, RandomPosMax);
        BlurImage->SetRenderTranslation(RandomPosition);
        GetWidgetBounds();
    }
    else {
        UE_LOG(LogTemp, Display, TEXT("RandomPosMin && RandomPosMax의 값을 선언부에 지정해야합니다. Setter 사용을 권장드립니다."));
    }
}
void UFadeImageWidget::SetRandomImageScale()
{
    if((RandomSizeMin && RandomSizeMax)!=0){
        float RandomSize = FMath::FRandRange(RandomSizeMin, RandomSizeMax);
        FVector2D ChoiceSize;
        ChoiceSize = FVector2D(RandomSize, RandomSize);
        if (BlurImage) {
            BlurImage->SetRenderScale(ChoiceSize);
        }
    }
    else {
        UE_LOG(LogTemp, Display, TEXT("RandomSizeMin && RandomSizeMax의 값을 선언부에 지정해야합니다. Setter 사용을 권장드립니다."));
    }   
}
void UFadeImageWidget::GetWidgetBounds()
{
    // 위젯의 슬롯 가져오기
    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
    if (!CanvasSlot)
    {
        // CanvasSlot이 없으면 Bound를 빈 박스로 초기화
        Bound = FBox2D();
        return;
    }
    // 위치 및 크기 가져오기
    FVector2D Position = CanvasSlot->GetPosition();
    FVector2D Size = CanvasSlot->GetSize();

    // Bound 멤버 변수에 바운딩 박스 할당
    Bound = FBox2D(Position, Position + Size);
}


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
void UFadeImageWidget::LoopFadeImage()
{
    PlayAnimation(LoopFadeImageAnim);
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
FTimerHandle& UFadeImageWidget::GetLoopTimer()
{
    return FadeLoopTimer;
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
        //GetWidgetBounds();
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
void UFadeImageWidget::SetImagePos(FVector2D Pos) {
    BlurImage->SetRenderTranslation(Pos);
}
void UFadeImageWidget::SetWidgetBounds()
{
    //// 위젯의 슬롯 가져오기
    //// 위치 및 크기 가져오기
    ////FVector2D Position = BlurImage->GetRenderTransform().Translation;
    //FVector2D Position = BlurImage->GetCachedGeometry().GetAbsolutePosition();
    //FVector2D Size = BlurImage->GetDesiredSize();

    //FVector2D BoundMin = FVector2D(Position.X - (Size.X / 2), Position.Y - (Size.Y / 2));
    //FVector2D BoundMax = FVector2D(Position.X + (Size.X / 2), Position.Y + (Size.Y / 2));
    //// Bound 멤버 변수에 바운딩 박스 할당
    //Bound = FBox2D(BoundMin,BoundMax);
        // 위젯의 절대 위치와 로컬 크기 가져오기
    
    FVector2D AbsolutePosition = BlurImage->GetRenderTransform().Translation;
    FVector2D LocalSize = BlurImage->GetCachedGeometry().GetLocalSize();

    // 스케일 및 앵커 보정
    FVector2D Scale = BlurImage->GetRenderTransform().Scale;
    FVector2D AdjustedSize = LocalSize * Scale;

    // Bound 계산
    FVector2D BoundMin = AbsolutePosition - (AdjustedSize / 2);
    FVector2D BoundMax = AbsolutePosition + (AdjustedSize / 2);

    Bound = FBox2D(BoundMin, BoundMax);

    // 디버그 로그
    UE_LOG(LogTemp, Display, TEXT("Bound -> Min: (%f, %f), Max: (%f, %f)"),
        BoundMin.X, BoundMin.Y, BoundMax.X, BoundMax.Y);

    //BlurImage->SetRenderTranslation(Bound.GetCenter());
}

FBox2D UFadeImageWidget::GetWidgetBounds()
{
    return Bound;
}
FVector2D UFadeImageWidget::GetWidgetPos() {
    return BlurImage->GetRenderTransform().Translation;
}
double UFadeImageWidget::GetLoopInterval()
{
    return LoopInterval;
}


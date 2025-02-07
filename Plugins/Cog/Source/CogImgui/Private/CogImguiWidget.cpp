#include "CogImguiWidget.h"

#include "CogImguiContext.h"
#include "CogImguiInputHelper.h"
#include "Engine/GameViewportClient.h"
#include "imgui.h"
#include "SlateOptMacros.h"
#include "Widgets/SWindow.h"

//--------------------------------------------------------------------------------------------------------------------------
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCogImguiWidget::Construct(const FArguments& InArgs)
{
    Context = InArgs._Context;

    RefreshVisibility();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

//--------------------------------------------------------------------------------------------------------------------------
SCogImguiWidget::~SCogImguiWidget()
{
}

//--------------------------------------------------------------------------------------------------------------------------
void SCogImguiWidget::SetDrawData(const ImDrawData* InDrawData)
{
    DrawData = FCogImguiDrawData(InDrawData);
}

//--------------------------------------------------------------------------------------------------------------------------
void SCogImguiWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
    Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
    RefreshVisibility();
}

//--------------------------------------------------------------------------------------------------------------------------
int32 SCogImguiWidget::OnPaint(
    const FPaintArgs& Args,
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyClippingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId,
    const FWidgetStyle& WidgetStyle,
    bool bParentEnabled) const
{

    const FSlateRenderTransform Transform(FCogImguiHelper::RoundTranslation(AllottedGeometry.GetAccumulatedRenderTransform().GetTranslation() - FVector2d(DrawData.DisplayPos)));

    FSlateBrush TextureBrush;
    for (const FCogImguiDrawList& DrawList : DrawData.DrawLists)
    {
        TArray<FSlateVertex> Vertices;
        Vertices.SetNumUninitialized(DrawList.VtxBuffer.Size);
        for (int32 BufferIdx = 0; BufferIdx < Vertices.Num(); ++BufferIdx)
        {
            const ImDrawVert& Vtx = DrawList.VtxBuffer.Data[BufferIdx];
            Vertices[BufferIdx] = FSlateVertex::Make<ESlateVertexRounding::Disabled>(Transform, FCogImguiHelper::ToFVector2f(Vtx.pos), FCogImguiHelper::ToFVector2f(Vtx.uv), FVector2f::UnitVector, FCogImguiHelper::ToFColor(Vtx.col));
        }

        TArray<SlateIndex> Indices;
        Indices.SetNumUninitialized(DrawList.IdxBuffer.Size);
        for (int32 BufferIdx = 0; BufferIdx < Indices.Num(); ++BufferIdx)
        {
            Indices[BufferIdx] = DrawList.IdxBuffer.Data[BufferIdx];
        }

        for (const ImDrawCmd& DrawCmd : DrawList.CmdBuffer)
        {
            TArray VerticesSlice(Vertices.GetData() + DrawCmd.VtxOffset, Vertices.Num() - DrawCmd.VtxOffset);
            TArray IndicesSlice(Indices.GetData() + DrawCmd.IdxOffset, DrawCmd.ElemCount);

            UTexture2D* Texture = DrawCmd.GetTexID();
            if (TextureBrush.GetResourceObject() != Texture)
            {
                TextureBrush.SetResourceObject(Texture);
                if (IsValid(Texture))
                {
                    TextureBrush.ImageSize.X = Texture->GetSizeX();
                    TextureBrush.ImageSize.Y = Texture->GetSizeY();
                    TextureBrush.ImageType = ESlateBrushImageType::FullColor;
                    TextureBrush.DrawAs = ESlateBrushDrawType::Image;
                }
                else
                {
                    TextureBrush.ImageSize.X = 0;
                    TextureBrush.ImageSize.Y = 0;
                    TextureBrush.ImageType = ESlateBrushImageType::NoImage;
                    TextureBrush.DrawAs = ESlateBrushDrawType::NoDrawType;
                }
            }

            FSlateRect ClipRect(DrawCmd.ClipRect.x, DrawCmd.ClipRect.y, DrawCmd.ClipRect.z, DrawCmd.ClipRect.w);
            ClipRect = TransformRect(Transform, ClipRect);

            OutDrawElements.PushClip(FSlateClippingZone(ClipRect));
            FSlateDrawElement::MakeCustomVerts(OutDrawElements, LayerId, TextureBrush.GetRenderingResource(), VerticesSlice, IndicesSlice, nullptr, 0, 0);
            OutDrawElements.PopClip();
        }
    }

    return LayerId;
}

//--------------------------------------------------------------------------------------------------------------------------
FVector2D SCogImguiWidget::ComputeDesiredSize(float Scale) const
{
    return FVector2D::ZeroVector;
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& CharacterEvent)
{
    return Context->GetInputHandler().OnKeyChar(CharacterEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
    return HandleKeyEvent(KeyEvent, true);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
    return HandleKeyEvent(KeyEvent, false);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::HandleKeyEvent(const FKeyEvent& KeyEvent, bool Down)
{
    if (Down)
    {
        return Context->GetInputHandler().OnKeyDown(KeyEvent);
    }
    else
    {
        return Context->GetInputHandler().OnKeyUp(KeyEvent);;
    }
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnAnalogValueChanged(const FGeometry& MyGeometry, const FAnalogInputEvent& AnalogInputEvent)
{
    if (AnalogInputEvent.GetKey().IsGamepadKey())
    {
        return FReply::Unhandled();
    }

    return FReply::Handled();
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return Context->GetInputHandler().OnMouseButtonDown(MouseEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return Context->GetInputHandler().OnMouseButtonDoubleClick(MouseEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return Context->GetInputHandler().OnMouseButtonUp(MouseEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return Context->GetInputHandler().OnMouseWheel(MouseEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    return Context->GetInputHandler().OnMouseMove(MouseEvent.GetScreenSpacePosition(), MouseEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
FReply SCogImguiWidget::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& FocusEvent)
{
    return Super::OnFocusReceived(MyGeometry, FocusEvent);
}

//--------------------------------------------------------------------------------------------------------------------------
void SCogImguiWidget::RefreshVisibility()
{
    EVisibility DesiredVisiblity = EVisibility::SelfHitTestInvisible;

    if (Context->GetEnableInput())
    {
        if (Context->GetShareMouse() && Context->GetWantCaptureMouse() == false)
        {
            DesiredVisiblity = EVisibility::SelfHitTestInvisible;
        }
        else
        {
            DesiredVisiblity = EVisibility::Visible;
        }
    }
    else
    {
        DesiredVisiblity = EVisibility::SelfHitTestInvisible;
    }

    if (DesiredVisiblity != GetVisibility())
    {
        SetVisibility(DesiredVisiblity);
    }
}

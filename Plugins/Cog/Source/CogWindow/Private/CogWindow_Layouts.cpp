#include "CogWindow_Layouts.h"

#include "CogImguiInputHelper.h"
#include "CogWindowManager.h"
#include "CogWindowModuleCommands.h"
#include "InputCoreTypes.h"

//--------------------------------------------------------------------------------------------------------------------------
void FCogWindow_Layouts::Initialize()
{
    Super::Initialize();

    bHasMenu = false;
}

//--------------------------------------------------------------------------------------------------------------------------
void FCogWindow_Layouts::RenderContent()
{
    const UPlayerInput* PlayerInput = FCogImguiInputHelper::GetPlayerInput(*GetWorld());
    if (PlayerInput == nullptr)
    {
        return;
    }

    if (ImGui::MenuItem("Reset Window Layout"))
    {
        GetOwner()->ResetLayout();
    }

    ImGui::Separator();
    for (int32 i = 1; i <= 4; ++i)
    {
        RenderLoadLayoutMenuItem(PlayerInput, i);
    }

    ImGui::Separator();
    for (int32 i = 1; i <= 4; ++i)
    {
        RenderSaveLayoutMenuItem(PlayerInput, i);
    }

}

//--------------------------------------------------------------------------------------------------------------------------
void FCogWindow_Layouts::RenderLoadLayoutMenuItem(const UPlayerInput* PlayerInput, int LayoutIndex)
{
	const FString Command = FString::Printf(TEXT("%s %d"), FCogWindowModuleCommands::LoadLayoutCommand, LayoutIndex);
	const FString Shortcut = FCogImguiInputHelper::CommandToString(PlayerInput, Command);
    if (ImGui::MenuItem(TCHAR_TO_ANSI(*FString::Printf(TEXT("Load Layout %d"), LayoutIndex)), TCHAR_TO_ANSI(*Shortcut)))
    {
        GetOwner()->LoadLayout(LayoutIndex);
    }
}

//--------------------------------------------------------------------------------------------------------------------------
void FCogWindow_Layouts::RenderSaveLayoutMenuItem(const UPlayerInput* PlayerInput, int LayoutIndex)
{
	const FString Command = FString::Printf(TEXT("%s %d"), FCogWindowModuleCommands::SaveLayoutCommand, LayoutIndex);
	const FString Shortcut = FCogImguiInputHelper::CommandToString(PlayerInput, Command);
    if (ImGui::MenuItem(TCHAR_TO_ANSI(*FString::Printf(TEXT("Save Layout %d"), LayoutIndex)), TCHAR_TO_ANSI(*Shortcut)))
    {
        GetOwner()->SaveLayout(LayoutIndex);
    }
}
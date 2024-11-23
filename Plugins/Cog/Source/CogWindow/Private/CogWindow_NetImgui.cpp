#include "CogWindow_NetImgui.h"

#include "CogWindowManager.h"
#include "CogWindowWidgets.h"
#include "imgui_internal.h"

void FCogWindow_NetImgui::Initialize()
{
	FCogWindow::Initialize();
}

void FCogWindow_NetImgui::RenderHelp()
{
	ImGui::Text("Allows connection to NetImgui");
}

void FCogWindow_NetImgui::RenderContent()
{
	FCogWindow::RenderContent();

	static char Buffer[256] = "";

	ImGui::Text("Is Connected: %s", GetOwner()->GetContext().IsConnectedToNetImGui() ? "true" : "false");
	ImGui::Spacing();

	ImStrncpy(Buffer, TCHAR_TO_ANSI(*FString::FromInt(Port)), IM_ARRAYSIZE(Buffer));
	bool PortHasChanged = false;
	if (ImGui::InputText("##Port", Buffer, IM_ARRAYSIZE(Buffer)))
	{
		Port = std::atoi(Buffer);
		PortHasChanged = true;
	}
	ImGui::Spacing();
	
	ImStrncpy(Buffer, TCHAR_TO_ANSI(*Host), IM_ARRAYSIZE(Buffer));
	bool HostHasChanged = false;
	if (ImGui::InputText("##Host", Buffer, IM_ARRAYSIZE(Buffer)))
	{
		Host = Buffer;
		HostHasChanged = true;
	}

	if (FCogWindowWidgets::ButtonWithTooltip("Listen", "Starts Listening"))
	{
		GetOwner()->GetContext().Listen(Port);
	}
	ImGui::Spacing();
	if (FCogWindowWidgets::ButtonWithTooltip("Connect", "Connect to NetImGUI"))
	{
		GetOwner()->GetContext().Connect(Host, Port);
	}
	ImGui::Spacing();
	if (FCogWindowWidgets::ButtonWithTooltip("Disconnect", "Disconnect From NetImGUI"))
	{
		GetOwner()->GetContext().Disconnect();
	}
}

#include "pch.h"
#include "ImGuiWindow.h"

bool ImGuiHUDWindow::Render()
{
	ImGui::Begin(title_.c_str(), &isOpen_);

	for (std::unique_ptr<IHUDElement>& el : els_)
	{
		el->Render();
	}

	ImGui::End();

	return true;
}

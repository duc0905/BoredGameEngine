#pragma once
#include "../../pch.h"
#include <imgui.h>
#include <stdarg.h>
#include "IHUDElement.h"

class ImGuiTextEl : public IHUDElement
{
private:
	//std::function<void()> f;
	std::string text_;
	bool hide_ = false;
public:
	ImGuiTextEl(std::string content) {
		text_ = content;
	}

	void updateString(std::string text) {
		text_ = text;
	}

	void Hide() { hide_ = true; }
	void Show() { hide_ = false; }

	// Inherited via IHUDElement
	virtual void Render() override
	{
		//f();
		ImGui::Text(text_.c_str());
	}
};

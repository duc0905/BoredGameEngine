#pragma once
#include "../../pch.h"
#include "IHUDElement.h"

class ImGuiTextEl : public IHUDElement
{
private:
	std::function<void()> f;
	std::string text_;
	bool hide_ = false;
public:
	template <typename ...Params>
	ImGuiTextEl(const std::string& text, Params&&... args)
		: text_(text) {
		f = [this, args...]() -> void {
			if (!hide_)
				ImGui::Text(text_.c_str(), std::forward<Params>(args)...);
		};
	}

	template <typename ...Params>
	void ChangeParams(Params&&... args)
	{
		f = [this, args...]() -> void {
			if (!hide_)
				ImGui::Text(text_.c_str(), std::forward<Params>(args)...);
		};
	}

	void Hide() { hide_ = true; }
	void Show() { hide_ = false; }

	// Inherited via IHUDElement
	virtual void Render() override
	{
		f();
	}
};

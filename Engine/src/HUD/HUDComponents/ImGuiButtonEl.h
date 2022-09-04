#pragma once
#include "../../pch.h"
#include "IHUDElement.h"

class ImGuiButtonEl : public IHUDElement
{
	typedef std::function<void()> ButtonOnClickCB ;
private:
	std::string title_;
	std::function<void()> cb_;
public:
	ImGuiButtonEl(const std::string& title, ButtonOnClickCB cb)
		: title_(title), cb_(cb) {}
	// Inherited via IHUDElement
	virtual void Render() override
	{
		if (ImGui::Button(title_.c_str()))
			cb_();
	};
};

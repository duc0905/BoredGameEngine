#pragma once
#include "../../pch.h"
#include "IHUDElement.h"

/**
* Root of HUD components
* Provide basic utilities to manage its child elements
*/
class IHUDWindow
{
protected:
	std::vector<std::unique_ptr<IHUDElement>> els_;
public:
	/** IHUSWindow::AddElement
	* 
	* @param {IHUDElemen*} raw pointer of new element
	* @return {IHUDWindow} the current window
	* 
	*/
	IHUDWindow& AddElement(IHUDElement* el)
	{
		els_.push_back(std::unique_ptr<IHUDElement>(el));

		return *this;
	}

	virtual bool Render() = 0;
};

class NullHUDWindow : public IHUDWindow
{
	virtual bool Render() override { return true; }
};

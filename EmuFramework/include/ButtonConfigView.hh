/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#pragma once

#include <util/gui/BaseMenuView.hh>
#include <gui/AlertView.hh>
#include <EmuInput.hh>

extern YesNoAlertView ynAlertView;

static class ButtonConfigSetView : public View
{
	Rect2<int> viewFrame
	#ifndef CONFIG_BASE_PS3
	, unbindB, cancelB
	#endif
	;

	Gfx::Text text
	#ifndef CONFIG_BASE_PS3
	, unbind, cancel
	#endif
	;

public:
	constexpr ButtonConfigSetView() { }

	typedef Delegate<void (const Input::Event &e)> OnSetDelegate;
	OnSetDelegate onSet;
	const Input::Device *dev = nullptr;
	const Input::Device *savedDev = nullptr;

	Rect2<int> &viewRect() { return viewFrame; }

	void init(Input::Device &dev);
	void deinit();
	void place();
	void inputEvent(const Input::Event &e);
	void draw();
} btnSetView2;



class ButtonConfigView : public BaseMenuView
{
	TextMenuItem reset;

	void inputEvent(const Input::Event &e);

	MenuItem **text = nullptr;

	struct BtnConfigMenuItem : public DualTextMenuItem
	{
		void draw(Coordinate xPos, Coordinate yPos, Coordinate xSize, Coordinate ySize, _2DOrigin align) const;
	};

	BtnConfigMenuItem *btn = nullptr;

public:
	constexpr ButtonConfigView() { }
	const KeyCategory *cat = nullptr;
	InputDeviceConfig *devConf = nullptr;
	int keyToSet = 0;

	void init(const KeyCategory *cat,
		InputDeviceConfig &devConf, bool highlightFirst);
	void deinit();
	void confirmUnbindKeysAlert(const Input::Event &e);
	void resetHandler(TextMenuItem &, const Input::Event &e);
	void onSet(const Input::Event &e);
	void onSelectElement(const GuiTable1D *, const Input::Event &e, uint i);
};

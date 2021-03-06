#pragma once
#include <input/Input.hh>
#include <gfx/Gfx.hh>

#ifdef CONFIG_BLUETOOTH
#include <bluetooth/BluetoothInputDevScanner.hh>
#endif

namespace Input
{

StaticDLList<Device, MAX_DEVS> devList;

void addDevice(Device d)
{
	d.idx = devList.size;
	devList.addToEnd(d);
}

void removeDevice(Device d)
{
	devList.remove(d);
	indexDevices();
}

void indexDevices()
{
	// re-number device indices
	uint i = 0;
	forEachInDLList(&Input::devList, e)
	{
		e.idx = i;
		i++;
	}
}

uint Device::map() const
{
	return
		#ifdef CONFIG_INPUT_ICADE
			iCadeMode_ ? (uint)Input::Event::MAP_ICADE :
		#endif
			map_;
}

#if defined CONFIG_INPUT_ICADE && !defined CONFIG_BASE_IOS
void Device::setICadeMode(bool on)
{
	if(map_ == Input::Event::MAP_KEYBOARD)
	{
		logMsg("set iCade mode %s for %s", on ? "on" : "off", name());
		iCadeMode_ = on;
	}
	else
		logWarn("tried to set iCade mode on device with map %d", map_);
}
#endif

bool Event::isDefaultConfirmButton(uint swapped) const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE: return swapped ? isDefaultCancelButton(0) :
				(button == Input::Wiimote::_1 || button == Input::Wiimote::B || button == Input::Wiimote::NUN_Z);
		case MAP_ICONTROLPAD: return swapped ? isDefaultCancelButton(0) : (button == Input::iControlPad::X);
		case MAP_ZEEMOTE: return swapped ? isDefaultCancelButton(0) : (button == Input::Zeemote::A);
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return swapped ? isDefaultCancelButton(0) : (button == Input::ICade::A || button == Input::ICade::B);
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return swapped ? isDefaultCancelButton(0) : (button == Input::Ps3::CROSS);
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			#ifdef CONFIG_BASE_ANDROID
			switch(device->subtype)
			{
				case Device::SUBTYPE_PS3_CONTROLLER:
					return swapped ? isDefaultCancelButton(0) : button == Input::Keycode::GAME_X;
			}
			#endif
			return button == Input::Keycode::ENTER
			#ifdef CONFIG_BASE_ANDROID
			|| button == Input::Keycode::CENTER || (swapped ? isDefaultCancelButton(0) : (button == Input::Keycode::GAME_A || button == Input::Keycode::GAME_1))
			#endif
			;
		#endif
	}
	return 0;
}

bool Event::isDefaultCancelButton(uint swapped) const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE: return swapped ? isDefaultConfirmButton(0) :
				(button == Input::Wiimote::_2 || button == Input::Wiimote::A || button == Input::Wiimote::NUN_C);
		case MAP_ICONTROLPAD: return swapped ? isDefaultConfirmButton(0) : (button == Input::iControlPad::B);
		case MAP_ZEEMOTE: return swapped ? isDefaultConfirmButton(0) : (button == Input::Zeemote::B);
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return swapped ? isDefaultConfirmButton(0) : (button == Input::ICade::C || button == Input::ICade::D);
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return swapped ? isDefaultConfirmButton(0) : (button == Input::Ps3::CIRCLE);
		#endif
		#ifdef INPUT_SUPPORTS_MOUSE
		case MAP_POINTER: return button == Input::Pointer::DOWN_BUTTON;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			#ifdef CONFIG_BASE_ANDROID
			switch(device->subtype)
			{
				case Device::SUBTYPE_PS3_CONTROLLER:
					return swapped ? isDefaultConfirmButton(0) : button == Input::Keycode::GAME_Y;
			}
			#endif
			return button == Input::Keycode::ESCAPE
				#ifdef CONFIG_INPUT_ANDROID
				|| (swapped ? isDefaultConfirmButton(0) : (button == Input::Keycode::GAME_B || button == Input::Keycode::GAME_2))
				#endif
				;
		#endif
	}
	return 0;
}

bool Event::isDefaultLeftButton() const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE:
			return button == Input::Wiimote::LEFT || button == Input::Wiimote::CC_LSTICK_LEFT || button == Input::Wiimote::NUN_STICK_LEFT;
		case MAP_ICONTROLPAD: return button == Input::iControlPad::LEFT || button == Input::iControlPad::LNUB_LEFT;
		case MAP_ZEEMOTE: return button == Input::Zeemote::LEFT;
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return button == Input::ICade::LEFT;
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return button == Input::Ps3::LEFT;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			return button == Input::Keycode::LEFT
			#ifdef CONFIG_BASE_ANDROID
			|| button == Input::Keycode::JS1_XAXIS_NEG
			#endif
			#ifdef CONFIG_ENV_WEBOS
			|| button == Input::asciiKey('d')
			#endif
			;
		#endif
	}
	return 0;
}

bool Event::isDefaultRightButton() const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE:
			return button == Input::Wiimote::RIGHT || button == Input::Wiimote::CC_LSTICK_RIGHT || button == Input::Wiimote::NUN_STICK_RIGHT;
		case MAP_ICONTROLPAD: return button == Input::iControlPad::RIGHT || button == Input::iControlPad::LNUB_RIGHT;
		case MAP_ZEEMOTE: return button == Input::Zeemote::RIGHT;
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return button == Input::ICade::RIGHT;
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return button == Input::Ps3::RIGHT;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			return button == Input::Keycode::RIGHT
			#ifdef CONFIG_BASE_ANDROID
			|| button == Input::Keycode::JS1_XAXIS_POS
			#endif
			#ifdef CONFIG_ENV_WEBOS
			|| button == Input::asciiKey('g')
			#endif
			;
		#endif
	}
	return 0;
}

bool Event::isDefaultUpButton() const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE:
			return button == Input::Wiimote::UP || button == Input::Wiimote::CC_LSTICK_UP || button == Input::Wiimote::NUN_STICK_UP;
		case MAP_ICONTROLPAD: return button == Input::iControlPad::UP || button == Input::iControlPad::LNUB_UP;
		case MAP_ZEEMOTE: return button == Input::Zeemote::UP;
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return button == Input::ICade::UP;
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return button == Input::Ps3::UP;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			return button == Input::Keycode::UP
			#ifdef CONFIG_BASE_ANDROID
			|| button == Input::Keycode::JS1_YAXIS_NEG
			#endif
			#ifdef CONFIG_ENV_WEBOS
			|| button == Input::asciiKey('r')
			#endif
			;
		#endif
	}
	return 0;
}

bool Event::isDefaultDownButton() const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE:
			return button == Input::Wiimote::DOWN || button == Input::Wiimote::CC_LSTICK_DOWN || button == Input::Wiimote::NUN_STICK_DOWN;
		case MAP_ICONTROLPAD: return button == Input::iControlPad::DOWN || button == Input::iControlPad::LNUB_DOWN;
		case MAP_ZEEMOTE: return button == Input::Zeemote::DOWN;
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return button == Input::ICade::DOWN;
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return button == Input::Ps3::DOWN;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			return button == Input::Keycode::DOWN
			#ifdef CONFIG_BASE_ANDROID
			|| button == Input::Keycode::JS1_YAXIS_POS
			#endif
			#ifdef CONFIG_ENV_WEBOS
			|| button == Input::asciiKey('c')
			#endif
			;
		#endif
	}
	return 0;
}

bool Event::isDefaultPageUpButton() const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE: return button == Input::Wiimote::PLUS || button == Input::Wiimote::L;
		case MAP_ICONTROLPAD: return button == Input::iControlPad::L;
		case MAP_ZEEMOTE: return 0;
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return button == Input::ICade::E;
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return button == Input::Ps3::L1;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			return button == Input::Keycode::PGUP
		#ifdef CONFIG_BASE_ANDROID
				|| button == Input::Keycode::GAME_L1
		#endif
				;
		#endif
	}
	return 0;
}

bool Event::isDefaultPageDownButton() const
{
	switch(map)
	{
		#ifdef CONFIG_BLUETOOTH
		case MAP_WIIMOTE: return button == Input::Wiimote::MINUS || button == Input::Wiimote::R;
		case MAP_ICONTROLPAD: return button == Input::iControlPad::R;
		case MAP_ZEEMOTE: return 0;
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case MAP_ICADE: return button == Input::ICade::F;
		#endif
		#ifdef CONFIG_BASE_PS3
		case MAP_PS3PAD: return button == Input::Ps3::R1;
		#endif
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case MAP_KEYBOARD:
			return button == Input::Keycode::PGDOWN
		#ifdef CONFIG_BASE_ANDROID
				|| button == Input::Keycode::GAME_R1
		#endif
				;
		#endif
	}
	return 0;
}

bool swappedGamepadConfirm = 0;

struct PointerState
{
	constexpr PointerState() { }
	int x = 0, y = 0, inWin = 0;
};

static uint xPointerTransform_ = POINTER_NORMAL;
void xPointerTransform(uint mode)
{
	xPointerTransform_ = mode;
}

static uint yPointerTransform_ = POINTER_NORMAL;
void yPointerTransform(uint mode)
{
	yPointerTransform_ = mode;
}

static uint pointerAxis_ = POINTER_NORMAL;
void pointerAxis(uint mode)
{
	pointerAxis_ = mode;
}

void pointerPos(int x, int y, int *xOut, int *yOut)
{
	// x,y axis is swapped first
	*xOut = pointerAxis_ == POINTER_INVERT ? y : x;
	*yOut = pointerAxis_ == POINTER_INVERT ? x : y;
	
	// then coordinates are inverted
	if(xPointerTransform_ == POINTER_INVERT)
		*xOut = Gfx::viewPixelWidth() - *xOut;
	if(yPointerTransform_ == POINTER_INVERT)
		*yOut = Gfx::viewPixelHeight() - *yOut;
}

#ifdef INPUT_SUPPORTS_KEYBOARD
static const char *keyButtonName(Key b)
{
	switch(b)
	{
		case 0: return "None";

		case asciiKey(' '): return "Space";
		#ifndef CONFIG_BASE_ANDROID
		// no unique codes for these on Android
		case asciiKey('{'): return "{";
		case asciiKey('|'): return "|";
		case asciiKey('}'): return "}";
		case asciiKey('~'): return "~";
		case asciiKey('!'): return "!";
		case asciiKey('"'): return "\"";
		case asciiKey('$'): return "$";
		case asciiKey('%'): return "%";
		case asciiKey('&'): return "&";
		case asciiKey('('): return "(";
		case asciiKey(')'): return ")";
		case asciiKey('>'): return ">";
		case asciiKey('<'): return "<";
		case asciiKey('?'): return "?";
		case asciiKey(':'): return ":";
		case asciiKey('^'): return "^";
		case asciiKey('_'): return "_";
		case asciiKey('A'): return "A";
		case asciiKey('B'): return "B";
		case asciiKey('C'): return "C";
		case asciiKey('D'): return "D";
		case asciiKey('E'): return "E";
		case asciiKey('F'): return "F";
		case asciiKey('G'): return "G";
		case asciiKey('H'): return "H";
		case asciiKey('I'): return "I";
		case asciiKey('J'): return "J";
		case asciiKey('K'): return "K";
		case asciiKey('L'): return "L";
		case asciiKey('M'): return "M";
		case asciiKey('N'): return "N";
		case asciiKey('O'): return "O";
		case asciiKey('P'): return "P";
		case asciiKey('Q'): return "Q";
		case asciiKey('R'): return "R";
		case asciiKey('S'): return "S";
		case asciiKey('T'): return "T";
		case asciiKey('U'): return "U";
		case asciiKey('V'): return "V";
		case asciiKey('W'): return "W";
		case asciiKey('X'): return "X";
		case asciiKey('Y'): return "Y";
		case asciiKey('Z'): return "Z";
		#endif
		case asciiKey('+'): return "+";
		case asciiKey('\''): return "'";
		case asciiKey(','): return ",";
		case asciiKey('-'): return "-";
		case asciiKey('.'): return ".";
		case asciiKey('/'): return "/";
		case asciiKey('#'): return "#";
		case asciiKey('*'): return "*";
		case asciiKey('0'): return "0";
		case asciiKey('1'): return "1";
		case asciiKey('2'): return "2";
		case asciiKey('3'): return "3";
		case asciiKey('4'): return "4";
		case asciiKey('5'): return "5";
		case asciiKey('6'): return "6";
		case asciiKey('7'): return "7";
		case asciiKey('8'): return "8";
		case asciiKey('9'): return "9";
		case asciiKey(';'): return ";";
		case asciiKey('='): return "=";
		case asciiKey('@'): return "@";
		case asciiKey('['): return "[";
		case asciiKey('\\'): return "\\";
		case asciiKey(']'): return "]";
		case asciiKey('`'): return "`";
		case asciiKey('a'): return "a";
		case asciiKey('b'): return "b";
		case asciiKey('c'): return "c";
		case asciiKey('d'): return "d";
		case asciiKey('e'): return "e";
		case asciiKey('f'): return "f";
		case asciiKey('g'): return "g";
		case asciiKey('h'): return "h";
		case asciiKey('i'): return "i";
		case asciiKey('j'): return "j";
		case asciiKey('k'): return "k";
		case asciiKey('l'): return "l";
		case asciiKey('m'): return "m";
		case asciiKey('n'): return "n";
		case asciiKey('o'): return "o";
		case asciiKey('p'): return "p";
		case asciiKey('q'): return "q";
		case asciiKey('r'): return "r";
		case asciiKey('s'): return "s";
		case asciiKey('t'): return "t";
		case asciiKey('u'): return "u";
		case asciiKey('v'): return "v";
		case asciiKey('w'): return "w";
		case asciiKey('x'): return "x";
		case asciiKey('y'): return "y";
		case asciiKey('z'): return "z";
		case Keycode::ESCAPE:
		#ifdef CONFIG_BASE_ANDROID
			return "Back";
		#else
			return "Escape";
		#endif
		case Keycode::ENTER: return "Enter";
		case Keycode::LALT: return "Left Alt";
		case Keycode::RALT:
		#if defined CONFIG_ENV_WEBOS && CONFIG_ENV_WEBOS_OS <= 2
			return "Mod";
		#else
			return "Right Alt";
		#endif
		case Keycode::LSHIFT: return "Left Shift";
		case Keycode::RSHIFT: return "Right Shift";
		case Keycode::LCTRL: return "Left Ctrl";
		case Keycode::RCTRL:
		#if defined CONFIG_ENV_WEBOS && CONFIG_ENV_WEBOS_OS <= 2
			return "Sym";
		#else
			return "Right Ctrl";
		#endif
		case Keycode::UP: return "Up";
		case Keycode::RIGHT: return "Right";
		case Keycode::DOWN: return "Down";
		case Keycode::LEFT: return "Left";
		case Keycode::BACK_SPACE: return "Back Space";
		case Keycode::MENU: return "Menu";
		case Keycode::HOME: return "Home";
		case Keycode::END: return "End";
		case Keycode::INSERT: return "Insert";
		case Keycode::DELETE: return "Delete";
		case Keycode::TAB: return "Tab";
		case Keycode::SCROLL_LOCK: return "Scroll Lock";
		case Keycode::CAPS: return "Caps Lock";
		case Keycode::PAUSE: return "Pause";
		case Keycode::LMETA: return "Left Meta";
		case Keycode::RMETA: return "Right Meta";
		case Keycode::F1: return "F1";
		case Keycode::F2: return "F2";
		case Keycode::F3: return "F3";
		case Keycode::F4: return "F4";
		case Keycode::F5: return "F5";
		case Keycode::F6: return "F6";
		case Keycode::F7: return "F7";
		case Keycode::F8: return "F8";
		case Keycode::F9: return "F9";
		case Keycode::F10: return "F10";
		case Keycode::F11: return "F11";
		case Keycode::F12: return "F12";
		#ifndef CONFIG_ENV_WEBOS
		case Keycode::SEARCH: return "Search";
		#endif

		// Android-specific
		#ifdef CONFIG_BASE_ANDROID
		case Keycode::CENTER: return "Center";
		case Keycode::CAMERA: return "Camera";
		case Keycode::CALL: return "Call";
		case Keycode::END_CALL: return "End Call";
		case Keycode::CLEAR: return "Clear";
		case Keycode::SYMBOL: return "Sym";
		case Keycode::EXPLORER: return "Explorer";
		case Keycode::MAIL: return "Mail";
		case Keycode::NUM: return "Num";
		case Keycode::FUNCTION: return "Function";
		case Keycode::GAME_A: return "A";
		case Keycode::GAME_B: return "B";
		case Keycode::GAME_C: return "C";
		case Keycode::GAME_X: return "X";
		case Keycode::GAME_Y: return "Y";
		case Keycode::GAME_Z: return "Z";
		case Keycode::GAME_L1: return "L1";
		case Keycode::GAME_R1: return "R1";
		case Keycode::GAME_L2: return "L2";
		case Keycode::GAME_R2: return "R2";
		case Keycode::GAME_LEFT_THUMB: return "L-Thumb";
		case Keycode::GAME_RIGHT_THUMB: return "R-Thumb";
		case Keycode::GAME_START: return "Start";
		case Keycode::GAME_SELECT: return "Select";
		case Keycode::GAME_MODE: return "Mode";
		case Keycode::GAME_1: return "G1";
		case Keycode::GAME_2: return "G2";
		case Keycode::GAME_3: return "G3";
		case Keycode::GAME_4: return "G4";
		case Keycode::GAME_5: return "G5";
		case Keycode::GAME_6: return "G6";
		case Keycode::GAME_7: return "G7";
		case Keycode::GAME_8: return "G8";
		case Keycode::GAME_9: return "G9";
		case Keycode::GAME_10: return "G10";
		case Keycode::GAME_11: return "G11";
		case Keycode::GAME_12: return "G12";
		case Keycode::GAME_13: return "G13";
		case Keycode::GAME_14: return "G14";
		case Keycode::GAME_15: return "G15";
		case Keycode::GAME_16: return "G16";
		case Keycode::VOL_UP: return "Vol Up";
		case Keycode::VOL_DOWN: return "Vol Down";
		case Keycode::FOCUS: return "Focus";

		case Keycode::JS1_XAXIS_POS: return "X Axis+";
		case Keycode::JS1_XAXIS_NEG: return "X Axis-";
		case Keycode::JS1_YAXIS_POS: return "Y Axis+";
		case Keycode::JS1_YAXIS_NEG: return "Y Axis-";
		case Keycode::JS2_XAXIS_POS: return "X Axis+ 2";
		case Keycode::JS2_XAXIS_NEG: return "X Axis- 2";
		case Keycode::JS2_YAXIS_POS: return "Y Axis+ 2";
		case Keycode::JS2_YAXIS_NEG: return "Y Axis- 2";
		case Keycode::JS3_XAXIS_POS: return "X Axis+ 3";
		case Keycode::JS3_XAXIS_NEG: return "X Axis- 3";
		case Keycode::JS3_YAXIS_POS: return "Y Axis+ 3";
		case Keycode::JS3_YAXIS_NEG: return "Y Axis- 3";
		case Keycode::JS_LTRIGGER_AXIS: return "L Trigger";
		case Keycode::JS_RTRIGGER_AXIS: return "R Trigger";
		#endif
	}
	return "Unknown";
}
#endif

static const char *wiimoteButtonName(Key b)
{
	switch(b)
	{
		case 0: return "None";
		case Wiimote::_1: return "1";
		case Wiimote::_2: return "2";
		case Wiimote::A: return "A";
		case Wiimote::B: return "B";
		case Wiimote::PLUS: return "+";
		case Wiimote::MINUS: return "-";
		case Wiimote::HOME: return "Home";
		case Wiimote::L: return "L";
		case Wiimote::R: return "R";
		case Wiimote::ZL: return "ZL";
		case Wiimote::ZR: return "ZR";
		case Wiimote::X: return "X";
		case Wiimote::Y: return "Y";
		case Wiimote::CC_LSTICK_LEFT: return "L:Left";
		case Wiimote::CC_LSTICK_RIGHT: return "L:Right";
		case Wiimote::CC_LSTICK_UP: return "L:Up";
		case Wiimote::CC_LSTICK_DOWN: return "L:Down";
		case Wiimote::CC_RSTICK_LEFT: return "R:Left";
		case Wiimote::CC_RSTICK_RIGHT: return "R:Right";
		case Wiimote::CC_RSTICK_UP: return "R:Up";
		case Wiimote::CC_RSTICK_DOWN: return "R:Down";
		case Wiimote::UP: return "Up";
		case Wiimote::RIGHT: return "Right";
		case Wiimote::DOWN: return "Down";
		case Wiimote::LEFT: return "Left";
		case Wiimote::NUN_C: return "C";
		case Wiimote::NUN_Z: return "Z";
		case Wiimote::NUN_STICK_LEFT: return "N:Left";
		case Wiimote::NUN_STICK_RIGHT: return "N:Right";
		case Wiimote::NUN_STICK_UP: return "N:Up";
		case Wiimote::NUN_STICK_DOWN: return "N:Down";
	}
	return "Unknown";
}

static const char *icpButtonName(Key b)
{
	switch(b)
	{
		case 0: return "None";
		case iControlPad::A: return "A";
		case iControlPad::B: return "B";
		case iControlPad::X: return "X";
		case iControlPad::Y: return "Y";
		case iControlPad::L: return "L";
		case iControlPad::R: return "R";
		case iControlPad::START: return "Start";
		case iControlPad::SELECT: return "Select";
		case iControlPad::LNUB_LEFT: return "L:Left";
		case iControlPad::LNUB_RIGHT: return "L:Right";
		case iControlPad::LNUB_UP: return "L:Up";
		case iControlPad::LNUB_DOWN: return "L:Down";
		case iControlPad::RNUB_LEFT: return "R:Left";
		case iControlPad::RNUB_RIGHT: return "R:Right";
		case iControlPad::RNUB_UP: return "R:Up";
		case iControlPad::RNUB_DOWN: return "R:Down";
		case iControlPad::UP: return "Up";
		case iControlPad::RIGHT: return "Right";
		case iControlPad::DOWN: return "Down";
		case iControlPad::LEFT: return "Left";
	}
	return "Unknown";
}

static const char *zeemoteButtonName(Key b)
{
	switch(b)
	{
		case 0: return "None";
		case Zeemote::A: return "A";
		case Zeemote::B: return "B";
		case Zeemote::C: return "C";
		case Zeemote::POWER: return "Power";
		case Zeemote::UP: return "Up";
		case Zeemote::RIGHT: return "Right";
		case Zeemote::DOWN: return "Down";
		case Zeemote::LEFT: return "Left";
	}
	return "Unknown";
}

static const char *iCadeButtonName(Key b)
{
	switch(b)
	{
		case 0: return "None";
		#ifdef CONFIG_BASE_IOS
		// Show the iControlPad buttons only on iOS
		case ICade::A: return "A (iCP A)";
		case ICade::B: return "B (iCP B)";
		case ICade::C: return "C (iCP X)";
		case ICade::D: return "D (iCP Y)";
		case ICade::E: return "E (iCP R)";
		case ICade::F: return "F (iCP L)";
		case ICade::G: return "G (iCP Start)";
		case ICade::H: return "H (iCP Select)";
		#else
		case ICade::A: return "A";
		case ICade::B: return "B";
		case ICade::C: return "C";
		case ICade::D: return "D";
		case ICade::E: return "E";
		case ICade::F: return "F";
		case ICade::G: return "G";
		case ICade::H: return "H";
		#endif
		case ICade::UP: return "Up";
		case ICade::RIGHT: return "Right";
		case ICade::DOWN: return "Down";
		case ICade::LEFT: return "Left";
	}
	return "Unknown";
}

static const char *ps3ButtonName(Key b)
{
	switch(b)
	{
		case 0: return "None";
		case Ps3::CROSS: return "Cross";
		case Ps3::CIRCLE: return "Circle";
		case Ps3::SQUARE: return "Square";
		case Ps3::TRIANGLE: return "Triangle";
		case Ps3::L1: return "L1";
		case Ps3::L2: return "L2";
		case Ps3::L3: return "L3";
		case Ps3::R1: return "R1";
		case Ps3::R2: return "R2";
		case Ps3::R3: return "R3";
		case Ps3::SELECT: return "Select";
		case Ps3::START: return "Start";
		case Ps3::UP: return "Up";
		case Ps3::RIGHT: return "Right";
		case Ps3::DOWN: return "Down";
		case Ps3::LEFT: return "Left";
	}
	return "Unknown";
}

const char *buttonName(uint map, Key b)
{
	switch(map)
	{
		#ifdef INPUT_SUPPORTS_KEYBOARD
		case Input::Event::MAP_KEYBOARD: return keyButtonName(b);
		#endif
		#ifdef CONFIG_BLUETOOTH
		case Input::Event::MAP_WIIMOTE: return wiimoteButtonName(b);
		case Input::Event::MAP_ICONTROLPAD: return icpButtonName(b);
		case Input::Event::MAP_ZEEMOTE: return zeemoteButtonName(b);
		#endif
		#ifdef CONFIG_INPUT_ICADE
		case Input::Event::MAP_ICADE:
			auto name = iCadeButtonName(b);
			#ifndef CONFIG_BASE_IOS
			if(string_equal(name, "Unknown"))
			{
				// if it's not an iCade button, fall back to regular keys
				return keyButtonName(b);
			}
			#endif
			return name;
		#endif
		#ifdef CONFIG_BASE_PS3
		case Input::Event::DEV_PS3PAD: return ps3ButtonName(b);
		#endif
	}
	return "Unknown";
}

bool keyInputIsPresent()
{
	return Device::anyTypeBitsPresent(Device::TYPE_BIT_KEYBOARD | Device::TYPE_BIT_GAMEPAD);
}

const char *eventActionToStr(int action)
{
	switch(action)
	{
		default:
		case UNUSED: return "Unknown";
		case RELEASED: return "Released";
		case PUSHED: return "Pushed";
		case MOVED: return "Moved";
		case MOVED_RELATIVE: return "Moved Relative";
		case EXIT_VIEW: return "Left View";
		case ENTER_VIEW: return "Entered View";
	}
}

}

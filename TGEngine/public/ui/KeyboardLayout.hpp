#ifdef _WIN32
#include <sdkddkver.h>
#endif 

//TODO OS Independency

namespace tge::key {

	constexpr unsigned int LBUTTON = 0x01;
	constexpr unsigned int RBUTTON = 0x02;
	constexpr unsigned int CANCEL = 0x03;
	constexpr unsigned int MBUTTON = 0x04;
#if(_WIN32_WINNT >= 0x0500)
	constexpr unsigned int _XBUTTON1 = 0x05;
	constexpr unsigned int _XBUTTON2 = 0x06;
#endif
	constexpr unsigned int BACK = 0x08;
	constexpr unsigned int TAB = 0x09;
	constexpr unsigned int CLEAR = 0x0C;
	constexpr unsigned int RETURN = 0x0D;
	constexpr unsigned int SHIFT = 0x10;
	constexpr unsigned int CONTROL = 0x11;
	constexpr unsigned int MENU = 0x12;
	constexpr unsigned int PAUSE = 0x13;
	constexpr unsigned int CAPITAL = 0x14;
	constexpr unsigned int KANA = 0x15;
	constexpr unsigned int HANGEUL = 0x15;
	constexpr unsigned int HANGUL = 0x15;
	constexpr unsigned int JUNJA = 0x17;
	constexpr unsigned int FINAL = 0x18;
	constexpr unsigned int HANJA = 0x19;
	constexpr unsigned int KANJI = 0x19;
	constexpr unsigned int ESCAPE = 0x1B;
	constexpr unsigned int CONVERT = 0x1C;
	constexpr unsigned int NONCONVERT = 0x1D;
	constexpr unsigned int ACCEPT = 0x1E;
	constexpr unsigned int MODECHANGE = 0x1F;
	constexpr unsigned int SPACE = 0x20;
	constexpr unsigned int PRIOR = 0x21;
	constexpr unsigned int NEXT = 0x22;
	constexpr unsigned int END = 0x23;
	constexpr unsigned int HOME = 0x24;
	constexpr unsigned int LEFT = 0x25;
	constexpr unsigned int UP = 0x26;
	constexpr unsigned int RIGHT = 0x27;
	constexpr unsigned int DOWN = 0x28;
	constexpr unsigned int SELECT = 0x29;
	constexpr unsigned int PRINT = 0x2A;
	constexpr unsigned int EXECUTE = 0x2B;
	constexpr unsigned int SNAPSHOT = 0x2C;
	constexpr unsigned int INSERT = 0x2D;
	constexpr unsigned int DEL = 0x2E;
	constexpr unsigned int HELP = 0x2F;
	constexpr unsigned int LWIN = 0x5B;
	constexpr unsigned int RWIN = 0x5C;
	constexpr unsigned int APPS = 0x5D;
	constexpr unsigned int SLEEP = 0x5F;
	constexpr unsigned int NUMPAD0 = 0x60;
	constexpr unsigned int NUMPAD1 = 0x61;
	constexpr unsigned int NUMPAD2 = 0x62;
	constexpr unsigned int NUMPAD3 = 0x63;
	constexpr unsigned int NUMPAD4 = 0x64;
	constexpr unsigned int NUMPAD5 = 0x65;
	constexpr unsigned int NUMPAD6 = 0x66;
	constexpr unsigned int NUMPAD7 = 0x67;
	constexpr unsigned int NUMPAD8 = 0x68;
	constexpr unsigned int NUMPAD9 = 0x69;
	constexpr unsigned int MULTIPLY = 0x6A;
	constexpr unsigned int ADD = 0x6B;
	constexpr unsigned int SEPARATOR = 0x6C;
	constexpr unsigned int SUBTRACT = 0x6D;
	constexpr unsigned int DECIMAL = 0x6E;
	constexpr unsigned int DIVIDE = 0x6F;
	constexpr unsigned int F1 = 0x70;
	constexpr unsigned int F2 = 0x71;
	constexpr unsigned int F3 = 0x72;
	constexpr unsigned int F4 = 0x73;
	constexpr unsigned int F5 = 0x74;
	constexpr unsigned int F6 = 0x75;
	constexpr unsigned int F7 = 0x76;
	constexpr unsigned int F8 = 0x77;
	constexpr unsigned int F9 = 0x78;
	constexpr unsigned int F10 = 0x79;
	constexpr unsigned int F11 = 0x7A;
	constexpr unsigned int F12 = 0x7B;
	constexpr unsigned int F13 = 0x7C;
	constexpr unsigned int F14 = 0x7D;
	constexpr unsigned int F15 = 0x7E;
	constexpr unsigned int F16 = 0x7F;
	constexpr unsigned int F17 = 0x80;
	constexpr unsigned int F18 = 0x81;
	constexpr unsigned int F19 = 0x82;
	constexpr unsigned int F20 = 0x83;
	constexpr unsigned int F21 = 0x84;
	constexpr unsigned int F22 = 0x85;
	constexpr unsigned int F23 = 0x86;
	constexpr unsigned int F24 = 0x87;

#if(_WIN32_WINNT >= 0x0604)
	constexpr unsigned int NAVIGATION_VIEW = 0x88; // reserved
	constexpr unsigned int NAVIGATION_MENU = 0x89; // reserved
	constexpr unsigned int NAVIGATION_UP = 0x8A; // reserved
	constexpr unsigned int NAVIGATION_DOWN = 0x8B; // reserved
	constexpr unsigned int NAVIGATION_LEFT = 0x8C; // reserved
	constexpr unsigned int NAVIGATION_RIGHT = 0x8D; // reserved
	constexpr unsigned int NAVIGATION_ACCEPT = 0x8E; // reserved
	constexpr unsigned int NAVIGATION_CANCEL = 0x8F; // reserved
#endif

	constexpr unsigned int NUMLOCK = 0x90;
	constexpr unsigned int SCROLL = 0x91;
	constexpr unsigned int OEM_NEC_EQUAL = 0x92;
	constexpr unsigned int OEM_FJ_JISHO = 0x92;
	constexpr unsigned int OEM_FJ_MASSHOU = 0x93;
	constexpr unsigned int OEM_FJ_TOUROKU = 0x94;
	constexpr unsigned int OEM_FJ_LOYA = 0x95;
	constexpr unsigned int OEM_FJ_ROYA = 0x96;
	constexpr unsigned int LSHIFT = 0xA0;
	constexpr unsigned int RSHIFT = 0xA1;
	constexpr unsigned int LCONTROL = 0xA2;
	constexpr unsigned int RCONTROL = 0xA3;
	constexpr unsigned int LMENU = 0xA4;
	constexpr unsigned int RMENU = 0xA5;

#if(_WIN32_WINNT >= 0x0500)
	constexpr unsigned int BROWSER_BACK = 0xA6;
	constexpr unsigned int BROWSER_FORWARD = 0xA7;
	constexpr unsigned int BROWSER_REFRESH = 0xA8;
	constexpr unsigned int BROWSER_STOP = 0xA9;
	constexpr unsigned int BROWSER_SEARCH = 0xAA;
	constexpr unsigned int BROWSER_FAVORITES = 0xAB;
	constexpr unsigned int BROWSER_HOME = 0xAC;

	constexpr unsigned int VOLUME_MUTE = 0xAD;
	constexpr unsigned int VOLUME_DOWN = 0xAE;
	constexpr unsigned int VOLUME_UP = 0xAF;
	constexpr unsigned int MEDIA_NEXT_TRACK = 0xB0;
	constexpr unsigned int MEDIA_PREV_TRACK = 0xB1;
	constexpr unsigned int MEDIA_STOP = 0xB2;
	constexpr unsigned int MEDIA_PLAY_PAUSE = 0xB3;
	constexpr unsigned int LAUNCH_MAIL = 0xB4;
	constexpr unsigned int LAUNCH_MEDIA_SELECT = 0xB5;
	constexpr unsigned int LAUNCH_APP1 = 0xB6;
	constexpr unsigned int LAUNCH_APP2 = 0xB7;

#endif

	constexpr unsigned int OEM_1 = 0xBA;   // ';:' for US
	constexpr unsigned int OEM_PLUS = 0xBB;   // '+' any country
	constexpr unsigned int OEM_COMMA = 0xBC;   // ',' any country
	constexpr unsigned int OEM_MINUS = 0xBD;   // '-' any country
	constexpr unsigned int OEM_PERIOD = 0xBE;   // '.' any country
	constexpr unsigned int OEM_2 = 0xBF;   // '/?' for US
	constexpr unsigned int OEM_3 = 0xC0;   // '`~' for US

#if(_WIN32_WINNT >= 0x0604)
	constexpr unsigned int GAMEPAD_A = 0xC3; // reserved
	constexpr unsigned int GAMEPAD_B = 0xC4; // reserved
	constexpr unsigned int GAMEPAD_X = 0xC5; // reserved
	constexpr unsigned int GAMEPAD_Y = 0xC6; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_SHOULDER = 0xC7; // reserved
	constexpr unsigned int GAMEPAD_LEFT_SHOULDER = 0xC8; // reserved
	constexpr unsigned int GAMEPAD_LEFT_TRIGGER = 0xC9; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_TRIGGER = 0xCA; // reserved
	constexpr unsigned int GAMEPAD_DPAD_UP = 0xCB; // reserved
	constexpr unsigned int GAMEPAD_DPAD_DOWN = 0xCC; // reserved
	constexpr unsigned int GAMEPAD_DPAD_LEFT = 0xCD; // reserved
	constexpr unsigned int GAMEPAD_DPAD_RIGHT = 0xCE; // reserved
	constexpr unsigned int GAMEPAD_MENU = 0xCF; // reserved
	constexpr unsigned int GAMEPAD_VIEW = 0xD0; // reserved
	constexpr unsigned int GAMEPAD_LEFT_THUMBSTICK_BUTTON = 0xD1; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2; // reserved
	constexpr unsigned int GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3; // reserved
	constexpr unsigned int GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4; // reserved
	constexpr unsigned int GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5; // reserved
	constexpr unsigned int GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9; // reserved
	constexpr unsigned int GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA; // reserved

#endif


	constexpr unsigned int OEM_4 = 0xDB;  //  '[{' for US
	constexpr unsigned int OEM_5 = 0xDC;  //  '\|' for US
	constexpr unsigned int OEM_6 = 0xDD;  //  ']}' for US
	constexpr unsigned int OEM_7 = 0xDE;  //  ''"' for US
	constexpr unsigned int OEM_8 = 0xDF;
	constexpr unsigned int OEM_AX = 0xE1;  //  'AX' key on Japanese AX kbd
	constexpr unsigned int OEM_102 = 0xE2;  //  "<>" or "\|" on RT 102-key kbd.
	constexpr unsigned int ICO_HELP = 0xE3;  //  Help key on ICO
	constexpr unsigned int ICO_00 = 0xE4;  //  00 key on ICO

#if(WINVER >= 0x0400)
	constexpr unsigned int PROCESSKEY = 0xE5;
#endif

	constexpr unsigned int ICO_CLEAR = 0xE6;

#if(_WIN32_WINNT >= 0x0500)
	constexpr unsigned int PACKET = 0xE7;
#endif

	constexpr unsigned int OEM_RESET = 0xE9;
	constexpr unsigned int OEM_JUMP = 0xEA;
	constexpr unsigned int OEM_PA1 = 0xEB;
	constexpr unsigned int OEM_PA2 = 0xEC;
	constexpr unsigned int OEM_PA3 = 0xED;
	constexpr unsigned int OEM_WSCTRL = 0xEE;
	constexpr unsigned int OEM_CUSEL = 0xEF;
	constexpr unsigned int OEM_ATTN = 0xF0;
	constexpr unsigned int OEM_FINISH = 0xF1;
	constexpr unsigned int OEM_COPY = 0xF2;
	constexpr unsigned int OEM_AUTO = 0xF3;
	constexpr unsigned int OEM_ENLW = 0xF4;
	constexpr unsigned int OEM_BACKTAB = 0xF5;

	constexpr unsigned int ATTN = 0xF6;
	constexpr unsigned int CRSEL = 0xF7;
	constexpr unsigned int EXSEL = 0xF8;
	constexpr unsigned int EREOF = 0xF9;
	constexpr unsigned int PLAY = 0xFA;
	constexpr unsigned int ZOOM = 0xFB;
	constexpr unsigned int NONAME = 0xFC;
	constexpr unsigned int PA1 = 0xFD;
	constexpr unsigned int OEM_CLEAR = 0xFE;
}
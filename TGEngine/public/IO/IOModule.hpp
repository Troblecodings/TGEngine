#pragma once

#include "../Module.hpp"

namespace tge::io {

struct KeyboardEvent {
  unsigned int signal;
};

struct MouseEvent {
  int x;
  int y;
  int pressed;
};

class IOModule : public tge::main::Module {

  main::Error init() override;

public:
  virtual void mouseEvent(const MouseEvent event) = 0;

  virtual void keyboardEvent(const KeyboardEvent event) = 0;
};

constexpr auto KC_LBUTTON = 0x01;
constexpr auto KC_RBUTTON = 0x02;
constexpr auto KC_CANCEL = 0x03;
constexpr auto KC_MBUTTON = 0x04; /* NOT contiguous with L & RBUTTON */
constexpr auto KC_XBUTTON1 = 0x05; /* NOT contiguous with L & RBUTTON */
constexpr auto KC_XBUTTON2 = 0x06; /* NOT contiguous with L & RBUTTON */
constexpr auto KC_BACK = 0x08;
constexpr auto KC_TAB = 0x09;
constexpr auto KC_CLEAR = 0x0C;
constexpr auto KC_RETURN = 0x0D;
constexpr auto KC_SHIFT = 0x10;
constexpr auto KC_CONTROL = 0x11;
constexpr auto KC_MENU = 0x12;
constexpr auto KC_PAUSE = 0x13;
constexpr auto KC_CAPITAL = 0x14;
constexpr auto KC_KANA = 0x15;
constexpr auto KC_HANGEUL = 0x15; /* old name - should be here for compatibility */
constexpr auto KC_HANGUL = 0x15;
constexpr auto KC_IME_ON = 0x16;
constexpr auto KC_JUNJA = 0x17;
constexpr auto KC_FINAL = 0x18;
constexpr auto KC_HANJA = 0x19;
constexpr auto KC_KANJI = 0x19;
constexpr auto KC_IME_OFF = 0x1A;
constexpr auto KC_ESCAPE = 0x1B;
constexpr auto KC_CONVERT = 0x1C;
constexpr auto KC_NONCONVERT = 0x1D;
constexpr auto KC_ACCEPT = 0x1E;
constexpr auto KC_MODECHANGE = 0x1F;
constexpr auto KC_SPACE = 0x20;
constexpr auto KC_PRIOR = 0x21;
constexpr auto KC_NEXT = 0x22;
constexpr auto KC_END = 0x23;
constexpr auto KC_HOME = 0x24;
constexpr auto KC_LEFT = 0x25;
constexpr auto KC_UP = 0x26;
constexpr auto KC_RIGHT = 0x27;
constexpr auto KC_DOWN = 0x28;
constexpr auto KC_SELECT = 0x29;
constexpr auto KC_PRINT = 0x2A;
constexpr auto KC_EXECUTE = 0x2B;
constexpr auto KC_SNAPSHOT = 0x2C;
constexpr auto KC_INSERT = 0x2D;
constexpr auto KC_DELETE = 0x2E;
constexpr auto KC_HELP = 0x2F;

/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x3A - 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

constexpr auto KC_LWIN = 0x5B;
constexpr auto KC_RWIN = 0x5C;
constexpr auto KC_APPS = 0x5D;
constexpr auto KC_SLEEP = 0x5F;
constexpr auto KC_NUMPAD0 = 0x60;
constexpr auto KC_NUMPAD1 = 0x61;
constexpr auto KC_NUMPAD2 = 0x62;
constexpr auto KC_NUMPAD3 = 0x63;
constexpr auto KC_NUMPAD4 = 0x64;
constexpr auto KC_NUMPAD5 = 0x65;
constexpr auto KC_NUMPAD6 = 0x66;
constexpr auto KC_NUMPAD7 = 0x67;
constexpr auto KC_NUMPAD8 = 0x68;
constexpr auto KC_NUMPAD9 = 0x69;
constexpr auto KC_MULTIPLY = 0x6A;
constexpr auto KC_ADD = 0x6B;
constexpr auto KC_SEPARATOR = 0x6C;
constexpr auto KC_SUBTRACT = 0x6D;
constexpr auto KC_DECIMAL = 0x6E;
constexpr auto KC_DIVIDE = 0x6F;
constexpr auto KC_F1 = 0x70;
constexpr auto KC_F2 = 0x71;
constexpr auto KC_F3 = 0x72;
constexpr auto KC_F4 = 0x73;
constexpr auto KC_F5 = 0x74;
constexpr auto KC_F6 = 0x75;
constexpr auto KC_F7 = 0x76;
constexpr auto KC_F8 = 0x77;
constexpr auto KC_F9 = 0x78;
constexpr auto KC_F10 = 0x79;
constexpr auto KC_F11 = 0x7A;
constexpr auto KC_F12 = 0x7B;
constexpr auto KC_F13 = 0x7C;
constexpr auto KC_F14 = 0x7D;
constexpr auto KC_F15 = 0x7E;
constexpr auto KC_F16 = 0x7F;
constexpr auto KC_F17 = 0x80;
constexpr auto KC_F18 = 0x81;
constexpr auto KC_F19 = 0x82;
constexpr auto KC_F20 = 0x83;
constexpr auto KC_F21 = 0x84;
constexpr auto KC_F22 = 0x85;
constexpr auto KC_F23 = 0x86;
constexpr auto KC_F24 = 0x87;
constexpr auto KC_NAVIGATION_VIEW = 0x88;   // reserved
constexpr auto KC_NAVIGATION_MENU = 0x89;   // reserved
constexpr auto KC_NAVIGATION_UP = 0x8A;     // reserved
constexpr auto KC_NAVIGATION_DOWN = 0x8B;   // reserved
constexpr auto KC_NAVIGATION_LEFT = 0x8C;   // reserved
constexpr auto KC_NAVIGATION_RIGHT = 0x8D;  // reserved
constexpr auto KC_NAVIGATION_ACCEPT = 0x8E; // reserved
constexpr auto KC_NAVIGATION_CANCEL = 0x8F; // reserved
constexpr auto KC_NUMLOCK = 0x90;
constexpr auto KC_SCROLL = 0x91;
constexpr auto KC_OEM_NEC_EQUAL = 0x92; // '=' key on numpad
constexpr auto KC_OEM_FJ_JISHO = 0x92;   // 'Dictionary' key
constexpr auto KC_OEM_FJ_MASSHOU = 0x93; // 'Unregister word' key
constexpr auto KC_OEM_FJ_TOUROKU = 0x94; // 'Register word' key
constexpr auto KC_OEM_FJ_LOYA = 0x95;    // 'Left OYAYUBI' key
constexpr auto KC_OEM_FJ_ROYA = 0x96;    // 'Right OYAYUBI' key
constexpr auto KC_LSHIFT = 0xA0;
constexpr auto KC_RSHIFT = 0xA1;
constexpr auto KC_LCONTROL = 0xA2;
constexpr auto KC_RCONTROL = 0xA3;
constexpr auto KC_LMENU = 0xA4;
constexpr auto KC_RMENU = 0xA5;
constexpr auto KC_BROWSER_BACK = 0xA6;
constexpr auto KC_BROWSER_FORWARD = 0xA7;
constexpr auto KC_BROWSER_REFRESH = 0xA8;
constexpr auto KC_BROWSER_STOP = 0xA9;
constexpr auto KC_BROWSER_SEARCH = 0xAA;
constexpr auto KC_BROWSER_FAVORITES = 0xAB;
constexpr auto KC_BROWSER_HOME = 0xAC;
constexpr auto KC_VOLUME_MUTE = 0xAD;
constexpr auto KC_VOLUME_DOWN = 0xAE;
constexpr auto KC_VOLUME_UP = 0xAF;
constexpr auto KC_MEDIA_NEXT_TRACK = 0xB0;
constexpr auto KC_MEDIA_PREV_TRACK = 0xB1;
constexpr auto KC_MEDIA_STOP = 0xB2;
constexpr auto KC_MEDIA_PLAY_PAUSE = 0xB3;
constexpr auto KC_LAUNCH_MAIL = 0xB4;
constexpr auto KC_LAUNCH_MEDIA_SELECT = 0xB5;
constexpr auto KC_LAUNCH_APP1 = 0xB6;
constexpr auto KC_LAUNCH_APP2 = 0xB7;
constexpr auto KC_OEM_1 = 0xBA;      // ';:' for US
constexpr auto KC_OEM_PLUS = 0xBB;   // '+' any country
constexpr auto KC_OEM_COMMA = 0xBC;  // ',' any country
constexpr auto KC_OEM_MINUS = 0xBD;  // '-' any country
constexpr auto KC_OEM_PERIOD = 0xBE; // '.' any country
constexpr auto KC_OEM_2 = 0xBF;      // '/?' for US
constexpr auto KC_OEM_3 = 0xC0;      // '`~' for US
constexpr auto KC_GAMEPAD_A = 0xC3;                       // reserved
constexpr auto KC_GAMEPAD_B = 0xC4;                       // reserved
constexpr auto KC_GAMEPAD_X = 0xC5;                       // reserved
constexpr auto KC_GAMEPAD_Y = 0xC6;                       // reserved
constexpr auto KC_GAMEPAD_RIGHT_SHOULDER = 0xC7;          // reserved
constexpr auto KC_GAMEPAD_LEFT_SHOULDER = 0xC8;           // reserved
constexpr auto KC_GAMEPAD_LEFT_TRIGGER = 0xC9;            // reserved
constexpr auto KC_GAMEPAD_RIGHT_TRIGGER = 0xCA;           // reserved
constexpr auto KC_GAMEPAD_DPAD_UP = 0xCB;                 // reserved
constexpr auto KC_GAMEPAD_DPAD_DOWN = 0xCC;               // reserved
constexpr auto KC_GAMEPAD_DPAD_LEFT = 0xCD;               // reserved
constexpr auto KC_GAMEPAD_DPAD_RIGHT = 0xCE;              // reserved
constexpr auto KC_GAMEPAD_MENU = 0xCF;                    // reserved
constexpr auto KC_GAMEPAD_VIEW = 0xD0;                    // reserved
constexpr auto KC_GAMEPAD_LEFT_THUMBSTICK_BUTTON = 0xD1;  // reserved
constexpr auto KC_GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2; // reserved
constexpr auto KC_GAMEPAD_LEFT_THUMBSTICK_UP = 0xD3;      // reserved
constexpr auto KC_GAMEPAD_LEFT_THUMBSTICK_DOWN = 0xD4;    // reserved
constexpr auto KC_GAMEPAD_LEFT_THUMBSTICK_RIGHT = 0xD5;   // reserved
constexpr auto KC_GAMEPAD_LEFT_THUMBSTICK_LEFT = 0xD6;    // reserved
constexpr auto KC_GAMEPAD_RIGHT_THUMBSTICK_UP = 0xD7;     // reserved
constexpr auto KC_GAMEPAD_RIGHT_THUMBSTICK_DOWN = 0xD8;   // reserved
constexpr auto KC_GAMEPAD_RIGHT_THUMBSTICK_RIGHT = 0xD9;  // reserved
constexpr auto KC_GAMEPAD_RIGHT_THUMBSTICK_LEFT = 0xDA;   // reserved
constexpr auto KC_OEM_4 = 0xDB; //  '[{' for US
constexpr auto KC_OEM_5 = 0xDC; //  '\|' for US
constexpr auto KC_OEM_6 = 0xDD; //  ']}' for US
constexpr auto KC_OEM_7 = 0xDE; //  ''"' for US
constexpr auto KC_OEM_8 = 0xDF;
constexpr auto KC_OEM_AX = 0xE1;   //  'AX' key on Japanese AX kbd
constexpr auto KC_OEM_102 = 0xE2;  //  "<>" or "\|" on RT 102-key kbd.
constexpr auto KC_ICO_HELP = 0xE3; //  Help key on ICO
constexpr auto KC_ICO_00 = 0xE4;   //  00 key on ICO
constexpr auto KC_PROCESSKEY = 0xE5;
constexpr auto KC_ICO_CLEAR = 0xE6;
constexpr auto KC_PACKET = 0xE7;
constexpr auto KC_OEM_RESET = 0xE9;
constexpr auto KC_OEM_JUMP = 0xEA;
constexpr auto KC_OEM_PA1 = 0xEB;
constexpr auto KC_OEM_PA2 = 0xEC;
constexpr auto KC_OEM_PA3 = 0xED;
constexpr auto KC_OEM_WSCTRL = 0xEE;
constexpr auto KC_OEM_CUSEL = 0xEF;
constexpr auto KC_OEM_ATTN = 0xF0;
constexpr auto KC_OEM_FINISH = 0xF1;
constexpr auto KC_OEM_COPY = 0xF2;
constexpr auto KC_OEM_AUTO = 0xF3;
constexpr auto KC_OEM_ENLW = 0xF4;
constexpr auto KC_OEM_BACKTAB = 0xF5;
constexpr auto KC_ATTN = 0xF6;
constexpr auto KC_CRSEL = 0xF7;
constexpr auto KC_EXSEL = 0xF8;
constexpr auto KC_EREOF = 0xF9;
constexpr auto KC_PLAY = 0xFA;
constexpr auto KC_ZOOM = 0xFB;
constexpr auto KC_NONAME = 0xFC;
constexpr auto KC_PA1 = 0xFD;
constexpr auto KC_OEM_CLEAR = 0xFE;

} // namespace tge::io

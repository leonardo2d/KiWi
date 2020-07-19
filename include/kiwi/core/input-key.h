#ifndef KW_INPUT_KEY_H
#define KW_INPUT_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum KW_InputKey {
  KW_INPUTKEY_UNKNOWN = 0,

  KW_INPUTKEY_RETURN = '\r',
  KW_INPUTKEY_ESCAPE = '\033',
  KW_INPUTKEY_BACKSPACE = '\b',
  KW_INPUTKEY_TAB = '\t',
  KW_INPUTKEY_SPACE = ' ',
  KW_INPUTKEY_EXCLAIM = '!',
  KW_INPUTKEY_QUOTEDBL = '"',
  KW_INPUTKEY_HASH = '#',
  KW_INPUTKEY_PERCENT = '%',
  KW_INPUTKEY_DOLLAR = '$',
  KW_INPUTKEY_AMPERSAND = '&',
  KW_INPUTKEY_QUOTE = '\'',
  KW_INPUTKEY_LEFTPAREN = '(',
  KW_INPUTKEY_RIGHTPAREN = ')',
  KW_INPUTKEY_ASTERISK = '*',
  KW_INPUTKEY_PLUS = '+',
  KW_INPUTKEY_COMMA = ',',
  KW_INPUTKEY_MINUS = '-',
  KW_INPUTKEY_PERIOD = '.',
  KW_INPUTKEY_SLASH = '/',
  KW_INPUTKEY_0 = '0',
  KW_INPUTKEY_1 = '1',
  KW_INPUTKEY_2 = '2',
  KW_INPUTKEY_3 = '3',
  KW_INPUTKEY_4 = '4',
  KW_INPUTKEY_5 = '5',
  KW_INPUTKEY_6 = '6',
  KW_INPUTKEY_7 = '7',
  KW_INPUTKEY_8 = '8',
  KW_INPUTKEY_9 = '9',
  KW_INPUTKEY_COLON = ':',
  KW_INPUTKEY_SEMICOLON = ';',
  KW_INPUTKEY_LESS = '<',
  KW_INPUTKEY_EQUALS = '=',
  KW_INPUTKEY_GREATER = '>',
  KW_INPUTKEY_QUESTION = '?',
  KW_INPUTKEY_AT = '@',
  /*
     Skip uppercase letters
   */
  KW_INPUTKEY_LEFTBRACKET = '[',
  KW_INPUTKEY_BACKSLASH = '\\',
  KW_INPUTKEY_RIGHTBRACKET = ']',
  KW_INPUTKEY_CARET = '^',
  KW_INPUTKEY_UNDERSCORE = '_',
  KW_INPUTKEY_BACKQUOTE = '`',
  KW_INPUTKEY_a = 'a',
  KW_INPUTKEY_b = 'b',
  KW_INPUTKEY_c = 'c',
  KW_INPUTKEY_d = 'd',
  KW_INPUTKEY_e = 'e',
  KW_INPUTKEY_f = 'f',
  KW_INPUTKEY_g = 'g',
  KW_INPUTKEY_h = 'h',
  KW_INPUTKEY_i = 'i',
  KW_INPUTKEY_j = 'j',
  KW_INPUTKEY_k = 'k',
  KW_INPUTKEY_l = 'l',
  KW_INPUTKEY_m = 'm',
  KW_INPUTKEY_n = 'n',
  KW_INPUTKEY_o = 'o',
  KW_INPUTKEY_p = 'p',
  KW_INPUTKEY_q = 'q',
  KW_INPUTKEY_r = 'r',
  KW_INPUTKEY_s = 's',
  KW_INPUTKEY_t = 't',
  KW_INPUTKEY_u = 'u',
  KW_INPUTKEY_v = 'v',
  KW_INPUTKEY_w = 'w',
  KW_INPUTKEY_x = 'x',
  KW_INPUTKEY_y = 'y',
  KW_INPUTKEY_z = 'z',

  KW_INPUTKEY_CAPSLOCK = (57u) | (1u << 30u),

  KW_INPUTKEY_F1 = (58u) | (1u << 30u),
  KW_INPUTKEY_F2 = (59u) | (1u << 30u),
  KW_INPUTKEY_F3 = (60u) | (1u << 30u),
  KW_INPUTKEY_F4 = (61u) | (1u << 30u),
  KW_INPUTKEY_F5 = (62u) | (1u << 30u),
  KW_INPUTKEY_F6 = (63u) | (1u << 30u),
  KW_INPUTKEY_F7 = (64u) | (1u << 30u),
  KW_INPUTKEY_F8 = (65u) | (1u << 30u),
  KW_INPUTKEY_F9 = (66u) | (1u << 30u),
  KW_INPUTKEY_F10 = (67u) | (1u << 30u),
  KW_INPUTKEY_F11 = (68u) | (1u << 30u),
  KW_INPUTKEY_F12 = (69u) | (1u << 30u),

  KW_INPUTKEY_INSERT = (73u) | (1u << 30u),
  KW_INPUTKEY_HOME = (74u) | (1u << 30u),
  KW_INPUTKEY_PAGEUP = (75u) | (1u << 30u),
  KW_INPUTKEY_DELETE = '\177',
  KW_INPUTKEY_END = (77u) | (1u << 30u),
  KW_INPUTKEY_PAGEDOWN = (78u) | (1u << 30u),
  KW_INPUTKEY_RIGHT = (79u) | (1u << 30u),
  KW_INPUTKEY_LEFT = (80u) | (1u << 30u),
  KW_INPUTKEY_DOWN = (81u) | (1u << 30u),
  KW_INPUTKEY_UP = (82u) | (1u << 30u),

  KW_INPUTKEY_REDO = (121u) | (1u << 30u),
  KW_INPUTKEY_UNDO = (122u) | (1u << 30u),
  KW_INPUTKEY_CUT = (123u) | (1u << 30u),
  KW_INPUTKEY_COPY = (124u) | (1u << 30u),
  KW_INPUTKEY_PASTE = (125u) | (1u << 30u),
} KW_InputKey;

#ifdef __cplusplus
}
#endif

#endif

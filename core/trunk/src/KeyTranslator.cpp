#include "NAS2D/KeyTranslator.h"

using namespace NAS2D;

const char* KeyTranslator::translate(KeyCode key, KeyModifier mod)
{
	switch(key)
	{
		// ====================================
		// = Alphabet
		// ====================================
		case KEY_a:
			if(shift(mod))
				return "A\0";
			else
				return "a\0";
			break;
		case KEY_b:
			if(shift(mod))
				return "B\0";
			else
				return "b\0";
			break;
		case KEY_c:
			if(shift(mod))
				return "C\0";
			else
				return "c\0";
			break;
		case KEY_d:
			if(shift(mod))
				return "D\0";
			else
				return "d\0";
			break;
		case KEY_e:
			if(shift(mod))
				return "E\0";
			else
				return "e\0";
			break;
		case KEY_f:
			if(shift(mod))
				return "F\0";
			else
				return "f\0";
			break;
		case KEY_g:
			if(shift(mod))
				return "G\0";
			else
				return "g\0";
			break;
		case KEY_h:
			if(shift(mod))
				return "H\0";
			else
				return "h\0";
			break;
		case KEY_i:
			if(shift(mod))
				return "I\0";
			else
				return "i\0";
			break;
		case KEY_j:
			if(shift(mod))
				return "J\0";
			else
				return "j\0";
			break;
		case KEY_k:
			if(shift(mod))
				return "K\0";
			else
				return "k\0";
			break;
		case KEY_l:
			if(shift(mod))
				return "L\0";
			else
				return "l\0";
			break;
		case KEY_m:
			if(shift(mod))
				return "M\0";
			else
				return "m\0";
			break;
		case KEY_n:
			if(shift(mod))
				return "N\0";
			else
				return "n\0";
			break;
		case KEY_o:
			if(shift(mod))
				return "O\0";
			else
				return "o\0";
			break;
		case KEY_p:
			if(shift(mod))
				return "P\0";
			else
				return "p\0";
			break;
		case KEY_q:
			if(shift(mod))
				return "Q\0";
			else
				return "q\0";
			break;
		case KEY_r:
			if(shift(mod))
				return "R\0";
			else
				return "r\0";
			break;
		case KEY_s:
			if(shift(mod))
				return "S\0";
			else
				return "s\0";
			break;
		case KEY_t:
			if(shift(mod))
				return "T\0";
			else
				return "t\0";
			break;
		case KEY_u:
			if(shift(mod))
				return "U\0";
			else
				return "u\0";
			break;
		case KEY_v:
			if(shift(mod))
				return "V\0";
			else
				return "v\0";
			break;
		case KEY_w:
			if(shift(mod))
				return "W\0";
			else
				return "w\0";
			break;
		case KEY_x:
			if(shift(mod))
				return "X\0";
			else
				return "x\0";
			break;
		case KEY_y:
			if(shift(mod))
				return "Y\0";
			else
				return "y\0";
			break;
		case KEY_z:
			if(shift(mod))
				return "Z\0";
			else
				return "z\0";
			break;

		// ====================================
		// = Numbers & Punctuation
		// ====================================
		case KEY_QUOTE:
			if(shift(mod))
				return "\"\0";
			else
				return "'\0";
			break;
		case KEY_0:
			if(shift(mod))
				return ")\0";
			else
				return "0\0";
			break;
		case KEY_1:
			if(shift(mod))
				return "!\0";
			else
				return "1\0";
			break;
		case KEY_2:
			if(shift(mod))
				return "@\0";
			else
				return "2\0";
			break;
		case KEY_3:
			if(shift(mod))
				return "#\0";
			else
				return "3\0";
			break;
		case KEY_4:
			if(shift(mod))
				return "$\0";
			else
				return "4\0";
			break;
		case KEY_5:
			if(shift(mod))
				return "%\0";
			else
				return "5\0";
			break;
		case KEY_6:
			if(shift(mod))
				return "^\0";
			else
				return "6\0";
			break;
		case KEY_7:
			if(shift(mod))
				return "&\0";
			else
				return "7\0";
			break;
		case KEY_8:
			if(shift(mod))
				return "*\0";
			else
				return "8\0";
			break;
		case KEY_9:
			if(shift(mod))
				return "(\0";
			else
				return "9\0";
			break;
		case KEY_SPACE:
			return " \0";
			break;
		case KEY_RETURN:
			return "\n\0";
			break;
		case KEY_COMMA:
			if(shift(mod))
				return "<\0";
			else
				return ",\0";
			break;
		case KEY_MINUS:
			if(shift(mod))
				return "_\0";
			else
				return "-\0";
			break;
		case KEY_PERIOD:
			if(shift(mod))
				return ">\0";
			else
				return ".\0";
			break;
		case KEY_SLASH:
			if(shift(mod))
				return "?\0";
			else
				return "/\0";
			break;
		case KEY_SEMICOLON:
			if(shift(mod))
				return ":\0";
			else
				return ";\0";
			break;
		case KEY_EQUALS:
			if(shift(mod))
				return "+\0";
			else
				return "=\0";
			break;
		case KEY_LEFTBRACKET:
			if(shift(mod))
				return "{\0";
			else
				return "[\0";
			break;
		case KEY_BACKSLASH:
			if(shift(mod))
				return "|\0";
			else
				return "\\\0";
			break;
		case KEY_RIGHTBRACKET:
			if(shift(mod))
				return "}\0";
			else
				return "]\0";
			break;
		case KEY_BACKQUOTE:
			if(shift(mod))
				return "~\0";
			else
				return "`\0";
			break;

		// ====================================
		// = Keypad
		// ====================================
		case KEY_KP0:
			if(numlock(mod))
				return "0\0";
			break;
		case KEY_KP1:
			if(numlock(mod))
				return "1\0";
			break;
		case KEY_KP2:
			if(numlock(mod))
				return "2\0";
			break;
		case KEY_KP3:
			if(numlock(mod))
				return "3\0";
			break;
		case KEY_KP4:
			if(numlock(mod))
				return "4\0";
			break;
		case KEY_KP5:
			if(numlock(mod))
				return "5\0";
			break;
		case KEY_KP6:
			if(numlock(mod))
				return "6\0";
			break;
		case KEY_KP7:
			if(numlock(mod))
				return "7\0";
			break;
		case KEY_KP8:
			if(numlock(mod))
				return "8\0";
			break;
		case KEY_KP9:
			if(numlock(mod))
				return "9\0";
			break;
		case KEY_KP_PERIOD:
			return ".\0";
			break;
		case KEY_KP_DIVIDE:
			return "/\0";
			break;
		case KEY_KP_MULTIPLY:
			return "*\0";
			break;
		case KEY_KP_MINUS:
			return "-\0";
			break;
		case KEY_KP_PLUS:
			return "+\0";
			break;
		case KEY_KP_ENTER:
			return "\n\0";
			break;
		case KEY_KP_EQUALS:
			return "=\0";
			break;

		default:
			return "\0";
			break;
	}

	return "\0";
}


bool KeyTranslator::shift(KeyModifier mod)
{
	return ((mod & KEY_MOD_SHIFT) || (mod & KEY_MOD_CAPS));
}


bool KeyTranslator::numlock(KeyModifier mod)
{
	return (mod & KEY_MOD_NUM) != 0;
}


bool KeyTranslator::control(KeyModifier mod)
{
	return (mod & KEY_MOD_CTRL) != 0;
}
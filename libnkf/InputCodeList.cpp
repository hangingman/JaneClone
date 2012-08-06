/*
 * InputCodeList.cpp
 *
 *  Created on: 2012/07/17
 * Contributor: Hiroyuki Nagata
 */

#include "InputCodeList.h"

/**
 * 指定された文字コードごとのメソッドを提供する
 */
void InputCode::StatusFunc(InputCode* inputCode, nkf_char c,
		std::bitset<nkf_flag_num> nkfFlags) {
	if (this->name == "EUC-JP") {
		EStatus(inputCode, c, nkfFlags);
	} else if (this->name == "Shift_JIS") {
		SStatus(inputCode, c, nkfFlags);
	} else if (this->name == "UTF-8") {
		WStatus(inputCode, c, nkfFlags);
	} else {
		// do nothing
	}
}

nkf_char InputCode::IconvFunc(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags) {
	if (this->name == "EUC-JP") {
		return EIconv(c2, c1, c0);
	} else if (this->name == "Shift_JIS") {
		return SIconv(c2, c1, c0);
	} else if (this->name == "UTF-8") {
		return WIconv(c2, c1, c0);
	} else if (this->name == "UTF-16") {
		return WIconv16(c2, c1, c0);
	} else if (this->name == "UTF-32") {
		return WIconv32(c2, c1, c0);
	} else {
		// do nothing
	}
}

void InputCode::EStatus(InputCode* inputCode, nkf_char c,
		std::bitset<nkf_flag_num> nkfFlags) {
	switch (inputCode->stat) {
	case -1:
		InputCode::StatusCheck(inputCode, c, nkfFlags);
		break;
	case 0:
		if (c <= DEL) {
			break;
		} else if (nkf_char_unicode_p(c)) {
			break;
		} else if (SS2 == c || (0xa1 <= c && c <= 0xfe)) {
			inputCode->stat = 1;
			StatusPushCh(inputCode, c);
		} else if (0x8f == c) {
			inputCode->stat = 2;
			StatusPushCh(inputCode, c);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	case 1:
		if (0xa1 <= c && c <= 0xfe) {
			StatusPushCh(inputCode, c);
			CodeScore(inputCode, nkfFlags);
			StatusClear(inputCode);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	case 2:
		if (0xa1 <= c && c <= 0xfe) {
			inputCode->stat = 1;
			StatusPushCh(inputCode, c);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
	}
}

void InputCode::SStatus(InputCode* inputCode, nkf_char c,
		std::bitset<nkf_flag_num> nkfFlags) {
	switch (inputCode->stat) {
	case -1:
		InputCode::StatusCheck(inputCode, c, nkfFlags);
		break;
	case 0:
		if (c <= DEL) {
			break;
		} else if (nkf_char_unicode_p(c)) {
			break;
		} else if (0xa1 <= c && c <= 0xdf) {
			StatusPushCh(inputCode, SS2);
			StatusPushCh(inputCode, c);
			CodeScore(inputCode, nkfFlags);
			InputCode::StatusClear(inputCode);
		} else if ((0x81 <= c && c < 0xa0) || (0xe0 <= c && c <= 0xea)) {
			inputCode->stat = 1;
			StatusPushCh(inputCode, c);
		} else if (0xed <= c && c <= 0xee) {
			inputCode->stat = 3;
			StatusPushCh(inputCode, c);
		} else if (is_ibmext_in_sjis(c)) {
			inputCode->stat = 2;
			StatusPushCh(inputCode, c);
		} else if (0xf0 <= c && c <= 0xfc) {
			inputCode->stat = 1;
			StatusPushCh(inputCode, c);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	case 1:
		if ((0x40 <= c && c <= 0x7e) || (0x80 <= c && c <= 0xfc)) {
			StatusPushCh(inputCode, c);
			Util::S2eConv(inputCode->buf[0], inputCode->buf[1],
					&inputCode->buf[0], &inputCode->buf[1], nkfFlags);
			CodeScore(inputCode, nkfFlags);
			InputCode::StatusClear(inputCode);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	case 2:
		if ((0x40 <= c && c <= 0x7e) || (0x80 <= c && c <= 0xfc)) {
			StatusPushCh(inputCode, c);
			if (Util::S2eConv(inputCode->buf[0], inputCode->buf[1],
					&inputCode->buf[0], &inputCode->buf[1], nkfFlags) == 0) {
				SetCodeScore(inputCode, SCORE_CP932);
				InputCode::StatusClear(inputCode);
				break;
			}
		}
		StatusDisable(inputCode, nkfFlags);
		break;
	case 3:
		if ((0x40 <= c && c <= 0x7e) || (0x80 <= c && c <= 0xfc)) {
			StatusPushCh(inputCode, c);
			Util::S2eConv(inputCode->buf[0], inputCode->buf[1],
					&inputCode->buf[0], &inputCode->buf[1], nkfFlags);
			SetCodeScore(inputCode, SCORE_CP932);
			InputCode::StatusClear(inputCode);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	}
}

void InputCode::WStatus(InputCode* inputCode, nkf_char c,
		std::bitset<nkf_flag_num> nkfFlags) {

	switch (inputCode->stat) {
	case -1:
		StatusCheck(inputCode, c, nkfFlags);
		break;
	case 0:
		if (c <= DEL) {
			break;
		} else if (nkf_char_unicode_p(c)) {
			break;
		} else if (0xc0 <= c && c <= 0xdf) {
			inputCode->stat = 1;
			StatusPushCh(inputCode, c);
		} else if (0xe0 <= c && c <= 0xef) {
			inputCode->stat = 2;
			StatusPushCh(inputCode, c);
		} else if (0xf0 <= c && c <= 0xf4) {
			inputCode->stat = 3;
			StatusPushCh(inputCode, c);
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	case 1:
	case 2:
		if (0x80 <= c && c <= 0xbf) {
			StatusPushCh(inputCode, c);
			if (inputCode->index > inputCode->stat) {
				int bom = (inputCode->buf[0] == 0xef
						&& inputCode->buf[1] == 0xbb
						&& inputCode->buf[2] == 0xbf);
				Util::W2eConv(inputCode->buf[0], inputCode->buf[1],
						inputCode->buf[2], &inputCode->buf[0],
						&inputCode->buf[1], nkfFlags);
				if (!bom) {
					CodeScore(inputCode, nkfFlags);
				}
				StatusClear(inputCode);
			}
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	case 3:
		if (0x80 <= c && c <= 0xbf) {
			if (inputCode->index < inputCode->stat) {
				StatusPushCh(inputCode, c);
			} else {
				StatusClear(inputCode);
			}
		} else {
			StatusDisable(inputCode, nkfFlags);
		}
		break;
	}
}

nkf_char InputCode::SIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::EIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::WIconv(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0) {
}

nkf_char InputCode::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0) {
}

void InputCode::StatusCheck(InputCode* ptr, nkf_char c,
		std::bitset<nkf_flag_num> nkfFlags) {
	if (c <= DEL && nkfFlags[estab_f]) {
		InputCode::StatusReset(ptr);
	}
}

void InputCode::StatusReset(InputCode* ptr) {
	InputCode::StatusClear(ptr);
	ptr->score = SCORE_INIT;
}

void InputCode::StatusClear(InputCode* ptr) {
	ptr->stat = 0;
	ptr->index = 0;
}

void InputCode::StatusPushCh(InputCode* ptr, nkf_char c) {
	ptr->buf[ptr->index++] = c;
}

void InputCode::StatusDisable(InputCode* ptr,
		std::bitset<nkf_flag_num> nkfFlags) {
	ptr->stat = -1;
	ptr->buf[0] = -1;
	CodeScore(ptr, nkfFlags);
//	if (LibNKF::inputEncoding->baseEncoding->name == ptr->name) {
//		GuessConv::SetIconv(FALSE, 0, nkfFlags);
//	}
}

void InputCode::CodeScore(InputCode* ptr, std::bitset<nkf_flag_num> nkfFlags) {
	nkf_char c2 = ptr->buf[0];
	nkf_char c1 = ptr->buf[1];

	if (c2 < 0) {
		SetCodeScore(ptr, SCORE_ERROR);
	} else if (c2 == SS2) {
		SetCodeScore(ptr, SCORE_KANA);
	} else if (c2 == 0x8f) {
		SetCodeScore(ptr, SCORE_X0212);
	} else if (!Util::E2wConv(c2, c1, nkfFlags)) {
		SetCodeScore(ptr, SCORE_NO_EXIST);
	} else if ((c2 & 0x70) == 0x20) {
		SetCodeScore(ptr, score_table_A0[c2 & 0x0f]);
	} else if ((c2 & 0x70) == 0x70) {
		SetCodeScore(ptr, score_table_F0[c2 & 0x0f]);
	} else if ((c2 & 0x70) >= 0x50) {
		SetCodeScore(ptr, SCORE_L2);
	}
}
/**
 * 関数名からInputCode名を引く
 */
InputCode* InputCode::FindInputcodeByFunc(std::string funcName) {

	InputCode* inputCode;

	if (funcName == "e_iconv") {
		inputCode->name = "EUC-JP";
	} else if (funcName == "s_iconv") {
		inputCode->name = "Shift_JIS";
	} else if (funcName == "w_iconv") {
		inputCode->name = "UTF-8";
	} else {
		// do nothing
	}

	return inputCode;
}
/**
 * Scoreの設定を行う
 */
void InputCode::ClrCodeScore(InputCode* ptr, nkf_char score) {
	if (ptr) {
		ptr->score &= ~score;
	}
}

void InputCode::SetCodeScore(InputCode* ptr, nkf_char score) {
	if (ptr) {
		ptr->score |= score;
	}
}

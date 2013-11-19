#include "inputcode.h"
#include "wxnkfencoding.h"

/**
 * same as nkf's code_status...
 */
void InputCode::Status(InputCode* inputCode, nkf_char c)
{
     if (strcmp(inputCode->name, "EUC-JP") == 0)
     {
	  InputCode::EStatus(inputCode, c);
     }
     else if (strcmp(inputCode->name, "Shift_JIS") == 0)
     {
	  InputCode::SStatus(inputCode, c);
     }
     else if (strcmp(inputCode->name, "UTF-8") == 0)
     {
	  InputCode::WStatus(inputCode, c);
     }
     else
     {
	  // do nothing
     }
}

/**
 * EUC-JP status
 */
void InputCode::EStatus(InputCode *ptr, nkf_char c)
{
     switch (ptr->stat){
     case -1:
	  StatusCheck(ptr, c);
	  break;
     case 0:
	  if (c <= DEL){
	       break;
	  }else if (nkf_char_unicode_p(c)){
	       break;
	  }else if (SS2 == c || (0xa1 <= c && c <= 0xfe)){
	       ptr->stat = 1;
	       StatusPushCh(ptr, c);
	  }else if (0x8f == c){
	       ptr->stat = 2;
	       StatusPushCh(ptr, c);
	  }else{
	       StatusDisable(ptr);
	  }
	  break;
     case 1:
	  if (0xa1 <= c && c <= 0xfe){
	       StatusPushCh(ptr, c);
	       CodeScore(ptr);
	       StatusClear(ptr);
	  }else{
	       StatusDisable(ptr);
	  }
	  break;
     case 2:
	  if (0xa1 <= c && c <= 0xfe){
	       ptr->stat = 1;
	       StatusPushCh(ptr, c);
	  }else{
	       StatusDisable(ptr);
	  }
     }    
}

/**
 * Shift_JIS status
 */
void InputCode::SStatus(InputCode *ptr, nkf_char c)
{
     switch(ptr->stat){
     case -1:
	  StatusCheck(ptr, c);
	  break;
     case 0:
	  if (c <= DEL){
	       break;
	  }else if (nkf_char_unicode_p(c)){
	       break;
	  }else if (0xa1 <= c && c <= 0xdf){
	       StatusPushCh(ptr, SS2);
	       StatusPushCh(ptr, c);
	       CodeScore(ptr);
	       StatusClear(ptr);
	  }else if ((0x81 <= c && c < 0xa0) || (0xe0 <= c && c <= 0xea)){
	       ptr->stat = 1;
	       StatusPushCh(ptr, c);
	  }else if (0xed <= c && c <= 0xee){
	       ptr->stat = 3;
	       StatusPushCh(ptr, c);
	  }else if (is_ibmext_in_sjis(c)){
	       ptr->stat = 2;
	       StatusPushCh(ptr, c);
	  }else if (0xf0 <= c && c <= 0xfc){
	       ptr->stat = 1;
	       StatusPushCh(ptr, c);
	  }else{
	       StatusDisable(ptr);
	  }
	  break;
     case 1:
	  if ((0x40 <= c && c <= 0x7e) || (0x80 <= c && c <= 0xfc)){
	       StatusPushCh(ptr, c);
	       Util::S2eConv(ptr->buf[0], ptr->buf[1], &ptr->buf[0], &ptr->buf[1], NULL);
	       CodeScore(ptr);
	       StatusClear(ptr);
	  }else{
	       StatusDisable(ptr);
	  }
	  break;
     case 2:
	  if ((0x40 <= c && c <= 0x7e) || (0x80 <= c && c <= 0xfc)) {
	       StatusPushCh(ptr, c);
	       if ( Util::S2eConv(ptr->buf[0], ptr->buf[1], &ptr->buf[0], &ptr->buf[1], NULL) == 0 ) {
		    SetCodeScore(ptr, SCORE_CP932);
		    StatusClear(ptr);
		    break;
	       }
	  }
	  StatusDisable(ptr);
	  break;
     case 3:
	  if ((0x40 <= c && c <= 0x7e) || (0x80 <= c && c <= 0xfc)){
	       StatusPushCh(ptr, c);
	       Util::S2eConv(ptr->buf[0], ptr->buf[1], &ptr->buf[0], &ptr->buf[1], NULL);
	       SetCodeScore(ptr, SCORE_CP932);
	       StatusClear(ptr);
	  }else{
	       StatusDisable(ptr);
	  }
	  break;
     }    
}

/**
 * UTF-8 status
 */
void InputCode::WStatus(InputCode *ptr, nkf_char c)
{
     switch (ptr->stat){
     case -1:
	  StatusCheck(ptr, c);
	  break;
     case 0:
	  if (c <= DEL){
	       break;
	  }else if (nkf_char_unicode_p(c)){
	       break;
	  }else if (0xc0 <= c && c <= 0xdf){
	       ptr->stat = 1;
	       StatusPushCh(ptr, c);
	  }else if (0xe0 <= c && c <= 0xef){
	       ptr->stat = 2;
	       StatusPushCh(ptr, c);
	  }else if (0xf0 <= c && c <= 0xf4){
	       ptr->stat = 3;
	       StatusPushCh(ptr, c);
	  }else{
	       StatusDisable(ptr);
	  }
	  break;
     case 1:
     case 2:
	  if (0x80 <= c && c <= 0xbf)
	  {
	       StatusPushCh(ptr, c);
	       if (ptr->index > ptr->stat)
	       {

		    int bom = (ptr->buf[0] == 0xef 
			       && ptr->buf[1] == 0xbb
			       && ptr->buf[2] == 0xbf);

		    Util::W2eConv(ptr->buf[0], 
				  ptr->buf[1], 
				  ptr->buf[2],
				  &ptr->buf[0], 
				  &ptr->buf[1],
				  NULL);
		    if (!bom)
		    {
			 CodeScore(ptr);
		    }
  
		    StatusClear(ptr);
	       }
	  }
	  else
	  {
	       StatusDisable(ptr);
	  }
	  break;

     case 3:
	  if (0x80 <= c && c <= 0xbf)
	  {
	       if (ptr->index < ptr->stat)
	       {
		    StatusPushCh(ptr, c);
	       } 
	       else 
	       {
		    StatusClear(ptr);
	       }
	  }
	  else
	  {
	       StatusDisable(ptr);
	  }
	  break;
     }
}

void InputCode::StatusClear(InputCode* ptr)
{
     ptr->stat = 0;
     ptr->index = 0;
}

void InputCode::StatusPushCh(InputCode* ptr, nkf_char c)
{
     ptr->buf[ptr->index++] = c;
}

void InputCode::StatusReset(InputCode* ptr)
{
     StatusClear(ptr);
     ptr->score = SCORE_INIT;
}

void InputCode::SetCodeScore(InputCode* ptr, nkf_char score)
{
     if (ptr){
	  ptr->score |= score;
     }
}

void InputCode::CodeScore(InputCode* ptr)
{
     nkf_char c2 = ptr->buf[0];
     nkf_char c1 = ptr->buf[1];
     if (c2 < 0){
	  SetCodeScore(ptr, SCORE_ERROR);
     }else if (c2 == SS2){
	  SetCodeScore(ptr, SCORE_KANA);
     }else if (c2 == 0x8f){
	  if ((c1 & 0x70) == 0x20){
	       SetCodeScore(ptr, score_table_8FA0[c1 & 0x0f]);
	  }else if ((c1 & 0x70) == 0x60){
	       SetCodeScore(ptr, score_table_8FE0[c1 & 0x0f]);
	  }else if ((c1 & 0x70) == 0x70){
	       SetCodeScore(ptr, score_table_8FF0[c1 & 0x0f]);
	  }else{
	       SetCodeScore(ptr, SCORE_X0212);
	  }
     }else if ( !Util::E2wConv(c2, c1, NULL) ){
	  SetCodeScore(ptr, SCORE_NO_EXIST);
     }else if ((c2 & 0x70) == 0x20){
	  SetCodeScore(ptr, score_table_A0[c2 & 0x0f]);
     }else if ((c2 & 0x70) == 0x70){
	  SetCodeScore(ptr, score_table_F0[c2 & 0x0f]);
     }else if ((c2 & 0x70) >= 0x50){
	  SetCodeScore(ptr, SCORE_L2);
     }
}

void InputCode::StatusCheck(InputCode* ptr, nkf_char c)
{
     //TODO: hand over estab_f from main class
     //if (c <= DEL && estab_f){
     if (c <= DEL)
     {
	  StatusReset(ptr);
     }
}

void InputCode::StatusDisable(InputCode* ptr)
{
    ptr->stat = -1;
    ptr->buf[0] = -1;
    CodeScore(ptr);
    //if (iconv == ptr->iconv_func) set_iconv(FALSE, 0);
}

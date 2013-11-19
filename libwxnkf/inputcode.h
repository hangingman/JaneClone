#ifndef INPUTCODE_H_
#define INPUTCODE_H_

#include <string.h>
#include "utf8table.h"
#include "util.h"
#include "wxnkfbaseconfig.h"

/**
 * for CodeStatus macros
 */
#define SCORE_L2       (1)                   /* Kanji Level 2 */
#define SCORE_KANA     (SCORE_L2 << 1)       /* Halfwidth Katakana */
#define SCORE_DEPEND   (SCORE_KANA << 1)     /* MD Characters */
#define SCORE_CP932    (SCORE_DEPEND << 1)   /* IBM extended characters */
#define SCORE_X0212    (SCORE_CP932 << 1)    /* JIS X 0212 */
#define SCORE_X0213    (SCORE_X0212 << 1)    /* JIS X 0213 */
#define SCORE_NO_EXIST (SCORE_X0213 << 1)    /* Undefined Characters */
#define SCORE_iMIME    (SCORE_NO_EXIST << 1) /* MIME selected */
#define SCORE_ERROR    (SCORE_iMIME << 1)    /* Error */

#define SCORE_INIT (SCORE_iMIME)

static const nkf_char score_table_A0[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, SCORE_DEPEND, SCORE_DEPEND, SCORE_DEPEND,
    SCORE_DEPEND, SCORE_DEPEND, SCORE_DEPEND, SCORE_X0213,
};

static const nkf_char score_table_F0[] = {
    SCORE_L2, SCORE_L2, SCORE_L2, SCORE_L2,
    SCORE_L2, SCORE_DEPEND, SCORE_X0213, SCORE_X0213,
    SCORE_DEPEND, SCORE_DEPEND, SCORE_CP932, SCORE_CP932,
    SCORE_CP932, SCORE_X0213, SCORE_X0213, SCORE_ERROR,
};

static const nkf_char score_table_8FA0[] = {
    0, SCORE_X0213, SCORE_X0212, SCORE_X0213,
    SCORE_X0213, SCORE_X0213, SCORE_X0212, SCORE_X0212,
    SCORE_X0213, SCORE_X0212, SCORE_X0212, SCORE_X0212,
    SCORE_X0213, SCORE_X0213, SCORE_X0213, SCORE_X0213,
};

static const nkf_char score_table_8FE0[] = {
    SCORE_X0212, SCORE_X0212, SCORE_X0212, SCORE_X0212,
    SCORE_X0212, SCORE_X0212, SCORE_X0212, SCORE_X0212,
    SCORE_X0212, SCORE_X0212, SCORE_X0212, SCORE_X0212,
    SCORE_X0212, SCORE_X0212, SCORE_X0213, SCORE_X0213,
};

static const nkf_char score_table_8FF0[] = {
    SCORE_X0213, SCORE_X0213, SCORE_X0213, SCORE_X0212,
    SCORE_X0212, SCORE_X0213, SCORE_X0213, SCORE_X0213,
    SCORE_X0213, SCORE_X0213, SCORE_X0213, SCORE_X0213,
    SCORE_X0213, SCORE_X0213, SCORE_X0213, SCORE_X0213,
};

/**
 * This class used when nkf_char code status is unknown.
 */
class InputCode {

 public:
  InputCode(){};
  InputCode(const char* name, nkf_char stat, nkf_char score, nkf_char index,
	    nkf_char buf[], int _file_stat)
    {
      this->name = name;
      this->stat  = stat;
      this->score = score;
      this->index = index;
      this->buf[0] = buf[0];
      this->buf[1] = buf[1];
      this->buf[2] = buf[2];
      this->_file_stat = _file_stat;
    };

  /**
   * same as nkf's code_status...
   */
  static void Status(InputCode* inputCode, nkf_char c);

  static void StatusClear(InputCode* ptr);
  static void StatusPushCh(InputCode* ptr, nkf_char c);
  static void StatusReset(InputCode* ptr);
  static void SetCodeScore(InputCode* ptr, nkf_char score);
  static void CodeScore(InputCode* ptr);
  static void StatusCheck(InputCode* ptr, nkf_char c);
  static void StatusDisable(InputCode* ptr);

  const char* getName()
  {
    this->name;
  };

  nkf_char getStat()
  {
    this->stat;
  };

  nkf_char getScore()
  {
    this->score;
  };

  nkf_char getIndex()
  {
    this->index;
  };

 private:

  /**
   * EUC-JP status
   */
  static void EStatus(InputCode *ptr, nkf_char c);
  /**
   * Shift_JIS status
   */
  static void SStatus(InputCode *ptr, nkf_char c);
  /**
   * UTF-8 status
   */
  static void WStatus(InputCode *ptr, nkf_char c);

  const char *name;
  nkf_char stat;
  nkf_char score;
  nkf_char index;
  nkf_char buf[3];
  int _file_stat;
};

#endif /** INPUTCODE_H_ */

/*
 * DataType.h
 *
 *  Created on: 2012/01/05
 *      Author: learning
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_

// wxHashMap用のクラス -- 2chの板名とURLを対応させる
class URLvsBoardName
{
public:
  wxString BoardName;
  wxString BoardURL;
  wxString BoardNameAscii;
};

// スレッド一覧用のクラス
class ThreadList
{
public:
  // キー値(ホスト名にこの番号をつけることでスレッドのURLになる)
  wxString oid;
  // 番号(単に取得したdatファイルの順序から)
  int number;
  // タイトル
  wxString title;
  // レス
  int response;
  // 取得
  int cachedResponseNumber;
  // 新着
  int newResponseNumber;
  // 増レス
  int increaseResponseNumber;
  // 勢い
  int momentum;
  // 最終取得
  wxString lastUpdate;
  // since
  wxString since;
  // 板
  wxString boardName;
};

#endif /* DATATYPE_H_ */

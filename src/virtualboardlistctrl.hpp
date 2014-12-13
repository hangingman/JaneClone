/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012-2014 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#ifndef VIRTUALBOARDLISTCTRL_HPP_
#define VIRTUALBOARDLISTCTRL_HPP_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/regex.h>
#include <wx/txtstrm.h>
#include <vector>
#include <algorithm>
#include "datatype.hpp"
#include "janecloneutil.hpp"

/**
 * スレッドの状態
 */


static const int THREAD_STATE_NORMAL = -1;
static const int THREAD_STATE_CHECK  =  0;
static const int THREAD_STATE_ADD    =  1;
static const int THREAD_STATE_DROP   =  2;
static const int THREAD_STATE_NEW    =  3;

class VirtualBoardListItem;

/**
 * 仮想リストの１項目分
 */
class VirtualBoardListItem {

public:
     /**
      * 仮想リストの１アイテムを表すオブジェクトのコンストラクタ
      */
     VirtualBoardListItem(const wxString& number, const wxString& title, const wxString& response, 
			  const wxString& cachedResponseNumber, const wxString& newResponseNumber, const wxString& increaseResponseNumber,
			  const wxString& momentum, const wxString& lastUpdate, const wxString& since,
			  const wxString& oid, const wxString& boardName) {
	       
	  // 各項目を設定する
	  m_number                 = number;
	  m_title		   = title; 
	  m_response		   = response;
	  m_cachedResponseNumber   = cachedResponseNumber;
	  m_newResponseNumber	   = newResponseNumber;
	  m_increaseResponseNumber = increaseResponseNumber;
	  m_momentum               = momentum;
	  m_lastUpdate             = lastUpdate;
	  m_since                  = since;
	  m_oid                    = oid;
	  m_boardName              = boardName;
     };

     // ゲッターとセッター
     int getCheck() const {
	  return m_check;
     };
     void setCheck(int chk) {
	  m_check = chk;
     };
     wxString getNumber() const {
	  return m_number;
     };
     wxString getTitle() const {
	  return m_title;
     };
     wxString getResponse() const {
	  return m_response;
     };
     wxString getCachedResponseNumber() const {
	  return m_cachedResponseNumber;
     };
     void setCachedResponseNumber(wxString& cachedResponseNumber) {
	  m_cachedResponseNumber = cachedResponseNumber;
     };
     wxString getNewResponseNumber() const {
	  return m_newResponseNumber;
     };
     void setNewResponseNumber(wxString& newResponseNumber) {
	  m_newResponseNumber = newResponseNumber;
     };
     wxString getIncreaseResponseNumber() const {
	  return m_increaseResponseNumber;
     };
     void setIncreaseResponseNumber(wxString& increaseResponseNumber) {
	  m_increaseResponseNumber = increaseResponseNumber;
     };
     wxString getMomentum() const {
	  return m_momentum;
     };
     wxString getLastUpdate() const {
	  return m_lastUpdate;
     };
     wxString getSince() const {
	  return m_since;
     };
     wxString getOid() const {
	  return m_oid;
     };
     wxString getBoardName() const {
	  return m_boardName;
     };

     /** 新月向け情報 */
     void setFilename(wxString& filename) {
	  m_filename = filename;
     };
     wxString getFilename() const {
	  return m_filename;
     };
     
     // それぞれのカラムをソートするPreidicate関数     

     // ! Fix Me ! 繰り返しが多すぎる。もう少し効率的に書ける方法を探すべき
     static bool PredicateForwardCheck(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getCheck() < rItem.getCheck();
     };

     static bool PredicateReverseCheck(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getCheck() > rItem.getCheck();
     };

     static bool PredicateForwardNumber(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getNumber()) < wxAtoi(rItem.getNumber());
     };
     
     static bool PredicateReverseNumber(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getNumber()) > wxAtoi(rItem.getNumber());
     };
     
     static bool PredicateForwardTitle(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getTitle() > rItem.getTitle();
     };

     static bool PredicateReverseTitle(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getTitle() < rItem.getTitle();
     };

     static bool PredicateForwardResponse(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getResponse()) < wxAtoi(rItem.getResponse());
     };
     
     static bool PredicateReverseResponse(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getResponse()) > wxAtoi(rItem.getResponse());
     };

     static bool PredicateForwardCachedResponseNumber(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getCachedResponseNumber()) < wxAtoi(rItem.getCachedResponseNumber());
     };
     
     static bool PredicateReverseCachedResponseNumber(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getCachedResponseNumber()) > wxAtoi(rItem.getCachedResponseNumber());
     };

     static bool PredicateForwardNewResponseNumber(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getNewResponseNumber()) < wxAtoi(rItem.getNewResponseNumber());
     };
     
     static bool PredicateReverseNewResponseNumber(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getNewResponseNumber()) > wxAtoi(rItem.getNewResponseNumber());
     };

     static bool PredicateForwardIncreaseResNum(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getIncreaseResponseNumber()) < wxAtoi(rItem.getIncreaseResponseNumber());
     };
     
     static bool PredicateReverseIncreaseResNum(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getIncreaseResponseNumber()) > wxAtoi(rItem.getIncreaseResponseNumber());
     };

     static bool PredicateForwardMomentum(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getMomentum()) < wxAtoi(rItem.getMomentum());
     };
     
     static bool PredicateReverseMomentum(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getMomentum()) > wxAtoi(rItem.getMomentum());
     };

     static bool PredicateForwardLastUpdate(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getLastUpdate() < rItem.getLastUpdate();
     };
     
     static bool PredicateReverseLastUpdate(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getLastUpdate() > rItem.getLastUpdate();
     };

     static bool PredicateForwardSince(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getSince() < rItem.getSince();
     };
     
     static bool PredicateReverseSince(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getSince() > rItem.getSince();
     };

     static bool PredicateForwardOid(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getOid()) < wxAtoi(rItem.getOid());
     };
     
     static bool PredicateReverseOid(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return wxAtoi(lItem.getOid()) > wxAtoi(rItem.getOid());
     };

     static bool PredicateForwardBoardName(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getBoardName() < rItem.getBoardName();
     };
     
     static bool PredicateReverseBoardName(const VirtualBoardListItem& lItem, const VirtualBoardListItem& rItem){
	  return lItem.getBoardName() > rItem.getBoardName();
     };

     /**
      * 文字列検索用叙述関数
      */
     // static bool PredSearchString(const VirtualBoardListItem& item) {
     // 	  return item.getTitle.Contains(
     // };


private:
     /** 新月向け情報 */

     // 正式なファイル名
     wxString m_filename;

private:
     // 新着チェック
     int m_check;
     // 番号(単に取得したdatファイルの順序から)
     wxString m_number;
     // タイトル
     wxString m_title;
     // レス
     wxString m_response;
     // 取得
     wxString m_cachedResponseNumber;
     // 新着
     wxString m_newResponseNumber;
     // 増レス
     wxString m_increaseResponseNumber;
     // 勢い
     wxString m_momentum;
     // 最終取得
     wxString m_lastUpdate;
     // since
     wxString m_since;
     // 固有番号(ホスト名にこの番号をつけることでスレッドのURLになる)
     wxString m_oid;
     // 板
     wxString m_boardName;

};

typedef std::vector<VirtualBoardListItem> VirtualBoardList;

/**
 * スレッド一覧リスト用の仮想リストコントロール
 * 大量のデータを扱うときは仮想リストを使用する
 */
class VirtualBoardListCtrl: public wxListCtrl {

public:
     /**
      * デフォルトコンストラクタ
      */
     VirtualBoardListCtrl() {
	  f_nowSearching = false;
     };
     VirtualBoardListCtrl(const VirtualBoardListCtrl&) {
	  f_nowSearching = false;
     };
     /**
      * コンストラクタ：配置するwindowと板名を指定
      * @param wxWindow*  parent                      親ウィンドウ
      * @param wxWindowID id                          ウィンドウID
      * @param wxString   boardName                   板名
      * @param wxString   outputPath                  datファイルのパス
      * @param map<wxString,ThreadList>& oldThreadMap 古いスレッドの情報を保持するコンテナ
      * @rapam bool     targetIsShingetsu             読み込み対象は新月のCSVか
      */
     VirtualBoardListCtrl(wxWindow* parent,
			  const wxWindowID id,
			  const wxString& boardName, 
			  const wxString& outputPath, 
			  const std::map<wxString,ThreadList>& oldThreadMap, 
			  bool targetIsShingetsu = false);
     /**
      * 2chのdatファイルを読み出す処理
      */
     void FileLoadMethod2ch(const wxString& boardName, const wxString& outputPath, 
			    const std::map<wxString,ThreadList>& oldThreadMap);

#ifdef USE_SHINGETSU
     /**
      * 新月ののcsvファイルを読み出す処理
      */
     void FileLoadMethodShingetsu(const wxString& boardName, const wxString& outputPath, 
				  const std::map<wxString,ThreadList>& oldThreadMap);
#endif /** USE_SHINGETSU */
     /**
      * コンストラクタ：ログ一覧リスト作成用
      * @param wxWindow* parent     親ウィンドウ
      * @param wxString boardName   板名(ログ一覧で固定)
      * @param wxString outputPath  datファイルのパス
      */
     VirtualBoardListCtrl(wxWindow* parent, const wxString& boardName, const wxArrayString& datFileList);
     /**
      * 内部リストの更新処理
      * @param wxString boardName   板名
      * @pram  wxString outputPath  datファイルのパス
      * @param VirtualBoardList     更新したリストのコンテナ
      */
     VirtualBoardList ThreadListUpdate(const wxString& boardName, const wxString& outputPath);
     /**
      * オペレーターに対する参照返し
      */
     VirtualBoardListCtrl& operator=(const VirtualBoardListCtrl&) {return *this;}
     /**
      * 指定されたアイテムとカラムに存在するテキストを返す
      * @param long item            インデックス
      * @param long column          カラム
      * @return wxString text       セル内のテキスト
      */
     wxString OnGetItemText(long item, long column) const;
     /**
      * 内部のリストをソートする
      */
     void SortVectorItems(int col);
     /**
      * 内部のリストをコピーして渡す
      */
     void CopyVectorItems(VirtualBoardList& vBoardList) {

	  //先にメモリ領域を確保する
	  vBoardList.reserve(m_vBoardList.size());
	  std::copy(m_vBoardList.begin(), m_vBoardList.end(), std::back_inserter(vBoardList));
     };
     /**
      * スレッドタイトル検索を実施する
      */
     void SearchAndSortItems(const wxString& keyword);
     /**
      * リスト内部のアイテムの数を返す
      */
     long GetItemCount() {
	  return m_vBoardList.size();
     }
     
     // 内部にあるリスト
     VirtualBoardList m_vBoardList;

     // リスト内部の列挙型
     enum Columns {
	  COL_CHK = 0,  // 新着チェック
	  COL_NUM, 	// 番号
	  COL_TITLE,	// タイトル
	  COL_RESP,	// レス
	  COL_CACHEDRES,// 取得
	  COL_NEWRESP,	// 新着
	  COL_INCRESP,	// 増レス
	  COL_MOMENTUM,	// 勢い
	  COL_LASTUP,	// 最終取得
	  COL_SINCE,	// SINCE
	  COL_OID,      // 固有番号
	  COL_BOARDNAME,// 板
	  COL_ENUMS_END // 列挙型がいくつあるか
     };


private:

     /**
      * 仮想リスト内のアイコンを表示させる
      */
     virtual int OnGetItemColumnImage(long item, long column) const;
     /**
      * 仮想リスト内の色情報等の設定
      */
     virtual wxListItemAttr* OnGetItemAttr(long item) const;
     /**
      *
      */
     long GetItemData(long item) const {
	  return wxListCtrl::GetItemData(item);
     };

     // 新着チェック
     bool f_check;
     // 番号
     bool f_number;
     // タイトル
     bool f_title;
     // レス
     bool f_response;
     // 取得
     bool f_cachedResponseNumber;
     // 新着
     bool f_newResponseNumber;
     // 増レス
     bool f_increaseResponseNumber;
     // 勢い
     bool f_momentum;
     // 最終取得
     bool f_lastUpdate;
     // since
     bool f_since;
     // 固有番号(ホスト名にこの番号をつけることでスレッドのURLになる)
     bool f_oid;
     // 板
     bool f_boardName;

     // 内部の色情報等
     wxListItemAttr m_attr;
     wxListItemAttr m_attr_search;

     // 検索を行なっているかどうか
     bool f_nowSearching;
     // 検索に当てはまった項目の数
     long searchItemNum;

     DECLARE_DYNAMIC_CLASS(VirtualBoardListCtrl)
};

#endif /* VIRTUALBOARDLISTCTRL_HPP_ */

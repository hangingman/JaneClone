/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
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

#ifndef ZMQCOMMUNICATION_HPP_
#define ZMQCOMMUNICATION_HPP_

#include "datatype.hpp"
#include "janecloneutil.hpp"
#include <zmq.hpp>

class ZMQCommunication {

public:

     /**
      * 指定された新月公開ノードホストにぶら下がるスレッド一覧リストをダウンロードしてくるメソッド
      * もし前回通信した際のログが残っていれば更新の確認のみ行う
      * @param  新月公開ノードのホスト名
      * @return 実行コード
      */
     int DownloadShingetsuThreadList(const wxString shingetsuHostname);
     /**
      * ログ出力用のウィンドウを設定する
      */
     void SetLogWindow(wxTextCtrl* logCtrl) {
	  this->m_logCtrl = logCtrl;
     };

private:
     /**
      * ログとして出力するためのテキストコントロールのポインタ
      */
     wxTextCtrl* m_logCtrl;

};

#endif /* ZMQCOMMUNICATION_HPP_ */

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

#include "zmqcommunication.hpp"

/**
 * 指定された新月公開ノードホストにぶら下がるスレッド一覧リストをダウンロードしてくるメソッド
 * もし前回通信した際のログが残っていれば更新の確認のみ行う
 * @param  新月公開ノードのホスト名
 * @return 実行コード
 * 
 * 新月のAPIについては：http://shingetsu.info/saku/api
 */
int ZMQCommunication::DownloadShingetsuThreadList(const wxString shingetsuHostname) {

     // URIから各パラメーターを抜き取る
     PartOfURI* uri = new PartOfURI;
     bool urlIsSane = JaneCloneUtil::SubstringURI(shingetsuHostname, uri);
     const wxString protocol = uri->protocol;
     const wxString hostname = uri->hostname;
     const wxString port = uri->port;
     const wxString path = uri->path;
     delete uri;

     if (!urlIsSane) return false;

     // 保存対象のディレクトリが存在するか確かめる
     wxString shingetsu = ::wxGetHomeDir()
	  + wxFileSeparator 
	  + JANECLONE_DIR 
	  + wxFileSeparator 
	  + wxT("shingetsu");

     wxDir chkDir(shingetsu);
     JaneCloneUtil::CreateSpecifyDirectory(chkDir, hostname);

     /**
     // 変数の準備
     zmq::context_t context (1);
     zmq::socket_t socket (context, ZMQ_REQ);
     // zmqを使って接続開始
     *m_logCtrl << wxT("Connecting to shingetsu server...");
     std::string dest = "tcp://" + std::string(hostname.mb_str()) + ":" + std::string(port.mb_str());
     socket.connect(dest.c_str());

     zmq::message_t request (6);
     memcpy ((void *) request.data (), "Hello", 5);
     std::cout << "Sending Hello " << request_nbr << "..." << std::endl;
     socket.send (request);

     //  Get the reply.
     zmq::message_t reply;
     socket.recv (&reply);
     std::cout << "Received World " << request_nbr << std::endl;
     */
     return 0;
}

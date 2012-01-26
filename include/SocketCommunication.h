/*
 * SocketCommunication.h
 *
 *  Created on: 2012/01/04
 *      Author: learning
 */

#ifndef SOCKETCOMMUNICATION_H_
#define SOCKETCOMMUNICATION_H_

#include <wx/wx.h>
#include <iostream>
#include <fstream>
#include "gimite/socket.h"

class SocketCommunication {

public:
	// スレッド一覧をダウンロードしてくるメソッド 　引数はサーバーのフルURL、サーバ名、板名、保存先
	// うまくいけばtrueを返す
	bool DownloadThreadList(wxString& boardURL, wxString& server, wxString& boardName, wxString outputPath, wxString headerPath);
};

#endif /* SOCKETCOMMUNICATION_H_ */

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

#ifndef FINDINDICATEDRESPONSE_H_
#define FINDINDICATEDRESPONSE_H_

#include <wx/wx.h>
#include <libxml/HTMLparser.h>

using namespace std;

class FindIndicatedResponse {

public:
	/**
	 * コンストラクタ
	 */
	FindIndicatedResponse(const wxString* allSource, wxString& resNumber);
	/**
	 * HTMLをパースした結果を渡す処理
	 */
	wxString GetIndicatedResponse();
};

#endif /* FINDINDICATEDRESPONSE_H_ */

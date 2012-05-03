/*
 * SQLiteAccessor.h
 *
 *  Created on: 2012/04/17
 *      Author: learning
 */

#ifndef SQLITEACCESSOR_H_
#define SQLITEACCESSOR_H_

#include <iostream>
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/wxsqlite3.h>

class SQLiteAccessor: public wxSQLite3Database {
public:
	SQLiteAccessor();
	void SetBoardInfo(const wxString, const wxString, const wxString);
	void SetCommit();
};

#endif /* SQLITEACCESSOR_H_ */

/*
 * SQLiteBundle.h
 *
 *  Created on: 2012/01/09
 *      Author: learning
 */

#ifndef SQLITEBUNDLE_H_
#define SQLITEBUNDLE_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/wxsqlite3.h"

/*
 * SQLiteBundleクラス
 * wxSQLite3Databaseを継承しユーザーが以前に見た板名
 * スレなどを逐次記録する。
 */

class SQLiteBundle : public wxSQLite3Database {

public:
  SQLiteBundle();

private:

protected:

};

#endif /* SQLITEBUNDLE_H_ */

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

#include <wx/image.h>
#include <wx/snglinst.h>
#include <wx/fs_mem.h>
#include "janeclone.hpp"

#ifdef __WXMSW__

#include <wx/cmdline.h>

/** 再起動処理のために引数が与えられた場合使用する */
static const wxCmdLineEntryDesc gCmdLineDesc[] =
{
     // コマンドラインオプションに -p or pid=xxx と入力された場合
#if wxCHECK_VERSION(2, 9, 0)
     { wxCMD_LINE_OPTION, "p", "pid", "past worked JaneClone pid", 
#else
       { wxCMD_LINE_OPTION, wxT("p"), wxT("pid"), wxT("past worked JaneClone pid"),
#endif
	 wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL },
       { wxCMD_LINE_NONE }
     };
#endif

/*
 * wxAppを継承したwxMainを宣言
 */
class wxMain: public wxApp {

  wxLocale m_Locale;

public:
  wxMain() : m_Locale(wxLANGUAGE_DEFAULT){}
  virtual bool OnInit();
  virtual int OnExit();

private:
  virtual int FilterEvent(wxEvent& event);

  // 再起動時のコマンドライン読み込み用
#ifdef __WXMSW__
  virtual int OnRun();
  virtual void OnInitCmdLine(wxCmdLineParser& parser);
  virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
#endif

  wxSingleInstanceChecker* m_checker;
  JaneClone* wxJaneClone;
  long m_pid;
};

IMPLEMENT_APP(wxMain)

/**
 * wxMainの実装
 */
bool wxMain::OnInit() {

    if (!wxApp::OnInit())
	 return false;
     
    // コマンドラインで与えられた引数を取得する
    if (m_pid != 0 && m_pid == wxGetProcessId()) {
	 // このプロセスをしばらく待機
	 // ! Fix Me ! なんかここ実装しなくても動作してるけど…
    }
     
    // JaneClone起動前に複数起動をチェックする
    const wxString name = wxString::Format(_("JaneClone-%s"), wxGetUserId().c_str());
    m_checker = new wxSingleInstanceChecker(name);
    if ( m_checker->IsAnotherRunning()) {
	 wxMessageBox(wxT("誤作動防止のためJaneCloneは複数起動できません。終了します。"), 
	    wxT("JaneClone起動"), wxOK | wxICON_ERROR);
	 return false;
    }
     
    wxInitAllImageHandlers();
    wxImage::AddHandler( new wxPNGHandler );
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxJaneClone = new JaneClone(NULL, wxID_ANY, wxEmptyString);
    wxJaneClone->pid = 0;
    SetTopWindow(wxJaneClone);
    wxJaneClone->Show();
    return true;
}
/**
 * 終了後の後始末
 */
int wxMain::OnExit() {

     unsigned long pid = wxJaneClone->pid;

     if (pid != 0) {
	  // 0でなければ再起動処理を行う & このプロセスは殺す
	  wxString execute = wxGetCwd() + wxFileSeparator + wxT("JaneClone") + wxExt;
	  ::wxExecute(execute + wxString::Format(_(" -p %lu"), pid), wxEXEC_ASYNC, NULL);
     }

     delete m_checker;

     return 0;
}
/**
 * キーイベントをここで判断する
 */
int wxMain::FilterEvent(wxEvent& event) {

    if (event.GetEventType() == wxEVT_KEY_DOWN) 
    {
	  wxKeyEvent e = dynamic_cast<wxKeyEvent&>(event);

	  if(e.GetModifiers() == wxMOD_CONTROL) 
	  {
	       // Ctrlキー同時押し
	       bool ret = -1;
	       switch(e.GetKeyCode()) 
	       {
	       case 'F':
		    wxJaneClone->CtrlF(e);
		    return true;
		    break;
	       default:
		    break;
	       }

	       return -1;
	  } 
	  else
	  {
	       // 普通のキー
	       bool ret = -1;
	       switch(e.GetKeyCode()) 
	       {
	       case WXK_RETURN:
		    ret = wxJaneClone->Enter(e);
		    return ret;
		    break;
	       default:
		    break;
	       }

	       return -1;
	  }

	  return -1;
    }
 
    return -1;
}

#ifdef __WXMSW__
/**
 * コンソールを走らせるために必要？
 */
int wxMain::OnRun() {
    int exitcode = wxApp::OnRun();
    if (exitcode!=0)
        return exitcode;
}
/**
 * コマンドライン読み取りを初期化する
 */
void wxMain::OnInitCmdLine(wxCmdLineParser& parser) {
     // オプションの始まりはハイフン
     parser.SetDesc(gCmdLineDesc);
     parser.SetSwitchChars(wxT("-"));
}
/**
 * コマンドラインからパラメーターを読み取る
 */
bool wxMain::OnCmdLineParsed(wxCmdLineParser& parser) {
     // JaneCloneがPIDつきで呼び出された場合
     parser.Found(wxT("p"), &m_pid);
     return true;
}
#endif

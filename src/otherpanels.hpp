﻿/* JaneClone - a text board site viewer for 2ch
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
 *	Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

// -*- C++ -*- generated by wxGlade 0.6.5 on Wed May 22 03:50:59 2013

#ifndef OTHERPANELS_HPP
#define OTHERPANELS_HPP

#include <utility>
#include <tuple>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/spinctrl.h>
#include <wx/filepicker.h>
#include <wx/colordlg.h>
#include <wx/fontdlg.h>
#include <wx/html/htmlwin.h>
#include <wx/dynarray.h>

#if wxCHECK_VERSION(2, 9, 1)
   #include <wx/generic/stattextg.h>
#endif

#include "enums.hpp"
#include "janecloneutil.hpp"
#include "drawpane.hpp"
#include "sqliteaccessor.hpp"
#include "socketcommunication.hpp"

// begin wxGlade: ::extracode
// end wxGlade

// Utility class definition
typedef std::tuple<wxString, wxControl*> WxCtrlTuple;
typedef std::vector<WxCtrlTuple>         WxCtrlTupleList;

/**
 * 設定画面の基底クラス
 */
class SettingPanelBase: public wxPanel {

public:
     SettingPanelBase(wxWindow* parent,
                      wxWindowID id = wxID_ANY,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = wxTAB_TRAVERSAL,
                      const wxString& name = wxPanelNameStr):
          wxPanel(parent, id, pos, size, style, name)
          {
               this->tupleList = {};
          };

     // パネルに設定されたプロパティをプロパティファイルに一括保存する
     virtual void save_properties() {};
     virtual void set_properties() {};

protected:
     virtual void do_layout() {};

     WxCtrlTupleList tupleList;
     void GetWidgetsProperties();
     void SetWidgetsProperties();
};


/**
 * 各種ネットワーク設定用画面
 */
class NetworkSettingPanel: public SettingPanelBase {

public:
     // begin wxGlade: NetworkSettingPanel::ids
     // end wxGlade
     NetworkSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition,
			 const wxSize& size=wxDefaultSize, long style=0);
     void save_properties() override;

#ifdef __WXMAC__
     // リソースの更新を行う
     void UpdateResources() {
	  wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
	  proxyUseCheck = new wxCheckBox(panel_6, wxID_ANY, wxT("Proxyを使用する"));
	  proxyCacheUseCheck = new wxCheckBox(panel_6, wxID_ANY, wxT("Proxy使用時にキャッシュを使用しない"));
	  sizer_6->Add(proxyUseCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	  sizer_6->Add(proxyCacheUseCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	  panel_6->SetSizer(sizer_6);
     };
#endif

private:
     // begin wxGlade: NetworkSettingPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

#ifdef __WXMAC__
     // メインのスレッドにイベントを送る
     void SendUIUpdateEvent() {
	  wxCommandEvent* event = new wxCommandEvent(wxEVT_UPDATE_UI, ID_NetworkPanelUpdate);
	  wxString ui = wxT("NetworkSettingPanel");
	  event->SetString(ui.c_str());
	  event->SetEventObject(this);

   #if wxCHECK_VERSION(2, 9, 0)
	  wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event->Clone());
   #else
	  this->GetEventHandler()->AddPendingEvent(*event);
   #endif
     };
#endif

protected:
     // begin wxGlade: NetworkSettingPanel::attributes
     wxCheckBox* proxyUseCheck;
     wxCheckBox* proxyCacheUseCheck;
     wxPanel* panel_6;
     wxStaticText* label_6;
     wxStaticText* label_7;
     wxStaticText* label_8;
     wxTextCtrl* recProxyAddrTC;
     wxTextCtrl* recProxyPortTC;
     wxStaticText* label_9;
     wxTextCtrl* sedProxyAddrTC;
     wxTextCtrl* sedProxyPortTC;
     wxStaticText* label_10;
     wxTextCtrl* authSSLAddrTC;
     wxTextCtrl* authSSLPortTC;
     wxStaticText* label_11;
     wxStaticText* label_12;
     wxTextCtrl* basicAuthUserNameTC;
     wxPanel* panel_8;
     wxStaticText* label_13;
     wxTextCtrl* basicAuthPasswordTC;
     wxPanel* panel_9;
     wxPanel* panel_7;
     wxPanel* panel_1;
     wxStaticText* label_4;
     wxTextCtrl* receiveTimeoutTC;
     wxPanel* panel_2;
     wxStaticText* label_5;
     wxTextCtrl* connectTimeoutTC;
     wxPanel* panel_3;
     wxStaticText* label_3;
     wxTextCtrl* boardListURLTC;
     wxPanel* panel_4;
     wxStaticText* label_1;
     wxTextCtrl* receiveBufferSizeTC;
     wxStaticText* label_2;
     wxSpinCtrl* maxConnSC;
     wxPanel* panel_5;
     wxPanel* panel_10;
     wxStaticText* label_14;
     wxTextCtrl* userAgentTC;
     // end wxGlade
}; // wxGlade: end class

/**
 * 各種パス設定用画面
 */
class PathSettingPanel: public SettingPanelBase {
public:
     // begin wxGlade: PathSettingPanel::ids
     // end wxGlade
     PathSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: PathSettingPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade
     void OnBrowserCheck(wxCommandEvent& event) {
	  // デフォルトのブラウザを指定する
	  JaneCloneUtil::SetJaneCloneProperties<bool>(wxT("SPECIFY_DEFAULT_BROWSER"), browserCheck->IsChecked());
	  this->set_properties();
     };
     void OnChangeDirPickerBrowser(wxFileDirPickerEvent& event) {
	  // ブラウザのパスを更新する
	  JaneCloneUtil::SetJaneCloneProperties<wxString>(wxT("DEFAULT_BROWSER_PATH"), event.GetPath());
	  this->set_properties();
     };
     void OnChangeDirPickerBoardList(wxFileDirPickerEvent& event) {
	  // 板一覧のパスを更新する
	  JaneCloneUtil::SetJaneCloneProperties<wxString>(wxT("DEFAULT_BOARDLIST_PATH"), event.GetPath());
	  this->set_properties();
     };
     void OnChangeDirPickerSkin(wxFileDirPickerEvent& event) {
	  // スキンのパスを更新する
	  JaneCloneUtil::SetJaneCloneProperties<wxString>(wxT("DEFAULT_SKINFILE_PATH"), event.GetPath());
	  this->set_properties();
     };
     void ClearPathSetting(wxCommandEvent& event) {
	  // パス設定をクリアする
	  const int id = event.GetId();

	  if (id == ID_ClearBrowserPath) {
	       JaneCloneUtil::DeleteJaneClonePropertyEntry(wxT("DEFAULT_BROWSER_PATH"));
	  } else if (id == ID_ClearBoardListPath) {
	       JaneCloneUtil::DeleteJaneClonePropertyEntry(wxT("DEFAULT_BOARDLIST_PATH"));
	  } else if (id == ID_ClearSkinPath) {
	       JaneCloneUtil::DeleteJaneClonePropertyEntry(wxT("DEFAULT_SKINFILE_PATH"));
	  }
	  this->set_properties();
     };

protected:
     // begin wxGlade: PathSettingPanel::attributes
     wxCheckBox* browserCheck;
     wxStaticText* label_1;
     wxDirPickerCtrl* dirPicker1;
     wxButton* clearButton_1;
     wxStaticText* label_2;
     wxDirPickerCtrl* dirPicker2;
     wxButton* clearButton_2;
     wxStaticText* label_3;
     wxDirPickerCtrl* dirPicker3;
     wxButton* clearButton_3;

#ifdef __WXGTK__
     wxTextCtrl* browserPathTC;
     wxTextCtrl* boardListTC;
     wxTextCtrl* skinPathTC;
#endif

     // end wxGlade
     DECLARE_EVENT_TABLE()
}; // wxGlade: end class

// begin wxGlade: ::extracode
// end wxGlade

/**
 * 各種動作設定用画面
 */
class BehaviorPanel: public SettingPanelBase {
public:
    // begin wxGlade: BehaviorPanel::ids
    // end wxGlade

    BehaviorPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: BehaviorPanel::methods
    void set_properties() override;
    void do_layout() override;
    // end wxGlade

protected:
    // begin wxGlade: BehaviorPanel::attributes
    wxStaticBox* sizer_7_staticbox;
    wxStaticBox* sizer_5_staticbox;
    wxStaticBox* sizer_3_staticbox;
    wxStaticBox* sizer_2_staticbox;
    wxCheckBox* openBNewTabCheck;
    wxCheckBox* onlyOneCategoryCheck;
    wxPanel* panel_2;
    wxCheckBox* openTNewTabCheck;
    wxComboBox* threadNewTabCombo;
    wxPanel* panel_6;
    wxCheckBox* lastReadThreadPosCheck;
    wxPanel* panel_3;
    wxRadioBox* threadJumpSettingRadio;
    wxCheckBox* redrawNewThreadCheck;
    wxStaticText* label_1;
    wxComboBox* showResponseCombo;
    wxPanel* panel_7;
    wxPanel* panel_4;
    wxCheckBox* favoriteNewTabCheck;
    wxCheckBox* favoriteOnlyOneFolderCheck;
    wxPanel* panel_5;
    // end wxGlade
}; // wxGlade: end class

/**
 * 各種操作設定用画面
 */
// begin wxGlade: ::extracode
// end wxGlade
class OperationPanel: public SettingPanelBase {

public:
     // begin wxGlade: OperationPanel::ids
     // end wxGlade
     OperationPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: OperationPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

protected:
     // begin wxGlade: OperationPanel::attributes
     wxStaticBox* sizer_7_staticbox;
     wxStaticBox* sizer_6_staticbox;
     wxStaticBox* sizer_5_staticbox;
     wxStaticBox* sizer_4_staticbox;
     wxCheckBox* checkbox_1;
     wxCheckBox* checkbox_2;
     wxCheckBox* checkbox_3;
     wxStaticText* label_1;
     wxComboBox* combo_box_1;
     wxPanel* panel_8;
     wxStaticText* label_2;
     wxComboBox* combo_box_2;
     wxPanel* panel_9;
     wxStaticText* label_3;
     wxComboBox* combo_box_3;
     wxPanel* panel_10;
     wxStaticText* label_4;
     wxComboBox* combo_box_4;
     wxPanel* panel_11;
     wxPanel* panel_4;
     wxCheckBox* checkbox_4;
     wxCheckBox* checkbox_5;
     wxCheckBox* checkbox_6;
     wxCheckBox* checkbox_7;
     wxCheckBox* checkbox_8;
     wxCheckBox* checkbox_9;
     wxPanel* panel_5;
     wxPanel* panel_2;
     wxStaticText* label_1_copy;
     wxComboBox* combo_box_1_copy;
     wxPanel* panel_8_copy;
     wxStaticText* label_2_copy;
     wxComboBox* combo_box_2_copy;
     wxPanel* panel_9_copy;
     wxStaticText* label_3_copy;
     wxComboBox* combo_box_3_copy;
     wxPanel* panel_10_copy;
     wxStaticText* label_4_copy;
     wxComboBox* combo_box_4_copy;
     wxPanel* panel_11_copy;
     wxPanel* panel_6;
     wxStaticText* label_5;
     wxSpinCtrl* spin_ctrl_1;
     wxPanel* panel_12;
     wxStaticText* label_6;
     wxSpinCtrl* spin_ctrl_2;
     wxPanel* panel_13;
     wxStaticText* label_7;
     wxSpinCtrl* spin_ctrl_3;
     wxPanel* panel_14;
     wxPanel* panel_7;
     wxPanel* panel_3;
     // end wxGlade
}; // wxGlade: end class

/**
 * 書き込み設定用画面
 */
// begin wxGlade: ::extracode
// end wxGlade

class KakikomiPanel: public SettingPanelBase {
public:
     // begin wxGlade: KakikomiPanel::ids
     // end wxGlade
     KakikomiPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: KakikomiPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

protected:
     // begin wxGlade: KakikomiPanel::attributes
     wxCheckBox* checkbox_1;
     wxCheckBox* checkbox_2;
     wxCheckBox* checkbox_3;
     wxCheckBox* checkbox_4;
     wxCheckBox* checkbox_5;
     wxCheckBox* checkbox_6;
     wxCheckBox* checkbox_7;
     wxCheckBox* checkbox_8;
     wxCheckBox* checkbox_9;
     wxCheckBox* checkbox_10;
     wxCheckBox* checkbox_11;
     wxCheckBox* checkbox_12;
     wxCheckBox* checkbox_13;
     wxPanel* panel_2;
     wxStaticText* label_1;
     wxTextCtrl* text_ctrl_1;
     wxStaticText* label_2;
     wxSpinCtrl* spin_ctrl_1;
     wxPanel* panel_4;
     wxStaticText* label_3;
     wxComboBox* combo_box_1;
     wxPanel* panel_5;
     wxStaticText* label_4;
     wxCheckBox* checkbox_14;
     wxPanel* panel_9;
     wxTextCtrl* text_ctrl_2;
     wxPanel* panel_7;
     wxStaticText* label_5;
     wxCheckBox* checkbox_15;
     wxPanel* panel_10;
     wxTextCtrl* text_ctrl_3;
     wxPanel* panel_8;
     wxPanel* panel_6;
     wxPanel* panel_3;
     // end wxGlade
}; // wxGlade: end class


// begin wxGlade: ::extracode
// end wxGlade

/**
 * スレ表示欄の設定画面
 */
class DoePanel: public SettingPanelBase {

public:
     // begin wxGlade: DoePanel::ids
     // end wxGlade
     DoePanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: DoePanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

protected:
     // begin wxGlade: DoePanel::attributes
     wxStaticBox* sizer_3_staticbox;
     wxStaticText* label_1;
     wxTextCtrl* text_ctrl_1;
     wxPanel* panel_2;
     wxRadioButton* radio_btn_1;
     wxPanel* panel_10;
     wxTextCtrl* text_ctrl_2;
     wxStaticText* label_2;
     wxSpinCtrl* spin_ctrl_1;
     wxStaticText* label_3;
     wxSpinCtrl* spin_ctrl_2;
     wxPanel* panel_9;
     wxRadioButton* radio_btn_2;
     wxCheckBox* checkbox_1;
     wxPanel* panel_3;
     wxCheckBox* checkbox_2;
     wxPanel* panel_4;
     wxCheckBox* checkbox_3;
     wxPanel* panel_5;
     wxStaticText* label_4;
     wxPanel* panel_6;
     wxTextCtrl* text_ctrl_3;
     wxTextCtrl* text_ctrl_4;
     wxTextCtrl* text_ctrl_5;
     wxTextCtrl* text_ctrl_6;
     wxTextCtrl* text_ctrl_7;
     wxPanel* panel_7;
     wxStaticText* label_5;
     wxButton* button_1;
     wxPanel* panel_8;
     // end wxGlade
}; // wxGlade: end class

// begin wxGlade: ::extracode
// end wxGlade

/**
 * その他の設定画面１
 */
class OtherSettingPanelOne: public SettingPanelBase {

public:
     OtherSettingPanelOne(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);
     const static wxString checkboxlabels[];

private:
     void set_properties() override;
     void do_layout() override;

protected:
     // チェックボックス
     wxCheckBox* checkboxes[16];
     wxPanel* panels[16];

     // 残りのwidgets
     wxSpinCtrl* retainBoardSC;
     wxStaticText* retainBoardST;

     wxTextCtrl* glanceTC;
     wxStaticText* glanceSC;

     wxSpinCtrl* autoSaveSC;
     wxStaticText* autoSaveST;

     wxPanel* restPanels[3];
};

/**
 * ユーザー設定設定用画面
 */
class UserSettingPanel: public SettingPanelBase {
public:
     // begin wxGlade: UserSettingPanel::ids
     // end wxGlade
     UserSettingPanel(wxWindow* parent,
		      const wxPoint& pos=wxDefaultPosition,
		      const wxSize& size=wxDefaultSize,
		      long style=0);
     void save_properties() override;

private:
     // begin wxGlade: UserSettingPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

     // ユーザーがリンクをクリックした場合ブラウザでジャンプ
     void OnLinkClocked(wxHtmlLinkEvent& event)
	  {
	       const wxHtmlLinkInfo linkInfo = event.GetLinkInfo();
	       const wxString href = linkInfo.GetHref();
	       wxLaunchDefaultBrowser(href);
	  };

protected:
     // begin wxGlade: UserSettingPanel::attributes
     wxStaticBox* sizer_3_staticbox;
     wxStaticBox* sizer_2_staticbox;
     wxHtmlWindow* window_1;
     wxStaticText* label_1;
     wxTextCtrl* maruUserID;
     wxPanel* panel_8;
     wxStaticText* label_2;
     wxTextCtrl* maruUserPassword;
     wxPanel* panel_9;
     wxCheckBox* maruAutoLoginCheck;
     wxButton* maruLogoutButton;
     wxPanel* panel_5;
     wxPanel* panel_2;
     wxHtmlWindow* window_2;
     wxStaticText* label_3;
     wxTextCtrl* beMailAddress;
     wxPanel* panel_6;
     wxStaticText* label_4;
     wxTextCtrl* bePassword;
     wxPanel* panel_7;
     wxPanel* panel_3;
     // end wxGlade
     DECLARE_EVENT_TABLE()
}; // wxGlade: end class

/**
 * 色・フォント設定用画面
 */
class ColorFontSettingPanel: public SettingPanelBase {

public:
     // begin wxGlade: ColorFontSettingPanel::ids
     // end wxGlade
     ColorFontSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

#ifdef __WXMAC__
     // リソースの更新を行う
     void UpdateResources() {

	  // 内部で保存されたIDを元に文字列を生成する
	  const std::string &str = EnumString<JANECLONE_ENUMS>::From( static_cast<JANECLONE_ENUMS>(m_id) );
	  const wxString bgColor = wxString((const char*)str.c_str(), wxConvUTF8);

	  // 背景色を変更する必要があるかどうかのフラグ
	  bool needToChangeBGColor = false;

	  wxColourData data;
	  data.SetChooseFull(true);
	  for (int i = 0; i < 16; i++)
	  {
	       wxColour colour(i*16, i*16, i*16);
	       data.SetCustomColour(i, colour);
	  }

	  wxColourDialog dialog(this, &data);
	  if (dialog.ShowModal() == wxID_OK)
	  {
	       wxColourData retData = dialog.GetColourData();
	       wxColour col = retData.GetColour();
	       // 結果を受け取る
	       const wxString colorInfo = col.GetAsString();
	       // フォント,色情報 の順でプロパティファイルに格納
	       JaneCloneUtil::SetJaneCloneProperties(bgColor, colorInfo);
	       needToChangeBGColor = true;
	  }

	  if (needToChangeBGColor)
	  {
	       SetSampleBGColorSetting(m_id);
	  }

	  return;
     };
#endif


private:
     // begin wxGlade: ColorFontSettingPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

     // 色選択用ダイアログを表示させる
     void OnClickColorFontSettingButton(wxCommandEvent& event);
     // 各部位のフォントを設定し、プロパティファイルに書き出す
     bool SetEachFontSetting(const wxString& font);
     // 色・フォント設定用画面のサンプル部分のフォントを変更する
     void SetSampleFontSetting(const int id);
     // 各部位の背景色を設定し、プロパティファイルに書き出す
     bool SetEachBGColorSetting(const wxString& bgColor);
     // 色・フォント設定用画面のサンプル部分の背景色を変更する
     void SetSampleBGColorSetting(const int id);

#ifdef __WXMAC__
     // メインのスレッドにイベントを送る
     void SendUIUpdateEvent() {
	  wxCommandEvent* event = new wxCommandEvent(wxEVT_UPDATE_UI, ID_SettingPanelUpdate);
	  wxString ui = wxT("ColorFontSettingPanel");
	  event->SetString(ui.c_str());
	  event->SetEventObject(this);

   #if wxCHECK_VERSION(2, 9, 0)
	  wxTheApp->GetTopWindow()->GetEventHandler()->QueueEvent(event->Clone());
   #else
	  this->GetEventHandler()->AddPendingEvent(*event);
   #endif
     };
#endif


protected:
     // begin wxGlade: ColorFontSettingPanel::attributes

     /**
      * サンプルのテキスト：全部で11ある
      */
#if wxCHECK_VERSION(2, 9, 1)
     wxGenericStaticText* treeSampleLabel;
     wxGenericStaticText* threadListSampleLabel;
     wxGenericStaticText* extractSampleLabel;
     wxGenericStaticText* logWindowSampleLabel;
     wxGenericStaticText* threadTitleSampleLabel;
     wxGenericStaticText* threadViewSampleLabel;
     wxGenericStaticText* kakikoSampleLabel;
     wxGenericStaticText* memoSampleLabel;
     wxGenericStaticText* hintSampleLabel;
     wxGenericStaticText* linkSampleLabel;
     wxGenericStaticText* othersSampleLabel;
#else
     BasicDrawPane* treeSampleLabel;
     BasicDrawPane* threadListSampleLabel;
     BasicDrawPane* extractSampleLabel;
     BasicDrawPane* logWindowSampleLabel;
     BasicDrawPane* threadTitleSampleLabel;
     BasicDrawPane* threadViewSampleLabel;
     BasicDrawPane* kakikoSampleLabel;
     BasicDrawPane* memoSampleLabel;
     BasicDrawPane* hintSampleLabel;
     BasicDrawPane* linkSampleLabel;
     BasicDrawPane* othersSampleLabel;
#endif

     wxStaticBox* sizer_6_staticbox;
     wxStaticBox* sizer_5_staticbox;
     wxStaticBox* sizer_4_staticbox;
     wxPanel* panel_7;
     wxPanel* panel_8;
     wxPanel* panel_9;
     wxPanel* panel_4;
     wxButton* treeFontButton;
     wxButton* threadListFontButton;
     wxButton* extractFontButton;
     wxPanel* panel_10;
     wxButton* logWindowFontButton;
     wxButton* threadTitleFontButton;
     wxButton* kakikoFontButton;
     wxButton* memoFontButton;
     wxPanel* panel_11;
     wxButton* hintFontButton;
     wxButton* linkFontButton;
     wxPanel* panel_12;
     wxButton* otherFontButton;
     wxButton* allFontButton;
     wxPanel* panel_5;
     wxButton* treeBGColorButton;
     wxButton* threadListBGColorButton;
     wxButton* logWindowBGColorButton;
     wxButton* threadTitleBGColorButton;
     wxButton* threadViewBGColorButton;
     wxButton* memoBGColorButton;
     wxButton* hintBGColorButton;
     wxButton* popupBGColorButton_copy_1;
     wxPanel* panel_13;
     wxButton* allBGColorButton;
     wxPanel* panel_6;
     wxPanel* panel_2;
     wxStaticText* label_1;
     wxPanel* panel_3;
     // end wxGlade

     // 変数一時保存のため宣言
     int m_id;

     DECLARE_EVENT_TABLE()
}; // wxGlade: end class


// begin wxGlade: ::extracode
// end wxGlade

/**
 * タブ色設定用画面
 */
class TabColorSettingPanel: public SettingPanelBase {

public:
     // begin wxGlade: TabColorSettingPanel::ids
     // end wxGlade
     TabColorSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: TabColorSettingPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

     // タブ色設定用画面のボタンイベントを処理する
     void OnClickTabColorSettingButton(wxCommandEvent& event);
     // 各部位の背景色を設定し、プロパティファイルに書き出す
     bool SetEachBGColorSetting(const wxString& bgColor);
     // タブ色設定用画面のサンプル部分の背景色を変更する
     void SetSampleBGColorSetting(const int id);
     // 色・フォント設定用画面のサンプル部分のフォント色を変更する
     bool SetEachFontColorSetting(const wxString& fontColor);
     // タブ色設定用画面のサンプル部分のフォント色を変更する
     void SetSampleFontColorSetting(const int id);

protected:
     // begin wxGlade: TabColorSettingPanel::attributes

     /**
      * サンプル用テキスト：全部で13
      */
#if wxCHECK_VERSION(2, 9, 1)
     wxGenericStaticText* defaultActiveTabSampleLabel;
     wxGenericStaticText* readingActiveTabSampleLabel;
     wxGenericStaticText* brokenActiveTabSampleLabel;
     wxGenericStaticText* cannotPostActiveTabSampleLabel;
     wxGenericStaticText* updateActiveTabSampleLabel;
     wxGenericStaticText* partialContentActiveTabSampleLabel;
     wxGenericStaticText* defaultDeactiveTabSampleLabel;
     wxGenericStaticText* readingDeactiveTabSampleLabel;
     wxGenericStaticText* brokenDeactiveTabSampleLabel;
     wxGenericStaticText* cannotPostDeactiveTabSampleLabel;
     wxGenericStaticText* updateDeactiveTabSampleLabel;
     wxGenericStaticText* partialContentDeactiveTabSampleLabel;
     wxGenericStaticText* autoReloadSampleLabel;
#else
     BasicDrawPane* defaultActiveTabSampleLabel;
     BasicDrawPane* readingActiveTabSampleLabel;
     BasicDrawPane* brokenActiveTabSampleLabel;
     BasicDrawPane* cannotPostActiveTabSampleLabel;
     BasicDrawPane* updateActiveTabSampleLabel;
     BasicDrawPane* partialContentActiveTabSampleLabel;
     BasicDrawPane* defaultDeactiveTabSampleLabel;
     BasicDrawPane* readingDeactiveTabSampleLabel;
     BasicDrawPane* brokenDeactiveTabSampleLabel;
     BasicDrawPane* cannotPostDeactiveTabSampleLabel;
     BasicDrawPane* updateDeactiveTabSampleLabel;
     BasicDrawPane* partialContentDeactiveTabSampleLabel;
     BasicDrawPane* autoReloadSampleLabel;
#endif

     wxButton* activeTabBGColorButton;
     wxButton* threadTabDefaultFontColorButton;
     wxButton* threadTabReadingFontColorButton;
     wxButton* threadTabBrokenFontColorButton;
     wxButton* threadTabCannotPostFontColorButton;
     wxButton* threadTabUpdateFontColorButton;
     wxButton* threadTabPartialContentFontColorButton;
     wxButton* deactiveTabBGColorButton;
     wxButton* autoReloadFontColorButton;

     wxStaticBox* sizer_6_staticbox;
     wxStaticBox* sizer_5_staticbox;
     wxCheckBox* threadTabColorCheck;
     wxPanel* panel_6;
     wxPanel* panel_7;
     wxPanel* panel_9;
     wxPanel* panel_8;
     wxPanel* panel_5;
     wxPanel* panel_4;
     wxPanel* panel_3;
     wxPanel* panel_2;
     // end wxGlade

     DECLARE_EVENT_TABLE()
}; // wxGlade: end class



/**
 * タブ操作設定用画面
 */
class TabControlSettingPanel: public SettingPanelBase {

public:
     // begin wxGlade: TabControlSettingPanel::ids
     // end wxGlade
     TabControlSettingPanel(wxWindow* parent, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
     // begin wxGlade: TabControlSettingPanel::methods
     void set_properties() override;
     void do_layout() override;
     // end wxGlade

protected:
     // begin wxGlade: TabControlSettingPanel::attributes
     wxStaticBox* sizer_4_staticbox;
     wxStaticBox* sizer_3_staticbox;
     wxStaticText* label_1;
     wxChoice* choice_1;
     wxStaticText* label_2;
     wxChoice* choice_2;
     wxPanel* panel_6;
     wxPanel* panel_4;
     wxStaticText* label_3;
     wxChoice* choice_3;
     wxStaticText* label_4;
     wxChoice* choice_4;
     wxPanel* panel_7;
     wxPanel* panel_5;
     // end wxGlade
}; // wxGlade: end class

/**
 * 外部板登録ダイアログ
 */
class NewBoardAddDialog: public wxDialog {

public:
     // begin wxGlade: NewBoardAddDialog::ids
     // end wxGlade
     NewBoardAddDialog(wxWindow* parent,
		       int id,
		       const wxString& title,
		       const wxPoint& pos=wxDefaultPosition,
		       const wxSize& size=wxDefaultSize,
		       long style=wxDEFAULT_DIALOG_STYLE);

private:
     // begin wxGlade: NewBoardAddDialog::methods
     void set_properties();
     void do_layout();
     // end wxGlade

     void OnClickOK(wxCommandEvent& event);
     void OnClickCancel(wxCommandEvent& event);
     void CheckBoardUrl(wxCommandEvent& event);
     void GetShitarabaBoardInfo(wxCommandEvent& event);

protected:
     // begin wxGlade: NewBoardAddDialog::attributes
     wxStaticBox* sizer_3_staticbox;
     wxStaticText* label_1;
     wxTextCtrl* text_ctrl_1;
     wxPanel* panel_4;
     wxStaticText* label_2;
     wxTextCtrl* text_ctrl_2;
     wxButton* button_3;
     wxPanel* panel_5;
     wxStaticText* label_3;
     wxComboBox* combo_box_1;
     wxPanel* panel_7;
     wxPanel* panel_6;
     wxPanel* panel_3;
     wxPanel* spacePanel;
     wxButton* button_1;
     wxButton* button_2;
     wxPanel* panel_1;
     // end wxGlade
}; // wxGlade: end class

#endif // OTHERPANELS_H

/**
 * janecloneimageviewer.hpp -  janeclone's plain imageviewer
 *
 * Copyright (C) 2013-2014 Hiroyuki Nagata <idiotpanzer@gmail.com>
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
#ifndef JANECLONEIMAGEVIEWER_HPP_
#define JANECLONEIMAGEVIEWER_HPP_

#include <wx/wx.h>
#include <wx/aui/auibook.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/clipbrd.h>
#include <wx/event.h>
#include "enums.hpp"
#include "datatype.hpp"
#include "imagepanel.hpp"
#include "janecloneuiutil.hpp"

/**
 * 画像のサムネイルと本体の表示を行うウィンドウ
 */
class JaneCloneImageViewer : public wxFrame {

public:

    /// constructor
    JaneCloneImageViewer(wxWindow* parent, int id, const wxString& title,
                         const wxPoint& pos = wxDefaultPosition, const wxSize& size =
                         wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);

    // 画像ファイルの情報を設定する
    void SetImageFile(std::unique_ptr<DownloadImageResult>& result);
    // ウィンドウ上で右クリックした場合の処理
    void OnRightClickImageViewer(wxContextMenuEvent& event);

private:
    /// Default constructor
    JaneCloneImageViewer();
    ~JaneCloneImageViewer();

    void set_properties(const wxString& title);
    void do_layout();

    // Auiの管理オブジェクト
    wxAuiManager m_mgr;
    // 画像を載せるノートブック
    wxAuiNotebook* thumbnailNoteBook;

    /// Copy constructor
    JaneCloneImageViewer(const JaneCloneImageViewer& rhs);
    /// Assignment operator
    JaneCloneImageViewer& operator=(const JaneCloneImageViewer& rhs);

    // ウィンドウを閉じるイベント
    void OnClose(wxCloseEvent& event);
    // 画像ビューアを開いた状態でマウスホイールを動かした場合のイベント
    void OnMouseWheel(wxMouseEvent& event);
    // 画像タブをひとつ閉じる
    void OneThumbnailTabClose(wxCommandEvent& event);
    // すべての画像タブを閉じる
    void AllThumbnailTabClose(wxCommandEvent& event);
    // これより左の画像タブをを閉じる
    void AllLeftThumbnailTabClose(wxCommandEvent& event);
    // これより右の画像タブを閉じる
    void AllRightThumbnailTabClose(wxCommandEvent& event);
    // 左の画像タブに移動
    void SelectLeftThumbnailTab(wxCommandEvent& event);
    // 右の画像タブに移動
    void SelectRightThumbnailTab(wxCommandEvent& event);
    // 画像をブラウザで開く
    void OnOpenImageByBrowser(wxCommandEvent& event);
    // 参照元スレッドを開く
    void OpenThreadGotImage(wxCommandEvent& event);
    // 画像ビューアを隠す
    void HideThumbnailTab(wxCommandEvent& event);
    // 名前をつけて保存
    void SaveAsImages(wxCommandEvent& event);
    // すべて保存
    void SaveAsImagesAll(wxCommandEvent& event);
    // URLをコピー
    void CopyImageURL(wxCommandEvent& event);
    // 左回転
    void Rotate90AntiClockwise(wxCommandEvent& event);
    // 右回転
    void Rotate90Clockwise(wxCommandEvent& event);
    // 画像を拡大する
    void ZoomIn(wxCommandEvent& event);
    void ZoomIn();
    // 画像を縮小する
    void ZoomOut(wxCommandEvent& event);
    void ZoomOut();
    // 画像を元のサイズに戻す
    void ResetImageOriginalSize(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};


#endif /* JANECLONEIMAGEVIEWER_HPP_ */

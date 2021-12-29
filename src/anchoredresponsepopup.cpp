
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
 *  Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#include "anchoredresponsepopup.hpp"


IMPLEMENT_CLASS(AnchoredResponsePopup,wxPopupTransientWindow)

BEGIN_EVENT_TABLE(AnchoredResponsePopup,wxPopupTransientWindow)
    EVT_ENTER_WINDOW( AnchoredResponsePopup::EnterWindow )
    EVT_LEAVE_WINDOW( AnchoredResponsePopup::LeaveWindow )
    EVT_SIZE( AnchoredResponsePopup::OnSize )
    EVT_SET_FOCUS( AnchoredResponsePopup::OnSetFocus )
    EVT_KILL_FOCUS( AnchoredResponsePopup::OnKillFocus )
    EVT_HTML_LINK_CLICKED(wxID_ANY, AnchoredResponsePopup::OnLinkClocked)
END_EVENT_TABLE()

/**
 * ポップアップ用ウィンドウのコンストラクタ
 *
 * @param wxWindow* parent     親ウィンドウのインスタンス
 * @param wxPoint&  point      ウィンドウの出現位置
 * @param wxSize    size       ウィンドウのサイズ
 * @raram wxString& htmlSource 表示させるHTMLソース
 */
AnchoredResponsePopup::AnchoredResponsePopup( wxWindow *parent, wxPoint& point, wxSize size, wxString& htmlSource )
:wxPopupTransientWindow( parent ) {

    // スキン用のパスが設定されていれば読み出す
    wxString skinPath = wxEmptyString;
    JaneCloneUtil::GetJaneCloneProperties(wxT("DEFAULT_SKINFILE_PATH"), &skinPath);
    bool useSkin = false;
    std::unique_ptr<SkinInfo> skinInfo = nullptr;

    if (skinPath != wxEmptyString) {
        useSkin = true;
        std::unique_ptr<SkinInfo> skinInfo = GetSkinInfo(skinPath);
    }

    // HTMLのソース
    wxString html = wxEmptyString;
    wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);
    // wxHtmlWindowにHTMLソースを設定する
    htmlWin = new wxHtmlWindow(this, wxID_ANY, wxPoint(0, 0), size, wxHW_SCROLLBAR_AUTO);

    if (!useSkin) {
        const int p = 12;
        static int f_size[] = {p - 2, p - 1, p, p + 1, p + 2, p + 3, p + 4 };

#ifdef __WXMSW__
        wxString fontName = wxT("ＭＳ Ｐゴシック");
        htmlWin->SetFonts(fontName, wxEmptyString, f_size);
#elif  __WXGTK__
        wxFont f;
        if (f.SetFaceName(wxT("Mona"))) {
            htmlWin->SetFonts(wxT("Mona"), wxEmptyString, f_size);
        } else if (f.SetFaceName(wxT("Mona-VLGothic"))) {
            htmlWin->SetFonts(wxT("Mona-VLGothic"), wxEmptyString, f_size);
        } else if (f.SetFaceName(wxT("Mona-sazanami"))) {
            htmlWin->SetFonts(wxT("Mona-sazanami"), wxEmptyString, f_size);
        }
#elif  __WXMAC__
        wxString fontName = wxT("Mona");
        htmlWin->SetFonts(fontName, wxEmptyString, f_size);
#endif
        html += HTML_HEADER;
        html += htmlSource;
        html += HTML_FOOTER;
    } else {
        // スキンを使用する
        html += skinInfo->header;
        html += htmlSource;
        html += skinInfo->footer;
    }

    htmlWin->SetBorders(0);
    htmlWin->SetPage(html);
    // wxHtmlWindow内部での実質サイズを取得する
    int hx = htmlWin->GetInternalRepresentation()->GetWidth();
    int hy = htmlWin->GetInternalRepresentation()->GetHeight();
    htmlWin->SetSize(hx, hy);
    // このウィンドウ(popup)にサイザーをセットして最小の大きさに設定する
    topsizer->Add(htmlWin, 1, wxALL, 10);
    SetSizerAndFit(topsizer);
    Layout();
}

wxSize AnchoredResponsePopup::GetPopupWindowSize() {
    return wxSize(htmlWin->GetInternalRepresentation()->GetWidth(), htmlWin->GetInternalRepresentation()->GetHeight());
}

void AnchoredResponsePopup::Popup(wxWindow* WXUNUSED(focus)) {
    wxPopupTransientWindow::Popup();
}

void AnchoredResponsePopup::OnDismiss() {
    wxPopupTransientWindow::OnDismiss();
}

bool AnchoredResponsePopup::ProcessLeftDown(wxMouseEvent& event) {
    return wxPopupTransientWindow::ProcessLeftDown(event);
}
bool AnchoredResponsePopup::Show(bool show) {
    return wxPopupTransientWindow::Show(show);
}

void AnchoredResponsePopup::OnSize(wxSizeEvent &event) {
    event.Skip();
}

void AnchoredResponsePopup::OnSetFocus(wxFocusEvent &event) {
    event.Skip();
}

void AnchoredResponsePopup::OnKillFocus(wxFocusEvent &event) {
    event.Skip();
}

void AnchoredResponsePopup::EnterWindow(wxMouseEvent &event) {
}

void AnchoredResponsePopup::LeaveWindow(wxMouseEvent &event) {
}

std::unique_ptr<SkinInfo>
AnchoredResponsePopup::GetSkinInfo(const wxString& skinPath) {

    std::unique_ptr<SkinInfo> skin = std::make_unique<SkinInfo>();
    if (!wxDir::Exists(skinPath)) {
        wxMessageBox(wxT("スキン用のディレクトリが存在しません、設定画面を開いてスキンのパス設定を確認してください。"),
                     wxT("スキン設定"), wxICON_ERROR);
        return nullptr;
    }

    // Footer.html
    if (wxFile::Exists(skinPath + wxFILE_SEP_PATH + wxT("Footer.html"))) {
        const wxString filePath = skinPath + wxFILE_SEP_PATH + wxT("Footer.html");
        skin->footer = ReadPlainTextFile(filePath);
    }
    // Header.html
    if (wxFile::Exists(skinPath + wxFILE_SEP_PATH + wxT("Header.html"))) {
        const wxString filePath = skinPath + wxFILE_SEP_PATH + wxT("Header.html");
        skin->header = ReadPlainTextFile(filePath);
    }
    // NewRes.html
    if (wxFile::Exists(skinPath + wxFILE_SEP_PATH + wxT("NewRes.html"))) {
        const wxString filePath = skinPath + wxFILE_SEP_PATH + wxT("NewRes.html");
        skin->newres = ReadPlainTextFile(filePath);
    }
    // PopupRes.html
    if (wxFile::Exists(skinPath + wxFILE_SEP_PATH + wxT("PopupRes.html"))) {
        const wxString filePath = skinPath + wxFILE_SEP_PATH + wxT("PopupRes.html");
        skin->popup = ReadPlainTextFile(filePath);
    }
    // Res.html
    if (wxFile::Exists(skinPath + wxFILE_SEP_PATH + wxT("Res.html"))) {
        const wxString filePath = skinPath + wxFILE_SEP_PATH + wxT("Res.html");
        skin->res = ReadPlainTextFile(filePath);
    }

    return std::move(skin);
}

/**
 * 指定されたファイル中のテキストをメモリに展開する
 */
wxString AnchoredResponsePopup::ReadPlainTextFile(const wxString& filePath) {

    wxTextFile textFile;
    wxString   htmlDOM;
    textFile.Open(filePath, wxConvUTF8);

    for (unsigned int i = 0; i < textFile.GetLineCount(); i++) {
        htmlDOM += textFile[i];
    }

    return htmlDOM;
}

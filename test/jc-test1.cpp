#include <wx/wx.h>
#include <wx/wxnkf.h>

class JaneCloneTest1 : public wxAppConsole {

public:
     virtual bool OnInit();
     virtual int  OnRun();
};

IMPLEMENT_APP(JaneCloneTest1)

bool JaneCloneTest1::OnInit() {
     return true;
}

int JaneCloneTest1::OnRun() {

     wxString test1 = wxT("あいうえお");

     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     wxNKF* nkf = new wxNKF();
     const std::string std1 = nkf->WxToMultiByte(test1, option);
     std::cout << std1 << std::endl;

     return 0;
}

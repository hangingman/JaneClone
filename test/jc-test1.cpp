#include <wx/wx.h>
#include <wx/wxnkf.h>

class JaneCloneTest1 : public wxAppConsole {

public:
     virtual bool OnInit();
     virtual int  OnRun();

private:
     int Test1();
     int Test2();
};

IMPLEMENT_APP(JaneCloneTest1)

bool JaneCloneTest1::OnInit() {
     return true;
}

int JaneCloneTest1::OnRun() {

     Test1();
     Test2();

     return 0;
}

int JaneCloneTest1::Test1() {

     wxString test1 = wxT("あいうえお");

     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     wxNKF* nkf = new wxNKF();
     const std::string std = nkf->WxToMultiByte(test1, option);
     delete nkf;

     std::cout.setf(std::ios::hex, std::ios::basefield);
     std::cout.setf(std::ios::showbase);

     for (std::string::const_iterator it = std.begin(); it != std.end(); ++it) {
	  std::cout << (int)(*it & 0xff) << " ";
     }

     std::cout << std::endl;
}

int JaneCloneTest1::Test2() {

     wxString test2 = wxT(":test:あいうえお");

     const wxString option = wxT("--ic=UTF-8 --oc=CP932");
     wxNKF* nkf = new wxNKF();
     const std::string std = nkf->WxToMultiByte(test2, option);
     delete nkf;

     std::cout.setf(std::ios::hex, std::ios::basefield);
     std::cout.setf(std::ios::showbase);

     for (std::string::const_iterator it = std.begin(); it != std.end(); ++it) {
	  std::cout << (int)(*it & 0xff) << " ";
     }

     std::cout << std::endl;
}


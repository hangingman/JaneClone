#include <wx/wx.h>
#include "wxUUID.h"

int main()
{
     wxLongLong pLLTime;

     wxUUID* uuid = new wxUUID();
     uuid->GetTimeStamp(&pLLTime);
     
     return 0;
}

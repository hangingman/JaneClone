#ifndef BOARDLIST_H
#define BOARDLIST_H

#include <wx/xml/xml.h>

class BoardList
{
	public:
		BoardList();
		wxArrayString GetBoardList();

	private:
		wxArrayString *m_AllBoardList;
};

#endif // BOARDLIST_H

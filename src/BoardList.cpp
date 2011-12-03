#include "BoardList.h"

BoardList::BoardList()
{
	// ArrayString文字列を用意する
	m_AllBoardList = new wxArrayString();

	// XMLパーサーで読み取るドキュメントを定める
	wxXmlDocument doc;
	if (!doc.Load(wxT("./dat/BoardListUTF8.txt")))
		return false;

	// start processing the XML file
	if (doc.GetRoot()->GetName() != wxT("myroot-node"))
		return false;

	wxXmlNode *child = doc.GetRoot()->GetChildren();
	while (child) {

		if (child->GetName() == wxT("tag1")) {

			// process text enclosed by <tag1></tag1>
			wxString content = child->GetNodeContent();

			...


			// process properties of <tag1>
			wxString propvalue1 =
				child->GetPropVal(wxT("prop1"),
								  wxT("default-value"));
			wxString propvalue2 =
				child->GetPropVal(wxT("prop2"),
								  wxT("default-value"));

			...

		} else if (child->GetName() == wxT("tag2")) {

			// process tag2 ...
		}

		child = child->GetNext();
	}
}

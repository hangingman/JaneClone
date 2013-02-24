/**
 * janecloneimageviewer.hpp -  janeclone's plain imageviewer
 *
 * Copyright (c) 2013 Hiroyuki Nagata <newserver002@gmail.com>
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
#ifndef JANECLONEIMAGEVIEWER_H_
#define JANECLONEIMAGEVIEWER_H_

#include <wx/popupwin.h>

class JaneCloneImageViewer : public wxPopupWindow {

public:
     /// Default constructor
     JaneCloneImageViewer();
     /// Destructor
     ~JaneCloneImageViewer();
     /// constructor
     JaneCloneImageViewer(wxWindow *parent, int flags=wxBORDER_SIMPLE );
     
     /// Copy constructor
     JaneCloneImageViewer(const JaneCloneImageViewer& rhs);
     /// Assignment operator
     JaneCloneImageViewer& operator=(const JaneCloneImageViewer& rhs);
};


#endif /* JANECLONEIMAGEVIEWER_H_ */

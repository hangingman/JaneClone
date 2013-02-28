//////////////////////////////////////////////////////////////////////
//	Name:				wxUUID.h
//	Purpose:			wxUUID Class
//	Author:				Casey O'Donnell
//	Creator:			Derived from UUID and GUID Specification in,
//						draft-leach-uuid-guids-01.txt, last updated
//						02/04/1998
//	Created:			03/30/2004
//	Last modified:		03/30/2004
//	Licence:			wxWindows license
//////////////////////////////////////////////////////////////////////

// wxUUID.h: interface for the wxUUID class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WXUUID_H__
#define _WXUUID_H__

#ifdef __GNUG__
	#pragma interface "wxUUID.h"
#endif

#include "wx/timer.h"
#include "wx/longlong.h"
#include "wx/string.h"

#define WXUUID_V1		0
#define WXUUID_VNAME	1
#define WXUUID_V3		2

class WXDLLEXPORT wxUUID
{
public:
	wxUUID(const int& iVersion = 0, const wxString& szNameOrHash = wxEmptyString);

	// Data Access Methods
	long		GetTimeLow() const				{ return m_lTimeLow; }
	short		GetTimeMid() const				{ return m_sTimeMid; }
	short		GetTimeHiAndVersion() const		{ return m_sTimeHiAndVersion; }
	char		GetClockHiAndReserved() const	{ return m_cClockHiAndReserved; }
	char		GetClockLow() const				{ return m_cClockLow; }
	const char*	GetIEEENode() const				{ return (const char*)m_IEEENode; }

	// Operator Overloading

	// Other Methods
	wxString ToString() const;

	// Static Methods
	static wxString GetUUID();
	static wxUUID ParseUUID(const wxString& szUUID);

protected:

	// UUID Generation Utility Methods
	void	GetTimeStamp(wxLongLong* pLLTime);
	void	GetIEEENode(unsigned char pIEEENode[6]);
	void	GetRandomInfo(char pSeed[16]);
	short	GetRandomNumber();

	void	DoV1();
	void	DoVName();
	void	DoV3();

	void	GenerateUUIDv1(const short& sClock, const wxLongLong& llTime, const unsigned char pIEEENode[6]);
	void	GenerateUUIDv3(const wxString& szHash);
	void	GenerateUUIDvName(const wxString& szName);

	wxString			m_szNameOrHash;

	unsigned long		m_lTimeLow;
	unsigned short		m_sTimeMid;
	unsigned short		m_sTimeHiAndVersion;
	unsigned char		m_cClockHiAndReserved;
	unsigned char		m_cClockLow;
	unsigned char		m_IEEENode[6];

private:

};

#endif // _WXUUID_H__

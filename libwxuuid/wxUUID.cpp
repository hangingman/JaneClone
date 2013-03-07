//////////////////////////////////////////////////////////////////////
//	Name:				wxUUID.cpp
//	Purpose:			wxUUID Class
//	Author:				Casey O'Donnell
//	Creator:			Derived from UUID and GUID Specification in,
//						draft-leach-uuid-guids-01.txt, last updated
//						02/04/1998
//	Created:			03/30/2004
//	Last modified:		03/30/2004
//	Licence:			wxWindows license
//////////////////////////////////////////////////////////////////////

// wxUUID.cpp: implementation of the wxUUID class.
//
//////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "wxMD5.h"
#endif

// for compilers that support precompilation, includes "wx.h"
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/utils.h>

#include "wxMD5.h"
#include "wxUUID.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxUUID::wxUUID(const int& iVersion /*= 0*/, const wxString& szNameOrHash /*= wxEmptyString*/)
{
     m_szNameOrHash = szNameOrHash;

     if(iVersion==0) {
	  DoV1();
     } else if(iVersion==1) {
	  DoVName();
     } else if(iVersion==2) {
	  DoV3();
     }
}

//////////////////////////////////////////////////////////////////////
// Operator Overloading
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Other Methods
//////////////////////////////////////////////////////////////////////

wxString wxUUID::ToString() const
{
     wxString szRetVal = 
	  wxString::Format(_("{%8.8x-%4.4x-%4.4x-%2.2x%2.2x-"), 
			   m_lTimeLow, m_sTimeMid, m_sTimeHiAndVersion, m_cClockHiAndReserved, m_cClockLow);

     for(unsigned int i = 0; i < 6; i++)
     {
	  szRetVal += wxString::Format(_("%2.2x"), m_IEEENode[i]);
     }

     szRetVal += _T("}");

     return szRetVal.MakeUpper();
}

//////////////////////////////////////////////////////////////////////
// Static Methods
//////////////////////////////////////////////////////////////////////

wxString wxUUID::GetUUID()
{
     wxUUID uuid;

     return uuid.ToString();
}

wxUUID wxUUID::ParseUUID(const wxString& szUUID)
{
     wxUUID uuid;

     return uuid;
}

//////////////////////////////////////////////////////////////////////
// UUID Generation Utility Methods
//////////////////////////////////////////////////////////////////////

void wxUUID::GetTimeStamp(wxLongLong* pLLTime)
{
     wxLongLong llBaseTime = wxGetLocalTimeMillis();

     // Offset between UUID formatted times and Unix formatted times.
     // UUID UTC base time is October 15, 1582.
     // Unix UTC base time is January 1, 1970.

     *pLLTime = llBaseTime.GetLo() * 10000000 + llBaseTime.GetHi() * 10 + wxLongLong(0x01B21DD213814000LL);
}

void wxUUID::GetIEEENode(unsigned char pIEEENode[6])
{
     char seed[16] = "\0";

     GetRandomInfo(seed);

     seed[0] |= 0x80;

     memcpy(pIEEENode, seed, sizeof(pIEEENode));
}

void wxUUID::GetRandomInfo(char pSeed[16])
{
     wxString	szSystemInfo = wxGetOsDescription();
     wxString	szHostName = wxGetFullHostName();
     wxLongLong	llTime;

     GetTimeStamp(&llTime);

     wxString	szToHash = szSystemInfo + llTime.ToString() + szHostName;
     wxString	szHash = wxMD5::GetDigestStatic(szToHash);
     const char* szHashChar = szHash.mb_str();
     
     for(unsigned int i = 0; i < 16; i++)
     {
	  pSeed[i] = *szHashChar;
	  ++szHashChar;
     }
}

short wxUUID::GetRandomNumber()
{
     long iSeed = wxGetUTCTime();

     srand(iSeed);

     return rand();
}

void wxUUID::DoV1()
{
     unsigned char	ieeeNode[6] = "\0";
     wxLongLong		llTime;
     short              sClock = GetRandomNumber();
     GetTimeStamp(&llTime);
     GetIEEENode(ieeeNode); // bug
     GenerateUUIDv1(sClock, llTime, ieeeNode);
}

void wxUUID::DoVName()
{
}

void wxUUID::DoV3()
{
}

void wxUUID::GenerateUUIDv1(const short& sClock, const wxLongLong& llTime, const unsigned char pIEEENode[6])
{
     m_lTimeLow = llTime.GetLo();
     m_sTimeMid = (unsigned short)((llTime >> 32) & 0xFFFF).GetLo();
     m_sTimeHiAndVersion = (unsigned short)((llTime >> 48) & 0x0FFF).GetLo();
     m_sTimeHiAndVersion |= (1 << 12);
     m_cClockLow = sClock & 0xFF;
     m_cClockHiAndReserved = (sClock & 0x3F00) >> 8;
     m_cClockHiAndReserved |= 0x80;
     memcpy(m_IEEENode, pIEEENode, sizeof(pIEEENode));
}

void wxUUID::GenerateUUIDv3(const wxString& szHash)
{
}

void wxUUID::GenerateUUIDvName(const wxString& szName)
{
}




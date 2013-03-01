//---------------------------------------------------------------------------
//
// Name:        wxGUID.cpp
// Author:      Jorgen Bodde
// Created:     07/11/03
// Copyright:
//
//---------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "wxGUID.cpp"
#endif

/* for compilers that support precompilation
   includes "wx/wx.h" */

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifdef LINUX
#include <uuid/uuid.h>
#endif

#include <wx/debug.h>
#include "wxGUID.h"


//----------------------------------------------------------------------------
// wxGUID
//----------------------------------------------------------------------------

wxGUID::wxGUID(bool create)
{
	if(create)
		Generate();
	else
		Clear();
}

wxGUID::wxGUID(const wxGUID &value)
{
	Copy(value);
}

wxGUID::~wxGUID()
{
	// TODO: Enter destructor code
}

bool wxGUID::IsEqual(const wxGUID &value) const
{
	// returns true if both guid's are equal

	for(int i = 0; i < WX_GUID_LEN; i++)
	{
		if(guid[i] != value.guid[i])
			return false;
	}

	return true;
}

bool wxGUID::Generate()
{
	bool value = false;
	
#ifdef _WINDOWS_
	unsigned char *ptr;
	HRESULT res;
	GUID uuid;

	res = CoCreateGuid(&uuid);
	if(res == S_OK)
	{
		// copy the values
		ptr = (unsigned char *)&uuid.Data1;
		for(int i = 0; i < 4; i++)
			guid[i] = ptr[i];

		guid[4] = uuid.Data2 & 0xff;
		guid[5] = (uuid.Data2 >> 8) & 0xff;

		guid[6] = uuid.Data3 & 0xff;
		guid[7] = (uuid.Data3 >> 8) & 0xff;

		for(int j = 0; j < 8; j++)
			guid[8+j] = uuid.Data4[j];

		value = true;
	}

#endif
#ifdef LINUX
	uuid_t uid;
	uuid_generate(uid);

	wxASSERT_MSG(WX_GUID_LEN == sizeof(uuid_t), 
	             wxT("wxGUID::Generate() - Size of WX_GUID_LEN is not equal to uuid_t struct"));
	
	for(int i = 0; i < 16; i++)
		guid[i] = uid[i];

	// expect it always to be true
	value = true;
#endif

	return value;
}

void wxGUID::Clear()
{
	// clear GUID (initialise)

	for(int i = 0; i < WX_GUID_LEN; i++)
		guid[i] = 0;
}

wxString wxGUID::ToString() const
{
	wxString uid;
	uid.Printf(_("{%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x}"),
			   guid[0], guid[1], guid[2],  guid[3],  guid[4],  guid[5],  guid[6],  guid[7],
			   guid[8], guid[9], guid[10], guid[11], guid[12], guid[13], guid[14], guid[15]);

	return uid.MakeUpper();
}

void wxGUID::Copy(const wxGUID &value)
{
	for(int i = 0; i < WX_GUID_LEN; i++)
		guid[i] = value.guid[i];
}

bool wxGUID::StrToGuid(const char *value)
{
	unsigned char number;
	wxString str(value, wxConvUTF8);

	Clear();

	str = str.Strip(wxString::both);

	if(str.Len() < 38)
		return false;

	if(str.GetChar(0) != '{' && str.Last() != '}')
		return false;

	str = str.Mid(1, str.Len() - 2);

	for(int i = 0; i < 16; i++)
	{
		if(GetStringByte(str, number))
			guid[i] = number;
		else
		{
			Clear();
			return false;
		}
	}

	return true;
}

bool wxGUID::GetStringByte(wxString &str, unsigned char &result)
{
	unsigned char high, low;

	if(str.Len() < 2)
		return false;		// no room

	for(int i = 0; i < (int)str.Len()-1; i++)
	{
		if(str.GetChar(i) != '-' && GetNibble(str.GetChar(i), high) && GetNibble(str.GetChar(i+1), low))
		{
			// two succeeding nibbles
			result = ((high << 4) & 0xf0) + low;
			str = str.Mid(i+2);		// chop off
			return true;
		}
	}

	return false;
}

bool wxGUID::GetNibble(unsigned char nibble, unsigned char &value)
{
	bool result = false;

	if(nibble >= 'a' && nibble <= 'f')
	{
		value = nibble - 87;
		result = true;
	}
	else if(nibble >= 'A' && nibble <= 'F')
	{
		value = nibble - 45;
		result = true;
	}
	else if(nibble >= '0' && nibble <= '9')
	{
		value = nibble - 48;
		result = true;
	}

	return result;
}

void wxGUID::SetData(const unsigned char *data)
{
	if(data)
	{
		// copy the data in the buffer
		for(int i = 0; i < WX_GUID_LEN; i++)
			guid[i] = data[i];
	}
	else
		Clear();
}

bool wxGUID::IsBlank() const
{
	// returns wether every GUID number is 0
	// this is not possible (even statistically seen)

	for(int i = 0; i < WX_GUID_LEN; i++)
	{
		if(guid[i] != 0)
			return false;
	}

	return true;
}

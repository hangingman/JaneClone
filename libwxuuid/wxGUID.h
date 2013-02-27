#ifndef __WXGUID_HPP_
#define __WXGUID_HPP_

#ifdef __GNUG__
    #pragma interface "wxGUID.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#define WX_GUID_LEN 16

//----------------------------------------------------------------------------
// wxGUID
//----------------------------------------------------------------------------

class wxGUID
{
public:
    wxGUID(bool create = true);
	wxGUID(const wxGUID &value);
    virtual ~wxGUID();

    bool operator==(const wxGUID &value) const { return IsEqual(value); };
    bool operator!=(const wxGUID &value) const { return !IsEqual(value); };
    void operator=(const wxGUID &value) { Copy(value); };

	bool IsEqual(const wxGUID &value) const;
	void Copy(const wxGUID &value);
	bool Generate();
	void Clear();
	wxString ToString() const;
	bool StrToGuid(const char *value);
	const unsigned char *GetData() const { return guid; };
	void SetData(const unsigned char *data);

	bool IsBlank() const;

private:
	bool GetNibble(unsigned char nibble, unsigned char &value);
	bool GetStringByte(wxString &str, unsigned char &result);
	unsigned char guid[WX_GUID_LEN];
};


#endif

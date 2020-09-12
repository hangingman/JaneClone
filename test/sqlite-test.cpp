#include <gtest/gtest.h>
#include "sqliteaccessor.hpp"
#include <iostream>

TEST(GetDBFilePath, wxString) {
    // default setting
    wxString actual = SQLiteAccessor::GetDBFilePath();
    wxFileName dbFile = wxFileName::FileName(actual, wxPATH_UNIX);

    ASSERT_STREQ("janeclone.db", dbFile.GetFullName());
}

TEST(GetDBFilePathEnv, wxString) {
    // test setting
    wxString testDir = wxEmptyString;
    ASSERT_TRUE(wxGetEnv("WK_DIR", &testDir));
    wxFileName testDbFile = wxFileName::DirName(testDir);
    testDbFile.SetFullName("janeclone.db");
    ASSERT_TRUE(wxSetEnv("JANECLONE_DB_FILE_PATH", testDbFile.GetFullPath()));

    wxString actual = SQLiteAccessor::GetDBFilePath();
    wxFileName dbFile = wxFileName::FileName(actual, wxPATH_UNIX);

    wxString expected = wxString::Format("%s/%s", testDir, "janeclone.db");
    ASSERT_STREQ(expected.mb_str(), dbFile.GetFullPath());
}

#include <iostream>
#include <gtest/gtest.h>
#include "sqliteaccessor.hpp"

// テストのGenerator
class DbGeneratorFixture : public testing::Test {

public:
    wxFileName testDbFile;

protected:
    void SetUp() override {
        // test setting
        wxString testDir = wxEmptyString;
        ASSERT_TRUE(wxGetEnv("WK_DIR", &testDir));
        testDbFile = wxFileName::DirName(testDir);
        testDbFile.SetFullName("janeclone.db");
        ASSERT_TRUE(wxSetEnv("JANECLONE_DB_FILE_PATH", testDbFile.GetFullPath()));

        // 事前にファイルがあれば削除しておく
        const wxString dbFile = this->testDbFile.GetFullPath();
        if (wxFileExists(dbFile)) {
            ASSERT_TRUE(wxRemoveFile(dbFile));
        }
    }

    void TearDown() override {
    }
};

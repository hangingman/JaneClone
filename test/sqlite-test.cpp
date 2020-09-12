#include "sqlite-test.hpp"

TEST(GetDBFilePath, Simple) {
    // default setting
    wxString actual = SQLiteAccessor::GetDBFilePath();
    wxFileName dbFile = wxFileName::FileName(actual, wxPATH_UNIX);

    ASSERT_STREQ("janeclone.db", dbFile.GetFullName());
}

TEST(GetDBFilePathEnv, EnvironmentVariable) {
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

TEST_F(DbGeneratorFixture, SQLiteAccessor) {

    DB db;
    SQLiteAccessor::AutoCloseable([&](DB& db)
    {
        db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS test (test TEXT);"));
    })(db);

    ASSERT_TRUE(wxFileExists(this->testDbFile.GetFullPath()));
}

TEST_F(DbGeneratorFixture, SQLiteAccessorCtor) {

    auto ac = SQLiteAccessor();
    ASSERT_TRUE(wxFileExists(this->testDbFile.GetFullPath()));
    DB db;

    const wxString tableNames[] = {
        wxT("board_info"),
        wxT("user_looking_boardlist"),
        wxT("user_looking_threadlist"),
        wxT("user_closed_boardlist"),
        wxT("user_closed_threadlist"),
        wxT("user_favorite_threadlist"),
        wxT("user_searched_boardname"),
        wxT("user_searched_threadname"),
        wxT("user_searched_threadcontents"),
        wxT("cached_image")
    };

    try {
        db.Open(this->testDbFile.GetFullPath());
        for (wxString table : tableNames) {
            ASSERT_TRUE(db.TableExists(table));
        }
        db.Close();

    } catch (wxSQLite3Exception& e) {
        std::cerr << e.GetMessage() << std::endl;
    }
}

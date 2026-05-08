#include "SQLiteDB.h"
#include <iostream>

#ifdef HAVE_SQLITE3
#include "../third_party/sqlite3.h"

bool insertToSQLite(const Schema& schema, const std::vector<Row>& rows, const std::string& dbPath) {
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "[ERROR] SQLite open failed: " << dbPath << '\n';
        return false;
    }

    // CREATE TABLE IF NOT EXISTS
    std::string createSql = "CREATE TABLE IF NOT EXISTS " + schema.table + " (";
    for (size_t i = 0; i < schema.columns.size(); ++i) {
        if (i) createSql += ", ";
        const auto& col = schema.columns[i];
        createSql += col.name + " ";
        createSql += (col.type == ColType::INT || col.type == ColType::BOOL) ? "INTEGER" : "TEXT";
    }
    createSql += ");";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createSql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "[ERROR] CREATE TABLE: " << errMsg << '\n';
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    // Prepared INSERT
    std::string colList, paramList;
    for (size_t i = 0; i < schema.columns.size(); ++i) {
        if (i) { colList += ", "; paramList += ", "; }
        colList   += schema.columns[i].name;
        paramList += "?";
    }
    std::string insertSql = "INSERT INTO " + schema.table +
                            " (" + colList + ") VALUES (" + paramList + ");";

    sqlite3_stmt* stmt = nullptr;
    sqlite3_prepare_v2(db, insertSql.c_str(), -1, &stmt, nullptr);

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            int idx = static_cast<int>(i + 1);
            const auto& col = schema.columns[i];
            if (col.type == ColType::INT || col.type == ColType::BOOL)
                sqlite3_bind_int(stmt, idx, std::stoi(row[i]));
            else
                sqlite3_bind_text(stmt, idx, row[i].c_str(), -1, SQLITE_TRANSIENT);
        }
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "[OK] DB   -> " << dbPath
              << "  (" << rows.size() << " rows inserted into [" << schema.table << "])\n";
    return true;
}

#else

bool insertToSQLite(const Schema& schema, const std::vector<Row>& rows, const std::string& dbPath) {
    std::cerr << "[WARN] SQLite support not enabled.\n"
              << "       1) Download sqlite3.h + sqlite3.c from https://sqlite.org/download.html\n"
              << "       2) Place them in third_party/\n"
              << "       3) Add HAVE_SQLITE3 to PreprocessorDefinitions in vcxproj\n"
              << "       4) Add third_party\\sqlite3.c to the project's ClCompile items\n";
    return false;
}

#endif

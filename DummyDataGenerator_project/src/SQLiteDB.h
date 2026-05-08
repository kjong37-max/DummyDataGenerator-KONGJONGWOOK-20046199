#pragma once
#include "Schema.h"
#include <vector>
#include <string>

// SQLite3 DB에 데이터 삽입.
// 빌드 시 HAVE_SQLITE3 를 정의하고 third_party/sqlite3.h + sqlite3.c 를 추가하면 활성화됨.
bool insertToSQLite(const Schema& schema, const std::vector<Row>& rows, const std::string& dbPath);

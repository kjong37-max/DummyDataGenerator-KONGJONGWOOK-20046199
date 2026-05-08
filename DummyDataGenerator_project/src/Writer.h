#pragma once
#include "Schema.h"
#include <vector>
#include <string>

void printConsole(const Schema& schema, const std::vector<Row>& rows);
void writeCsv(const Schema& schema, const std::vector<Row>& rows, const std::string& path);
void writeSql(const Schema& schema, const std::vector<Row>& rows, const std::string& path);

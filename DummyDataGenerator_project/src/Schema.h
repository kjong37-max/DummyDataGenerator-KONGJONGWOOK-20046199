#pragma once
#include <string>
#include <vector>

enum class ColType { INT, TEXT, NAME, EMAIL, DATE, BOOL, PHONE };

struct ColDef {
    std::string name;
    ColType     type     = ColType::TEXT;
    int         minVal   = 0;
    int         maxVal   = 100;
    int         textLen  = 10;
    std::string dateFrom = "2020-01-01";
    std::string dateTo   = "2024-12-31";
};

struct Schema {
    std::string         table;
    std::vector<ColDef> columns;
};

using Row = std::vector<std::string>;

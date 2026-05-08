#pragma once
#include "Schema.h"
#include <string>

struct SchemaLoader {
    static Schema load(const std::string& path);
    static Schema defaultSchema();
};

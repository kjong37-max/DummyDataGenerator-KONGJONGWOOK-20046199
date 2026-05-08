#include "SchemaLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

static std::string trim(const std::string& s) {
    auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return {};
    return s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
}

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim))
        result.push_back(trim(token));
    return result;
}

static ColType parseType(const std::string& t) {
    if (t == "INT")   return ColType::INT;
    if (t == "NAME")  return ColType::NAME;
    if (t == "EMAIL") return ColType::EMAIL;
    if (t == "DATE")  return ColType::DATE;
    if (t == "BOOL")  return ColType::BOOL;
    if (t == "PHONE") return ColType::PHONE;
    return ColType::TEXT;
}

Schema SchemaLoader::load(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "[WARN] Schema file not found: " << path << " - using default schema\n";
        return defaultSchema();
    }

    Schema schema;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        auto eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = trim(line.substr(0, eqPos));
        std::string val = trim(line.substr(eqPos + 1));

        if (key == "table") {
            schema.table = val;
        } else if (key == "col") {
            auto parts = split(val, ',');
            if (parts.size() < 2) continue;

            ColDef col;
            col.name = parts[0];
            col.type = parseType(parts[1]);

            std::map<std::string, std::string> opts;
            for (size_t i = 2; i < parts.size(); ++i) {
                auto pos = parts[i].find('=');
                if (pos != std::string::npos)
                    opts[trim(parts[i].substr(0, pos))] = trim(parts[i].substr(pos + 1));
            }
            if (opts.count("min"))  col.minVal   = std::stoi(opts["min"]);
            if (opts.count("max"))  col.maxVal   = std::stoi(opts["max"]);
            if (opts.count("len"))  col.textLen  = std::stoi(opts["len"]);
            if (opts.count("from")) col.dateFrom = opts["from"];
            if (opts.count("to"))   col.dateTo   = opts["to"];

            schema.columns.push_back(col);
        }
    }
    return schema;
}

Schema SchemaLoader::defaultSchema() {
    Schema s;
    s.table = "dummy_users";
    s.columns.push_back({"id",          ColType::INT,   1, 99999});
    s.columns.push_back({"full_name",   ColType::NAME});
    s.columns.push_back({"email",       ColType::EMAIL});
    s.columns.push_back({"age",         ColType::INT,   18, 65});
    s.columns.push_back({"phone",       ColType::PHONE});
    s.columns.push_back({"signup_date", ColType::DATE,  0, 0, 10, "2020-01-01", "2024-12-31"});
    s.columns.push_back({"is_active",   ColType::BOOL});
    s.columns.push_back({"memo",        ColType::TEXT,  0, 0, 20});
    return s;
}

#pragma once
#include "Schema.h"
#include <random>
#include <vector>

class DataGenerator {
public:
    explicit DataGenerator(const Schema& schema);
    std::vector<Row> generate(int count);

private:
    const Schema& schema_;
    std::mt19937  rng_;

    std::string genValue(const ColDef& col);
    std::string genInt(const ColDef& col);
    std::string genText(const ColDef& col);
    std::string genName();
    std::string genEmail();
    std::string genDate(const ColDef& col);
    std::string genBool();
    std::string genPhone();
};

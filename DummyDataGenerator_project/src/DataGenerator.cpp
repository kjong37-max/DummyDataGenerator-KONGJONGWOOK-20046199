#include "DataGenerator.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iterator>

static const char* kFirstNames[] = {
    "James","John","Robert","Michael","William","David","Richard","Joseph",
    "Mary","Patricia","Jennifer","Linda","Barbara","Elizabeth","Susan","Jessica",
    "MinJun","JiHo","SooYeon","HanNa","DongHyun","YeJin","JuHyeon","SoYeon"
};
static const char* kLastNames[] = {
    "Smith","Johnson","Williams","Brown","Jones","Garcia","Miller","Davis",
    "Kim","Lee","Park","Choi","Jung","Kang","Cho","Yoon"
};
static const char* kDomains[] = {
    "gmail.com","naver.com","daum.net","outlook.com","yahoo.com","kakao.com"
};

DataGenerator::DataGenerator(const Schema& schema)
    : schema_(schema), rng_(std::random_device{}()) {}

std::vector<Row> DataGenerator::generate(int count) {
    std::vector<Row> rows;
    rows.reserve(count);
    for (int i = 0; i < count; ++i) {
        Row row;
        for (const auto& col : schema_.columns)
            row.push_back(genValue(col));
        rows.push_back(std::move(row));
    }
    return rows;
}

std::string DataGenerator::genValue(const ColDef& col) {
    switch (col.type) {
        case ColType::INT:   return genInt(col);
        case ColType::TEXT:  return genText(col);
        case ColType::NAME:  return genName();
        case ColType::EMAIL: return genEmail();
        case ColType::DATE:  return genDate(col);
        case ColType::BOOL:  return genBool();
        case ColType::PHONE: return genPhone();
    }
    return {};
}

std::string DataGenerator::genInt(const ColDef& col) {
    std::uniform_int_distribution<int> dist(col.minVal, col.maxVal);
    return std::to_string(dist(rng_));
}

std::string DataGenerator::genText(const ColDef& col) {
    static const char kAlpha[] = "abcdefghijklmnopqrstuvwxyz";
    int len = col.textLen > 0 ? col.textLen : 10;
    std::uniform_int_distribution<int> dist(0, 25);
    std::string s(len, ' ');
    std::generate(s.begin(), s.end(), [&] { return kAlpha[dist(rng_)]; });
    return s;
}

std::string DataGenerator::genName() {
    std::uniform_int_distribution<int> fd(0, (int)std::size(kFirstNames) - 1);
    std::uniform_int_distribution<int> ld(0, (int)std::size(kLastNames)  - 1);
    return std::string(kFirstNames[fd(rng_)]) + " " + kLastNames[ld(rng_)];
}

std::string DataGenerator::genEmail() {
    std::uniform_int_distribution<int> fd(0, (int)std::size(kFirstNames) - 1);
    std::uniform_int_distribution<int> dd(0, (int)std::size(kDomains)    - 1);
    std::uniform_int_distribution<int> nd(100, 999);
    std::string user = kFirstNames[fd(rng_)];
    std::transform(user.begin(), user.end(), user.begin(), ::tolower);
    return user + std::to_string(nd(rng_)) + "@" + kDomains[dd(rng_)];
}

std::string DataGenerator::genDate(const ColDef& col) {
    auto parseDate = [](const std::string& s) -> std::time_t {
        std::tm t{};
        std::istringstream ss(s);
        ss >> std::get_time(&t, "%Y-%m-%d");
        return std::mktime(&t);
    };

    std::time_t tFrom = parseDate(col.dateFrom);
    std::time_t tTo   = parseDate(col.dateTo);
    if (tFrom > tTo) std::swap(tFrom, tTo);

    std::uniform_int_distribution<long long> dist(0, (long long)(tTo - tFrom));
    std::time_t chosen = tFrom + (std::time_t)dist(rng_);

    std::tm tmBuf{};
#ifdef _WIN32
    localtime_s(&tmBuf, &chosen);
#else
    localtime_r(&chosen, &tmBuf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y-%m-%d");
    return oss.str();
}

std::string DataGenerator::genBool() {
    return std::uniform_int_distribution<int>(0, 1)(rng_) ? "1" : "0";
}

std::string DataGenerator::genPhone() {
    std::uniform_int_distribution<int> d(1000, 9999);
    std::ostringstream oss;
    oss << "010-" << d(rng_) << "-" << d(rng_);
    return oss.str();
}

#include <iostream>
#include <string>
#include "Schema.h"
#include "SchemaLoader.h"
#include "DataGenerator.h"
#include "Writer.h"
#include "SQLiteDB.h"

static void printUsage(const char* prog) {
    std::cout <<
        "\n=== Dummy Data Generator ===\n"
        "Usage: " << prog << " [options]\n\n"
        "Options:\n"
        "  --schema <file>   Schema file path          (default: schema.txt)\n"
        "  --count  <n>      Number of rows to generate (default: 10)\n"
        "  --csv    <file>   Write output to CSV file\n"
        "  --sql    <file>   Write INSERT statements to .sql file\n"
        "  --db     <file>   Insert into SQLite .db file\n"
        "  --preview         Print data to console (default if no output specified)\n"
        "  --help            Show this help\n\n"
        "Example:\n"
        "  DummyDataGenerator --schema schema.txt --count 100 --csv out.csv --db test.db\n\n";
}

int main(int argc, char* argv[]) {
    std::string schemaFile = "schema.txt";
    int         count      = 10;
    std::string csvFile;
    std::string sqlFile;
    std::string dbFile;
    bool        preview    = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if      (arg == "--schema"  && i+1 < argc) schemaFile = argv[++i];
        else if (arg == "--count"   && i+1 < argc) count      = std::stoi(argv[++i]);
        else if (arg == "--csv"     && i+1 < argc) csvFile    = argv[++i];
        else if (arg == "--sql"     && i+1 < argc) sqlFile    = argv[++i];
        else if (arg == "--db"      && i+1 < argc) dbFile     = argv[++i];
        else if (arg == "--preview")               preview    = true;
        else if (arg == "--help")   { printUsage(argv[0]); return 0; }
    }

    std::cout << "=== Dummy Data Generator ===\n";

    Schema schema = SchemaLoader::load(schemaFile);
    if (schema.table.empty() || schema.columns.empty()) {
        std::cerr << "[ERROR] Invalid or empty schema.\n";
        return 1;
    }

    std::cout << "Table   : " << schema.table << '\n';
    std::cout << "Columns : " << schema.columns.size() << '\n';
    std::cout << "Count   : " << count << '\n';
    std::cout << "Generating...\n";

    DataGenerator gen(schema);
    auto rows = gen.generate(count);

    bool hasOutput = !csvFile.empty() || !sqlFile.empty() || !dbFile.empty();
    if (!hasOutput || preview) printConsole(schema, rows);
    if (!csvFile.empty())      writeCsv(schema, rows, csvFile);
    if (!sqlFile.empty())      writeSql(schema, rows, sqlFile);
    if (!dbFile.empty())       insertToSQLite(schema, rows, dbFile);

    std::cout << "\nDone. " << count << " rows generated.\n";
    return 0;
}

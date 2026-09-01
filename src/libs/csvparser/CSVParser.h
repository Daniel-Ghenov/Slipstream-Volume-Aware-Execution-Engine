#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <algorithm>
#include <charconv>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string_view>

#include "CSVTypes.h"

class CSVParser {
private:
    bool reachedEnd_ = false;
    std::ifstream file;

public:

    CSVParser(const std::string& filename);
    CSVParser(const char* filename);

    csv::Quote readQuote();

    csv::Trade readTrade();
    bool reachedEnd();

private:

    void skipHeader();

    static size_t parseTimestamp(std::string_view field);

    static size_t parsePrice(std::string_view field);

    static size_t parseUInt(std::string_view field);

    static void copySymbol(char (&dest)[SYMBOL_MAX_LEN], std::string_view field);

};

#endif //CSVPARSER_H

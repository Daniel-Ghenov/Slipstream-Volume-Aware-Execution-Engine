#include "CSVParser.h"

CSVParser::CSVParser(const std::string& filename): file(filename) {
    if (!file.is_open())
        throw std::runtime_error("Failed to open CSV file: " + filename);

    skipHeader();
}

CSVParser::CSVParser(const char* filename): file(filename) {
    if (!file.is_open())
        throw std::runtime_error("Failed to open CSV file: " + std::string(filename));

    skipHeader();
}

Quote CSVParser::readQuote() {
    if (reachedEnd_)
        throw std::out_of_range("Reached end of csv file");

    std::string line_buffer;
    while (std::getline(file, line_buffer)) {
        if (line_buffer.empty())
            continue;

        std::stringstream ss(line_buffer);
        std::string buffer;

        std::getline(ss, buffer, ',');
        const size_t timestamp = parseTimestamp(buffer);

        std::getline(ss, buffer, ',');
        if (buffer != "Q")
            continue;

        Quote quote;
        quote.timestamp = timestamp;

        std::getline(ss, buffer, ',');
        copySymbol(quote.symbol, buffer);

        std::getline(ss, buffer, ',');
        quote.bidPrice = parsePrice(buffer);

        std::getline(ss, buffer, ',');
        quote.bidQuantity = parseUInt(buffer);

        std::getline(ss, buffer, ',');
        quote.askPrice = parsePrice(buffer);

        std::getline(ss, buffer, ',');
        quote.askQuantity = parseUInt(buffer);

        return quote;
    }

    reachedEnd_ = true;
    throw std::out_of_range("Reached end of csv file");
}

Trade CSVParser::readTrade() {
    if (reachedEnd_)
        throw std::out_of_range("Reached end of csv file");

    std::string line_buffer;
    while (std::getline(file, line_buffer)) {
        if (line_buffer.empty())
            continue;

        std::stringstream ss(line_buffer);
        std::string buffer;

        std::getline(ss, buffer, ',');
        const size_t timestamp = parseTimestamp(buffer);

        std::getline(ss, buffer, ',');
        if (buffer != "T")
            continue;

        Trade trade;
        trade.timestamp = timestamp;

        std::getline(ss, buffer, ',');
        copySymbol(trade.symbol, buffer);

        while(ss.peek() == ',')
            ss.ignore();

        std::getline(ss, buffer, ',');
        trade.price = parsePrice(buffer);

        std::getline(ss, buffer, ',');
        trade.quantity = parseUInt(buffer);

        return trade;
    }

    reachedEnd_ = true;
    throw std::out_of_range("Reached end of csv file");
}

bool CSVParser::reachedEnd() {
    return reachedEnd_;
}

void CSVParser::skipHeader() {
    std::string line_buffer;
    std::streampos lastGoodPos = file.tellg();

    while (std::getline(file, line_buffer)) {
        if (line_buffer.empty() || line_buffer[0] == '#') {
            lastGoodPos = file.tellg();
            continue;
        }
        return;
    }

    file.seekg(lastGoodPos);
}

size_t CSVParser::parseTimestamp(std::string_view field) {
    auto digit = [&](size_t i) { return static_cast<unsigned>(field[i] - '0'); };

    const unsigned hours = digit(0) * 10 + digit(1);
    const unsigned minutes = digit(3) * 10 + digit(4);
    const unsigned seconds = digit(6) * 10 + digit(7);
    const unsigned millis = digit(9) * 100 + digit(10) * 10 + digit(11);

    return ((hours * 60 + minutes) * 60 + seconds) * 1000 + millis;
}

size_t CSVParser::parsePrice(std::string_view field) {
    size_t value = 0;
    int fracDigits = 0;
    bool sawPoint = false;

    for (const char c : field) {
        if (c == '.') {
            sawPoint = true;
            continue;
        }
        if (sawPoint && fracDigits == 4)
            break;

        value = value * 10 + static_cast<size_t>(c - '0');
        if (sawPoint)
            ++fracDigits;
    }

    for (; fracDigits < 4; ++fracDigits)
        value *= 10;

    return value;
}

size_t CSVParser::parseUInt(std::string_view field) {
    size_t value = 0;
    std::from_chars(field.data(), field.data() + field.size(), value);
    return value;
}

void CSVParser::copySymbol(char (&dest)[SYMBOL_MAX_LEN], std::string_view field) {
    const size_t len = std::min(field.size(), SYMBOL_MAX_LEN - 1);
    std::memcpy(dest, field.data(), len);
    dest[len] = '\0';
}

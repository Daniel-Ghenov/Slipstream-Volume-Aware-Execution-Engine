#include "OrderBookService.h"
#include <cstring>
#include <stdexcept>


OrderBookService::OrderBookService(const std::string& symbol) {
    if (symbol.size() != sizeof(this->symbol))
        throw std::invalid_argument("Symbol must be exactly " + std::to_string(sizeof(this->symbol)) + " characters (ISIN)");
    memcpy(this->symbol, symbol.data(), sizeof(this->symbol));
}

Quote OrderBookService::getQuote() {
    Quote quote = latestQuote;
    return quote;

}

void OrderBookService::setQuote(Quote quote) {
    if (memcmp(quote.symbol, symbol, sizeof(symbol)) != 0)
        return;
    latestQuote = quote;
    ++quotesReceived;
}

size_t OrderBookService::getQuotesReceived() {
    size_t qr = quotesReceived;
    return qr;
}

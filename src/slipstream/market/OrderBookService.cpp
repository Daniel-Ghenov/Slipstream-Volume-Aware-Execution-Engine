#include "OrderBookService.h"
#include <cstring>
#include <stdexcept>


OrderBookService::OrderBookService(const std::string& symbol) {
    if (symbol.size() != sizeof(this->symbol))
        throw std::invalid_argument("Symbol must be exactly " + std::to_string(sizeof(this->symbol)) + " characters (ISIN)");
    memcpy(this->symbol, symbol.data(), sizeof(this->symbol));
}

Quote OrderBookService::getQuote() {
    m.lock();
    Quote quote = latestQuote;
    m.unlock();
    return quote;

}

void OrderBookService::setQuote(Quote quote) {
    if (memcmp(quote.symbol, symbol, sizeof(symbol)) != 0)
        return;
    m.lock();
    latestQuote = quote;
    ++quotesReceived;
    m.unlock();
}

size_t OrderBookService::getQuotesReceived() {
    m.lock();
    size_t qr = quotesReceived;
    m.unlock();
    return qr;
}

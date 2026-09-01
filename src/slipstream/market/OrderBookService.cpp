#include "OrderBookService.h"
#include <algorithm>
#include <cstring>


OrderBookService::OrderBookService(const std::string& symbol) {
    size_t len = std::min(symbol.size(), sizeof(this->symbol) - 1);
    memcpy(this->symbol, symbol.data(), len);
    this->symbol[len] = '\0';
}

Quote OrderBookService::getQuote() {
    m.lock();
    Quote quote = latestQuote;
    m.unlock();
    return quote;

}

void OrderBookService::setQuote(Quote quote) {
    if (strcmp(quote.symbol, symbol) != 0)
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

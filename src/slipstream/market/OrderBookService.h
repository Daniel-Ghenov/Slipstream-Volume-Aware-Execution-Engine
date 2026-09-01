#ifndef ORDER_BOOK_SERVICE_H
#define ORDER_BOOK_SERVICE_H

#include "GCMDTypes.h"
#include <mutex>
#include <string>
class OrderBookService {
private:
    char symbol[12];
    std::mutex m;
    Quote latestQuote;
    size_t quotesReceived = 0;
public:

    OrderBookService(const std::string& symbol);

    OrderBookService(const OrderBookService&) = delete;
    OrderBookService(OrderBookService&&) = delete;
    OrderBookService operator=(const OrderBookService&) = delete;
    OrderBookService operator=(OrderBookService&&) = delete;

    Quote getQuote();
    void setQuote(Quote quote);
    size_t getQuotesReceived();
};

#endif // ORDER_BOOK_SERVICE_H

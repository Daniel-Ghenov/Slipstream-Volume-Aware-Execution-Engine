#ifndef QUOTE_H
#define QUOTE_H

#include <cstddef>
#include <string>

static const size_t SYMBOL_MAX_LEN = 12;

namespace csv {

struct Quote {
    size_t timestamp;
    char symbol [SYMBOL_MAX_LEN];
    size_t bidPrice;
    size_t bidQuantity;
    size_t askPrice;
    size_t askQuantity;
};

struct Trade {
    size_t timestamp; // ms after start of day
    char symbol [SYMBOL_MAX_LEN];
    size_t price;
    size_t quantity;
};

}

#endif //QUOTE_H

#include <gtest/gtest.h>

#include <cstring>
#include <stdexcept>

#include "GCMDTypes.h"
#include "OrderBookService.h"

namespace {

Quote makeQuote(std::string_view symbol, int64_t bidPrice, int64_t askPrice) {
    Quote quote{};
    std::memcpy(quote.symbol, symbol.data(), sizeof(quote.symbol));
    quote.bidPrice = bidPrice;
    quote.askPrice = askPrice;
    return quote;
}

}

TEST(OrderBookServiceTest, ConstructorAcceptsTwelveCharacterSymbol) {
    EXPECT_NO_THROW(OrderBookService("US0378331005"));
}

TEST(OrderBookServiceTest, ConstructorThrowsOnShortSymbol) {
    EXPECT_THROW(OrderBookService("SHORT"), std::invalid_argument);
}

TEST(OrderBookServiceTest, ConstructorThrowsOnLongSymbol) {
    EXPECT_THROW(OrderBookService("WAYTOOLONGOFASYMBOL"), std::invalid_argument);
}

TEST(OrderBookServiceTest, SetQuoteUpdatesStateWhenSymbolMatches) {
    OrderBookService obService("US0378331005");

    obService.setQuote(makeQuote("US0378331005", 1000, 1010));

    EXPECT_EQ(obService.getQuotesReceived(), 1u);
    EXPECT_EQ(obService.getQuote().bidPrice, 1000);
    EXPECT_EQ(obService.getQuote().askPrice, 1010);
}

TEST(OrderBookServiceTest, SetQuoteIsNoOpWhenSymbolDoesNotMatch) {
    OrderBookService obService("US0378331005");

    obService.setQuote(makeQuote("GB0002634946", 1000, 1010));

    EXPECT_EQ(obService.getQuotesReceived(), 0u);
}

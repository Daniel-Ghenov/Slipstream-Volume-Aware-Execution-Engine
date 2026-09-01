#include <gtest/gtest.h>

#include <cstring>
#include <fstream>
#include <string>

#include "CSVParser.h"
#include "CSVTypes.h"

namespace {

std::string writeCsv(const std::string& content) {
    const std::string path = "csvparser_test_scratch.csv";
    std::ofstream out(path, std::ios::trunc);
    out << content;
    out.close();
    return path;
}

}

TEST(CSVParserTest, SkipsCommentsAndHeaderThenParsesQuote) {
    auto path = writeCsv(
        "# Format: Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty (for quotes)\n"
        "# some other comment\n"
        "\n"
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.003,Q,SYNTH3,87.37,55,87.49,50,,\n"
    );

    CSVParser parser(path);
    csv::Quote quote = parser.readQuote();

    EXPECT_EQ(quote.timestamp, 34200003u);
    EXPECT_STREQ(quote.symbol, "SYNTH3");
    EXPECT_EQ(quote.bidPrice, 873700u);
    EXPECT_EQ(quote.bidQuantity, 55u);
    EXPECT_EQ(quote.askPrice, 874900u);
    EXPECT_EQ(quote.askQuantity, 50u);
}

TEST(CSVParserTest, ParsesTrade) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.190,T,SYNTH2,,,,,248.53,65\n"
    );

    CSVParser parser(path);
    csv::Trade trade = parser.readTrade();

    EXPECT_EQ(trade.timestamp, 34200190u);
    EXPECT_STREQ(trade.symbol, "SYNTH2");
    EXPECT_EQ(trade.price, 2485300u);
    EXPECT_EQ(trade.quantity, 65u);
}

TEST(CSVParserTest, ReadQuoteSkipsTradeRows) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.100,T,SYNTH2,,,,,248.53,65\n"
        "09:30:00.200,Q,SYNTH1,101.23,175,101.25,150,,\n"
    );

    CSVParser parser(path);
    csv::Quote quote = parser.readQuote();

    EXPECT_EQ(quote.timestamp, 34200200u);
    EXPECT_STREQ(quote.symbol, "SYNTH1");
}

TEST(CSVParserTest, ReadTradeSkipsQuoteRows) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.100,Q,SYNTH1,101.23,175,101.25,150,,\n"
        "09:30:00.200,T,SYNTH2,,,,,248.53,65\n"
    );

    CSVParser parser(path);
    csv::Trade trade = parser.readTrade();

    EXPECT_EQ(trade.timestamp, 34200200u);
    EXPECT_STREQ(trade.symbol, "SYNTH2");
}

TEST(CSVParserTest, ReachedEndThrowsAndSetsFlag) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.003,Q,SYNTH3,87.37,55,87.49,50,,\n"
    );

    CSVParser parser(path);
    parser.readQuote();

    EXPECT_FALSE(parser.reachedEnd());
    EXPECT_THROW(parser.readQuote(), std::out_of_range);
    EXPECT_TRUE(parser.reachedEnd());
}

TEST(CSVParserTest, PriceParsingHandlesWholeNumbers) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.000,Q,SYNTH1,100,10,101,20,,\n"
    );

    CSVParser parser(path);
    csv::Quote quote = parser.readQuote();

    EXPECT_EQ(quote.bidPrice, 1000000u);
    EXPECT_EQ(quote.askPrice, 1010000u);
}

TEST(CSVParserTest, PriceParsingTruncatesBeyondFourDecimals) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.000,Q,SYNTH1,87.123456,10,87.3700,20,,\n"
    );

    CSVParser parser(path);
    csv::Quote quote = parser.readQuote();

    EXPECT_EQ(quote.bidPrice, 871234u);
    EXPECT_EQ(quote.askPrice, 873700u);
}

TEST(CSVParserTest, SymbolLongerThanLimitIsTruncated) {
    auto path = writeCsv(
        "Timestamp,Type,Symbol,BidPrice,BidQty,AskPrice,AskQty,Price,Qty\n"
        "09:30:00.000,Q,SYNTHLONGNAME123,87.37,55,87.49,50,,\n"
    );

    CSVParser parser(path);
    csv::Quote quote = parser.readQuote();

    EXPECT_EQ(std::string(quote.symbol), "SYNTHLONGNA");
    EXPECT_EQ(std::strlen(quote.symbol), SYMBOL_MAX_LEN - 1);
}

TEST(CSVParserTest, ConstructorThrowsOnMissingFile) {
    EXPECT_THROW(CSVParser("definitely_does_not_exist_12345.csv"), std::runtime_error);
}

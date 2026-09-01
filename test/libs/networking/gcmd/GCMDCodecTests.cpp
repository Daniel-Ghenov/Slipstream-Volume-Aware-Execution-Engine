#include <gtest/gtest.h>

#include <cstring>
#include <vector>

#include "GCMDSerialiser.h"
#include "GCMDDeserialiser.h"
#include "GCMDTypes.h"

namespace {

std::vector<std::byte> makeBuffer() {
    return std::vector<std::byte>(GCMDSerialiser::MAX_FRAME_SIZE);
}

}

TEST(GCMDCodecTest, QuoteRoundTrip) {
    auto buf = makeBuffer();
    Quote quote{};
    std::memcpy(quote.symbol, "SYNTH1", 7);
    quote.timestampNs = 1234567890;
    quote.bidQuantity = 175;
    quote.bidPrice = 1012300;
    quote.askQuantity = 150;
    quote.askPrice = 1012500;

    size_t n = GCMDSerialiser::serialiseQuoteMessage(buf.data(), quote);
    ASSERT_GT(n, 0u);

    Quote decoded = GCMDDeserialiser::getQuoteMessage(buf.data());
    EXPECT_STREQ(decoded.symbol, "SYNTH1");
    EXPECT_EQ(decoded.timestampNs, quote.timestampNs);
    EXPECT_EQ(decoded.bidQuantity, quote.bidQuantity);
    EXPECT_EQ(decoded.bidPrice, quote.bidPrice);
    EXPECT_EQ(decoded.askQuantity, quote.askQuantity);
    EXPECT_EQ(decoded.askPrice, quote.askPrice);
}

TEST(GCMDCodecTest, TradeRoundTripEachAggressor) {
    for (TradeAggressor aggressor : {TradeAggressor::BUY, TradeAggressor::SELL, TradeAggressor::UNKNOWN}) {
        auto buf = makeBuffer();
        Trade trade{};
        std::memcpy(trade.symbol, "SYNTH4", 7);
        trade.timestampNs = 42;
        trade.quantity = 900;
        trade.price = 341800;
        trade.aggressor = aggressor;
        trade.id = 7;

        GCMDSerialiser::serialiseTradeMessage(buf.data(), trade);
        Trade decoded = GCMDDeserialiser::getTradeMessage(buf.data());

        EXPECT_STREQ(decoded.symbol, "SYNTH4");
        EXPECT_EQ(decoded.timestampNs, trade.timestampNs);
        EXPECT_EQ(decoded.quantity, trade.quantity);
        EXPECT_EQ(decoded.price, trade.price);
        EXPECT_EQ(decoded.aggressor, aggressor);
        EXPECT_EQ(decoded.id, trade.id);
    }
}

TEST(GCMDCodecTest, HeartbeatRoundTrip) {
    auto buf = makeBuffer();
    Heartbeat hb{999888};

    GCMDSerialiser::serialiseHeartBeatMessage(buf.data(), hb);
    Heartbeat decoded = GCMDDeserialiser::getHeartBeatMessage(buf.data());

    EXPECT_EQ(decoded.timestamp, hb.timestamp);
}

// This specifically exercises OPEN/HALT/CLOSE round-tripping through both
// directions (state -> wire byte -> state), not just decode alone.
TEST(GCMDCodecTest, SessionControlRoundTripEachState) {
    for (SessionControlState state : {SessionControlState::OPEN, SessionControlState::HALT, SessionControlState::CLOSE}) {
        auto buf = makeBuffer();
        SessionControl sc{555, state};

        GCMDSerialiser::serialiseSessionControlMessage(buf.data(), sc);
        SessionControl decoded = GCMDDeserialiser::getSessionControlMessage(buf.data());

        EXPECT_EQ(decoded.timestamp, sc.timestamp);
        EXPECT_EQ(decoded.state, state) << "state " << static_cast<int>(state) << " did not round-trip";
    }
}

TEST(GCMDCodecTest, DeserialiseMessageDispatchesEachType) {
    {
        auto buf = makeBuffer();
        Quote quote{};
        std::memcpy(quote.symbol, "SYNTH1", 7);
        GCMDSerialiser::serialiseQuoteMessage(buf.data(), quote);
        MDMessage msg = GCMDDeserialiser::deserialiseMessage(buf.data());
        EXPECT_TRUE(std::holds_alternative<Quote>(msg));
    }
    {
        auto buf = makeBuffer();
        Trade trade{};
        std::memcpy(trade.symbol, "SYNTH4", 7);
        trade.aggressor = TradeAggressor::BUY;
        GCMDSerialiser::serialiseTradeMessage(buf.data(), trade);
        MDMessage msg = GCMDDeserialiser::deserialiseMessage(buf.data());
        EXPECT_TRUE(std::holds_alternative<Trade>(msg));
    }
    {
        auto buf = makeBuffer();
        Heartbeat hb{1};
        GCMDSerialiser::serialiseHeartBeatMessage(buf.data(), hb);
        MDMessage msg = GCMDDeserialiser::deserialiseMessage(buf.data());
        EXPECT_TRUE(std::holds_alternative<Heartbeat>(msg));
    }
    {
        auto buf = makeBuffer();
        SessionControl sc{1, SessionControlState::OPEN};
        GCMDSerialiser::serialiseSessionControlMessage(buf.data(), sc);
        MDMessage msg = GCMDDeserialiser::deserialiseMessage(buf.data());
        EXPECT_TRUE(std::holds_alternative<SessionControl>(msg));
    }
}

TEST(GCMDCodecTest, MessageTypeRoundTrip) {
    for (network::MessageType type : {network::MessageType::QUOTE, network::MessageType::TRADE,
                                        network::MessageType::HEARTBEAT, network::MessageType::SESSION_CONTROL}) {
        EXPECT_EQ(messageTypeFromNum(messageTypeToNum(type)), type);
    }
}

TEST(GCMDCodecTest, MessageTypeFromNumThrowsOnUnknownValue) {
    EXPECT_THROW(messageTypeFromNum(255), std::logic_error);
}

TEST(GCMDCodecTest, AggressorRoundTrip) {
    for (TradeAggressor a : {TradeAggressor::BUY, TradeAggressor::SELL, TradeAggressor::UNKNOWN}) {
        EXPECT_EQ(aggressorFromChar(aggressorToChar(a)), a);
    }
}

TEST(GCMDCodecTest, AggressorFromCharThrowsOnUnknownValue) {
    EXPECT_THROW(aggressorFromChar('Z'), std::logic_error);
}

TEST(GCMDCodecTest, SessionControlStateRoundTrip) {
    for (SessionControlState s : {SessionControlState::OPEN, SessionControlState::HALT, SessionControlState::CLOSE}) {
        EXPECT_EQ(sessionControlStateFromNum(sessionControlStateToNum(s)), s)
            << "state " << static_cast<int>(s) << " did not round-trip through to/from num";
    }
}

TEST(GCMDCodecTest, SessionControlStateFromNumThrowsOnUnknownValue) {
    EXPECT_THROW(sessionControlStateFromNum(255), std::logic_error);
}

#include <gtest/gtest.h>

#include <cstring>
#include <vector>

#include "OESerialiser.h"
#include "OEDeserialiser.h"
#include "OETypes.h"

namespace {

std::vector<std::byte> makeBuffer() {
    return std::vector<std::byte>(OESerialiser::MAX_FRAME_SIZE);
}

}

class OECodecTest : public ::testing::Test {
protected:
    OEDeserialiser deserialiser;
};

TEST_F(OECodecTest, NewOrderRoundTrip) {
    auto buf = makeBuffer();
    network::NewOrderBody order{};
    order.clientOrderId = 42;
    std::memcpy(order.symbol, "SYNTH2", 7);
    order.status = 'A';
    order.tsNs = 123456789;
    order.tradeId = 17;
    order.side = 'B';
    order.qty = 500;
    order.limitPx = 2485300;

    size_t n = OESerialiser::serialiseNewOrderMessage(buf.data(), order);
    ASSERT_GT(n, 0u);

    network::NewOrderBody decoded = deserialiser.getNewOrderMessage(buf.data());
    EXPECT_EQ(decoded.clientOrderId, order.clientOrderId);
    EXPECT_STREQ(decoded.symbol, "SYNTH2");
    EXPECT_EQ(decoded.status, order.status);
    EXPECT_EQ(decoded.tsNs, order.tsNs);
    EXPECT_EQ(decoded.tradeId, order.tradeId);
    EXPECT_EQ(decoded.side, order.side);
    EXPECT_EQ(decoded.qty, order.qty);
    EXPECT_EQ(decoded.limitPx, order.limitPx);
}

TEST_F(OECodecTest, ExecReportRoundTrip) {
    auto buf = makeBuffer();
    network::ExecReportBody report{};
    report.clientOrderId = 99;
    report.tsNs = 555;
    report.status = 1; // FILL
    report.filledQty = 300;
    report.avgPx = 341800;
    report.reasonCode = 0; // NONE

    OESerialiser::serialiseExecReportMessage(buf.data(), report);
    network::ExecReportBody decoded = deserialiser.getExecReportMessage(buf.data());

    EXPECT_EQ(decoded.clientOrderId, report.clientOrderId);
    EXPECT_EQ(decoded.tsNs, report.tsNs);
    EXPECT_EQ(decoded.status, report.status);
    EXPECT_EQ(decoded.filledQty, report.filledQty);
    EXPECT_EQ(decoded.avgPx, report.avgPx);
    EXPECT_EQ(decoded.reasonCode, report.reasonCode);
}

TEST_F(OECodecTest, GetMessageTypeMatchesWhatWasSent) {
    {
        auto buf = makeBuffer();
        network::NewOrderBody order{};
        OESerialiser::serialiseNewOrderMessage(buf.data(), order);
        EXPECT_EQ(deserialiser.getMessageType(buf.data()), network::OEMessageType::NEW_ORDER);
    }
    {
        auto buf = makeBuffer();
        network::ExecReportBody report{};
        OESerialiser::serialiseExecReportMessage(buf.data(), report);
        EXPECT_EQ(deserialiser.getMessageType(buf.data()), network::OEMessageType::EXEC_REPORT);
    }
}

TEST(OETypesTest, ToNewOrderDecodesEnumsCorrectly) {
    network::NewOrderBody body{};
    body.clientOrderId = 1;
    std::memcpy(body.symbol, "SYNTH3", 7);
    body.status = 'R';
    body.tsNs = 2;
    body.tradeId = 3;
    body.side = 'S';
    body.qty = 4;
    body.limitPx = 5;

    NewOrder order = toNewOrder(body);
    EXPECT_EQ(order.clientOrderId, 1u);
    EXPECT_STREQ(order.symbol, "SYNTH3");
    EXPECT_EQ(order.status, OrderStatus::REJECTED);
    EXPECT_EQ(order.tsNs, 2u);
    EXPECT_EQ(order.tradeId, 3);
    EXPECT_EQ(order.side, OrderSide::SELL);
    EXPECT_EQ(order.qty, 4u);
    EXPECT_EQ(order.limitPx, 5);
}

TEST(OETypesTest, ToExecReportDecodesEnumsCorrectly) {
    network::ExecReportBody body{};
    body.clientOrderId = 10;
    body.tsNs = 20;
    body.status = 2; // PARTIAL
    body.filledQty = 30;
    body.avgPx = 40;
    body.reasonCode = 3; // SIZE

    ExecReport report = toExecReport(body);
    EXPECT_EQ(report.clientOrderId, 10u);
    EXPECT_EQ(report.tsNs, 20u);
    EXPECT_EQ(report.status, ExecStatus::PARTIAL);
    EXPECT_EQ(report.filledQty, 30u);
    EXPECT_EQ(report.avgPx, 40);
    EXPECT_EQ(report.reasonCode, RejectReason::SIZE);
}

TEST(OETypesTest, OEMessageTypeRoundTrip) {
    for (network::OEMessageType t : {network::OEMessageType::NEW_ORDER, network::OEMessageType::EXEC_REPORT}) {
        EXPECT_EQ(oeMessageTypeFromNum(oeMessageTypeToNum(t)), t);
    }
}

TEST(OETypesTest, OrderStatusRoundTrip) {
    for (OrderStatus s : {OrderStatus::ACCEPTED, OrderStatus::REJECTED}) {
        EXPECT_EQ(orderStatusFromChar(orderStatusToChar(s)), s);
    }
}

TEST(OETypesTest, OrderSideRoundTrip) {
    for (OrderSide s : {OrderSide::BUY, OrderSide::SELL}) {
        EXPECT_EQ(orderSideFromChar(orderSideToChar(s)), s);
    }
}

TEST(OETypesTest, ExecStatusRoundTrip) {
    for (ExecStatus s : {ExecStatus::ACK, ExecStatus::FILL, ExecStatus::PARTIAL, ExecStatus::REJECT}) {
        EXPECT_EQ(execStatusFromNum(execStatusToNum(s)), s);
    }
}

TEST(OETypesTest, RejectReasonRoundTrip) {
    for (RejectReason r : {RejectReason::NONE, RejectReason::RISK, RejectReason::PRICE,
                             RejectReason::SIZE, RejectReason::THROTTLE}) {
        EXPECT_EQ(rejectReasonFromNum(rejectReasonToNum(r)), r);
    }
}

TEST(OETypesTest, InvalidCodesThrow) {
    EXPECT_THROW(oeMessageTypeFromNum(255), std::logic_error);
    EXPECT_THROW(orderStatusFromChar('Z'), std::logic_error);
    EXPECT_THROW(orderSideFromChar('Z'), std::logic_error);
    EXPECT_THROW(execStatusFromNum(255), std::logic_error);
    EXPECT_THROW(rejectReasonFromNum(255), std::logic_error);
}

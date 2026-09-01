#include <gtest/gtest.h>

#include <vector>

#include "MessageHandler.h"
#include "GCMDDeserialiser.h"
#include "GCMDSerialiser.h"
#include "GCMDTypes.h"

namespace {

std::vector<std::byte> encodeHeartbeat(uint64_t timestamp) {
    std::vector<std::byte> buf(GCMDSerialiser::MAX_FRAME_SIZE);
    Heartbeat body{timestamp};
    const size_t n = GCMDSerialiser::serialiseHeartBeatMessage(buf.data(), body);
    buf.resize(n);
    return buf;
}

} // namespace

// MessageHandler itself is protocol-agnostic (it only knows FrameHeader);
// GCMDDeserialiser here is used purely to decode message bodies for
// assertions, not passed to the handler.
class MessageHandlerTest : public ::testing::Test {
protected:
    GCMDDeserialiser deserialiser;
    MessageHandler handler;
};

TEST_F(MessageHandlerTest, SingleMessageWholeInOneCall) {
    const auto bytes = encodeHeartbeat(12345);

    const MessagesRecieved result = handler.recieve(const_cast<std::byte*>(bytes.data()), bytes.size());

    ASSERT_EQ(result.messagesCount, 1u);
    EXPECT_EQ(messageTypeFromNum(static_cast<network::FrameHeader*>(result.start)->msgType), network::MessageType::HEARTBEAT);
    EXPECT_EQ(deserialiser.getHeartBeatMessage(result.start).timestamp, 12345u);
}

// The exact scenario this class exists for: a message whose bytes are
// delivered across two separate recv() calls must still produce exactly
// one coherent message once the second call completes it.
TEST_F(MessageHandlerTest, MessageSplitAcrossTwoChunks) {
    const auto bytes = encodeHeartbeat(999);
    const size_t splitPoint = bytes.size() / 2;

    const MessagesRecieved first = handler.recieve(const_cast<std::byte*>(bytes.data()), splitPoint);
    EXPECT_EQ(first.messagesCount, 0u) << "should not report a message before the body is complete";

    const MessagesRecieved second = handler.recieve(
        const_cast<std::byte*>(bytes.data()) + splitPoint, bytes.size() - splitPoint);

    ASSERT_EQ(second.messagesCount, 1u);
    EXPECT_EQ(deserialiser.getHeartBeatMessage(second.start).timestamp, 999u);
}

// Same idea, pushed to the extreme: one byte per recv() call. The parser
// must stay silent for every partial call and emit exactly one message
// on the call that completes the frame, regardless of how finely the
// stream was chopped up.
TEST_F(MessageHandlerTest, MessageSplitByteByByte) {
    const auto bytes = encodeHeartbeat(42);

    for (size_t i = 0; i + 1 < bytes.size(); ++i) {
        const MessagesRecieved r = handler.recieve(const_cast<std::byte*>(bytes.data()) + i, 1);
        EXPECT_EQ(r.messagesCount, 0u) << "unexpected complete message after byte " << i;
    }

    const MessagesRecieved last = handler.recieve(
        const_cast<std::byte*>(bytes.data()) + bytes.size() - 1, 1);

    ASSERT_EQ(last.messagesCount, 1u);
    EXPECT_EQ(deserialiser.getHeartBeatMessage(last.start).timestamp, 42u);
}

// A single recv() can also hand back more than one frame at once (the
// sender wrote them back-to-back). Both must come out correctly, in order.
TEST_F(MessageHandlerTest, TwoMessagesInOneCall) {
    const auto first = encodeHeartbeat(1);
    const auto second = encodeHeartbeat(2);

    std::vector<std::byte> combined = first;
    combined.insert(combined.end(), second.begin(), second.end());

    const MessagesRecieved result = handler.recieve(combined.data(), combined.size());

    ASSERT_EQ(result.messagesCount, 2u);
    EXPECT_EQ(deserialiser.getHeartBeatMessage(result.start).timestamp, 1u);

    auto* secondHeader = reinterpret_cast<network::FrameHeader*>(
        reinterpret_cast<std::byte*>(result.start) + sizeof(network::FrameHeader) + static_cast<network::FrameHeader*>(result.start)->bodyLen);
    EXPECT_EQ(deserialiser.getHeartBeatMessage(secondHeader).timestamp, 2u);
}

// The tail end of frame N and the leading (partial) bytes of frame N+1
// can arrive in the same recv() call, with the rest of frame N+1 only
// showing up on a later call. Frame N must come out immediately; frame
// N+1 must wait for its remaining bytes.
TEST_F(MessageHandlerTest, CompleteMessagePlusPartialNextSplitAcrossCalls) {
    const auto first = encodeHeartbeat(111);
    const auto second = encodeHeartbeat(222);
    const size_t secondSplit = second.size() / 2;

    std::vector<std::byte> firstChunk = first;
    firstChunk.insert(firstChunk.end(), second.begin(), second.begin() + secondSplit);

    const MessagesRecieved r1 = handler.recieve(firstChunk.data(), firstChunk.size());
    ASSERT_EQ(r1.messagesCount, 1u) << "the complete leading message should be emitted immediately";
    EXPECT_EQ(deserialiser.getHeartBeatMessage(r1.start).timestamp, 111u);

    const MessagesRecieved r2 = handler.recieve(
        const_cast<std::byte*>(second.data()) + secondSplit, second.size() - secondSplit);

    ASSERT_EQ(r2.messagesCount, 1u) << "the trailing partial message should complete once its bytes arrive";
    EXPECT_EQ(deserialiser.getHeartBeatMessage(r2.start).timestamp, 222u);
}

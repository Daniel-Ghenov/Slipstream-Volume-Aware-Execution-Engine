#include <chrono>
#include <thread>

#include "CSVParser.h"

static const char* CSV_FILENAME = "";

void sendQuote(const Quote& quote) {

}

void sendHeartBeat() {

}

size_t getNow() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

int main() {

    CSVParser parser{CSV_FILENAME};

    Quote firstQuote = parser.readQuote();
    size_t firstTimestamp = firstQuote.timestamp;
    size_t startTimeMillis = getNow();
    size_t startTimestamp = startTimeMillis - firstTimestamp;

    sendQuote(firstQuote);
    while(!parser.reachedEnd()) {
        try {
            Quote quote = parser.readQuote();
            size_t timeToSend = startTimestamp + quote.timestamp;
            size_t now = getNow();
            while(now < timeToSend) {
                std::this_thread::sleep_for(std::chrono::milliseconds(timeToSend - now));
            }
            sendQuote(quote);
        } catch (std::out_of_range& err)
        {
            break;
        }
    }

    return 0;
}

#include "CSVParser.h"
#include "ClientConnection.h"
#include "MarketClient.h"

static const char* CSV_FILENAME = "";


int main() {

    CSVParser parser{CSV_FILENAME};
    ClientConnection connection("localhost", 2550);
    MarketClient client(&connection, &parser);

    client.run();
    return 0;
}

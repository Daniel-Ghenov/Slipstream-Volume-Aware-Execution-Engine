
#include "CSVParser.h"
#include "ClientConnection.h"
#include "OrderClient.h"

static const char* CSV_FILENAME = "";


int main() {

    CSVParser parser{CSV_FILENAME};
    ClientConnection connection("localhost", 2551);

    TradeClient client(&connection, &parser);

    return 0;
}

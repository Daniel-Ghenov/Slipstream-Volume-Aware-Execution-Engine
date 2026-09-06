#include "OrderMessageSender.h"



void OrderMessageSender::send(NewOrder& order) {

    network::NewOrderBody body = order.toBody();

    size_t bytes = OESerialiser::serialiseNewOrderMessage(buffer, body);
    clientConnection->sendBuffer(buffer, bytes);

}

void OrderMessageSender::send(ExecReport& report) {

    network::ExecReportBody body = report.toBody();

    size_t bytes = OESerialiser::serialiseExecReportMessage(buffer, body);
    clientConnection->sendBuffer(buffer, bytes);

}


#include "OrderMessageSender.h"


OrderMessageSender::~OrderMessageSender() {
    shouldStop = true;
    if (runningThread.joinable())
        runningThread.join();
}

void OrderMessageSender::send(NewOrder& order) {
    messageQueue.push(order);
}

void OrderMessageSender::send(ExecReport& report) {
    messageQueue.push(report);

}

void OrderMessageSender::run() {
    while (!shouldStop) {
        while (messageQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::variant<NewOrder, ExecReport> message = messageQueue.pop();

        if (std::holds_alternative<NewOrder>(message)) {
            NewOrder& order = std::get<NewOrder>(message);
            sendNewOrder(order);
        } else if (std::holds_alternative<ExecReport>(message)) {
            ExecReport& report = std::get<ExecReport>(message);
            sendExecReport(report);
        }

    }
}
void OrderMessageSender::sendNewOrder(NewOrder& order) {
    
    network::NewOrderBody body = order.toBody();

    size_t bytes = OESerialiser::serialiseNewOrderMessage(buffer, body);
    clientConnection->sendBuffer(buffer, bytes);
}
void OrderMessageSender::sendExecReport(ExecReport& report) {
    
    network::ExecReportBody body = report.toBody();

    size_t bytes = OESerialiser::serialiseExecReportMessage(buffer, body);
    clientConnection->sendBuffer(buffer, bytes);
}

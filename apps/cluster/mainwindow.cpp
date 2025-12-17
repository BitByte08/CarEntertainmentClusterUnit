#include "mainwindow.hpp"
#include "SocketCANInterface.hpp"
#include <iostream>

#include <QTimer>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    can_interface_ = std::make_unique<SocketCANInterface>("vcan0");

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        can_frame frame{};
        if(can_interface_->recive(frame)) {
            // 수신된 CAN 프레임 처리
            std::cout << "Received CAN frame with ID: " << std::hex << frame.can_id << std::dec << std::endl;
        }
    });

    timer->start(10);
}
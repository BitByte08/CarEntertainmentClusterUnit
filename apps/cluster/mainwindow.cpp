#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "models/ClusterModel.hpp"
#include "widgets/GaugeWidget.hpp"
#include "SocketCANInterface.hpp"

#include <memory>

MainWindow::MainWindow(QWidget* parent) 
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      cluster_model_(nullptr) {
    
    ui->setupUi(this);
    
    // ClusterModel 생성 및 CAN 인터페이스 설정
    cluster_model_ = new ClusterModel(this);
    auto can_interface = std::make_unique<SocketCANInterface>("vcan0");
    cluster_model_->setCANInterface(std::move(can_interface));
    
    setupGauges();
    connectSignals();
    
    // CAN 수신 시작
    cluster_model_->startReceiving();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupGauges() {
    // 속도계 설정 (눈금 10km, 숫자 20km 단위)
    ui->speedGauge->setMinValue(0);
    ui->speedGauge->setMaxValue(200);
    ui->speedGauge->setMajorTicks(20);  // 10km 간격 눈금
    ui->speedGauge->setUnit("km/h");
    ui->speedGauge->setLabel("SPEED");
    
    // RPM 계 설정 (눈금 500, 숫자 1000 단위)
    ui->rpmGauge->setMinValue(0);
    ui->rpmGauge->setMaxValue(8000);
    ui->rpmGauge->setMajorTicks(16);  // 500 간격 눈금
    ui->rpmGauge->setUnit("RPM");
    ui->rpmGauge->setLabel("ENGINE");
}

void MainWindow::connectSignals() {
    // ClusterModel의 신호를 GaugeWidget의 슬롯에 연결
    connect(cluster_model_, &ClusterModel::speedChanged,
            ui->speedGauge, &GaugeWidget::setValue);
    
    connect(cluster_model_, &ClusterModel::rpmChanged,
            ui->rpmGauge, &GaugeWidget::setValue);
}
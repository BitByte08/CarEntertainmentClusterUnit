#ifndef CLUSTER_MAINWINDOW_HPP
#define CLUSTER_MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>

#include "CANInterface.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;
private:
    std::unique_ptr<CANInterface> can_interface_;
};
#endif // CLUSTER_MAINWINDOW_HPP
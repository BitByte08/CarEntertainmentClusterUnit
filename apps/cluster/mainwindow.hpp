#ifndef CLUSTER_MAINWINDOW_HPP
#define CLUSTER_MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class ClusterModel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setupGauges();
    void connectSignals();

    Ui::MainWindow *ui;
    ClusterModel *cluster_model_;
};
#endif // CLUSTER_MAINWINDOW_HPP
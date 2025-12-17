#ifndef CORE_CANINTERFACE_H
#define CORE_CANINTERFACE_H

#include <QObject>

struct can_frame;

class CANInterface : public QObject {
public:
    explicit CANInterface(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~CANInterface();
    virtual void start() = 0;
    virtual void stop() = 0;
signals:
    void frameReceived(const can_frame &frame);
};

#endif // CORE_CANINTERFACE_H

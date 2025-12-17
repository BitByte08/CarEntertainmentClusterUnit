#ifndef CORE_CANINTERFACE_H
#define CORE_CANINTERFACE_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <net/if.h>
#include <cstring>
#include <stdexcept>
#include <string>

struct can_frame;

class CANInterface {
public:
    CANInterface() = default;
    virtual ~CANInterface();
    virtual bool recive(can_frame &frame) = 0;
};

#endif // CORE_CANINTERFACE_H

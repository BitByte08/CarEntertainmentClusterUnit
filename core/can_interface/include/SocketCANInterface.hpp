#ifndef CORE_SOCKETCANINTERFACE_CPP
#define CORE_SOCKETCANINTERFACE_CPP
#include "CANInterface.hpp"

class SocketCANInterface : public CANInterface {
public:
    explicit SocketCANInterface(const std::string &interface_name);
    ~SocketCANInterface() override;

    bool recive(can_frame &frame) override;
private:
    int socket_fd_{-1};
};

#endif // CORE_SOCKETCANINTERFACE_CPP

#include "SocketCANInterface.hpp"

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <net/if.h>
#include <stdexcept>

SocketCANInterface::SocketCANInterface(const std::string &interface_name) {
    socket_fd_ = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd_ < 0) throw std::runtime_error("Failed to create CAN socket");
    
    struct ifreq ifr{};
    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ -1);
    if(ioctl(socket_fd_, SIOCGIFINDEX, &ifr) < 0) {
        close(socket_fd_);
        throw std::runtime_error("Failed to get interface index for " + interface_name);
    }

    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(socket_fd_, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(socket_fd_);
        throw std::runtime_error("Failed to bind CAN socket to interface " + interface_name);
    }
}
SocketCANInterface::~SocketCANInterface() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
    }
}
bool SocketCANInterface::recive(can_frame &frame) {
    ssize_t nbytes = read(socket_fd_, &frame, sizeof(struct can_frame));
    return nbytes == sizeof(struct can_frame);
}
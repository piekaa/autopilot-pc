#ifndef MSFS_CONTROLLER_SDKCONNECTIONMUTEX_H
#define MSFS_CONTROLLER_SDKCONNECTIONMUTEX_H
#include <mutex>

class SdkConnectionMutex {
public:
    static inline std::mutex mtx;
};

#endif //MSFS_CONTROLLER_SDKCONNECTIONMUTEX_H

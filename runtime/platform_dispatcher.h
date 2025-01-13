#ifndef KEELS_RUNTIME_PLATFORM_DISPATCHER_H_
#define KEELS_RUNTIME_PLATFORM_DISPATCHER_H_

#include "flutter/lib/ui/window/platform_configuration.h"

namespace keels{

class PlatformDispatcher{
private:
    PlatformDispatcher();

public:
    ~PlatformDispatcher()=default;
    static PlatformDispatcher& instance();

    void SetPlatformConfiguration(flutter::PlatformConfiguration* config) {config_ = config;}
    flutter::PlatformConfiguration* GetPlatformConfiguration() {return config_;}


private:
    flutter::PlatformConfiguration* config_ = nullptr;

};

}

#endif
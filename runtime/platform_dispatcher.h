#ifndef KEELS_RUNTIME_PLATFORM_DISPATCHER_H_
#define KEELS_RUNTIME_PLATFORM_DISPATCHER_H_

#include "flutter/lib/ui/window/platform_configuration.h"
#include "flutter/fml/closure.h"

namespace keels{

class PlatformDispatcher{
private:
    PlatformDispatcher();

public:
    ~PlatformDispatcher()=default;
    static PlatformDispatcher& instance();

    void SetPlatformConfiguration(flutter::PlatformConfiguration* config) {config_ = config;}
    flutter::PlatformConfiguration* GetPlatformConfiguration() {return config_;}
    void SetOnBeginFrame(const fml::closure& fn) {on_begin_frame_ = fn;}
    const fml::closure& GetOnBeginFrame() {return on_begin_frame_;}

private:
    flutter::PlatformConfiguration* config_ = nullptr;
    fml::closure on_begin_frame_;


};

}

#endif
#include "flutter/runtime/platform_dispatcher.h"

namespace keels{

PlatformDispatcher::PlatformDispatcher() {
    //TODO missed
}

PlatformDispatcher& PlatformDispatcher::instance() {
    static std::unique_ptr<PlatformDispatcher> inst = std::unique_ptr<PlatformDispatcher>(new PlatformDispatcher);
    return *inst;
}


}
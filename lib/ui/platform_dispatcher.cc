#include "flutter/lib/ui/platform_dispatcher.h"
#include "flutter/lib/ui/window.h"

namespace keels{

PlatformDispatcher::PlatformDispatcher() {
    //TODO missed
}

PlatformDispatcher& PlatformDispatcher::instance() {
    static std::unique_ptr<PlatformDispatcher> inst = std::unique_ptr<PlatformDispatcher>(new PlatformDispatcher);
    return *inst;
}

void PlatformDispatcher::AddView(const int64_t view_id, const flutter::ViewportMetrics& view_metrics) {
    auto ptr = std::make_unique<FlutterView>(view_id, *this, view_metrics);
    views_[view_id] = std::move(ptr);
}


}
#include "flutter/lib/ui/window.h"

namespace keels {
FlutterView::FlutterView(int64_t view_id, PlatformDispatcher& platform_dispatcher,const flutter::ViewportMetrics& view_metrics):
    view_id_(view_id), platform_dispatcher_(platform_dispatcher), view_metrics_(view_metrics){}
}


#ifndef KEELS_LIB_UI_WINDOW_H_
#define KEELS_LIB_UI_WINDOW_H_

#include "flutter/lib/ui/window/platform_configuration.h"

//dart file: flutter/bin/cache/pkg/sky_engine/lib/ui/window.dart

namespace keels{
class PlatformDispatcher;
class FlutterView{
public:
    FlutterView(int64_t view_id, PlatformDispatcher& platform_dispatcher,const flutter::ViewportMetrics& view_metrics);
private:
    int64_t view_id_;
    PlatformDispatcher& platform_dispatcher_;
    const flutter::ViewportMetrics view_metrics_;
};
}


#endif
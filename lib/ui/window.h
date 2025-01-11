#ifndef KEELS_LIB_UI_WINDOW_H_
#define KEELS_LIB_UI_WINDOW_H_

#include "flutter/lib/ui/window/platform_configuration.h"
#include "flutter/lib/ui/geometry.h"
#include "flutter/lib/ui/compositing/scene.h"

//dart file: flutter/bin/cache/pkg/sky_engine/lib/ui/window.dart

namespace keels{
class PlatformDispatcher;
class FlutterView{
public:
    FlutterView(int64_t view_id, PlatformDispatcher& platform_dispatcher,const flutter::ViewportMetrics& view_metrics);
    double devicePixelRatio() const { return view_metrics_.device_pixel_ratio;}
    Size physicalSize() const {return Size(view_metrics_.physical_width, view_metrics_.physical_height);}
    flutter::ViewportMetrics& view_metrics() {return view_metrics_;}
    int64_t viewId()const {return view_id_;} 
    void render(fml::RefPtr<flutter::Scene> scene, std::optional<Size> size=std::nullopt);
private:
    int64_t view_id_;
    PlatformDispatcher& platform_dispatcher_;
    flutter::ViewportMetrics view_metrics_;
};
}


#endif
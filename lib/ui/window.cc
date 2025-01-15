#include "flutter/lib/ui/window.h"
#include "flutter/lib/ui/platform_dispatcher.h"

namespace keels {
FlutterView::FlutterView(int64_t view_id, PlatformDispatcher& platform_dispatcher,const flutter::ViewportMetrics& view_metrics):
    view_id_(view_id), platform_dispatcher_(platform_dispatcher), view_metrics_(view_metrics){}

void FlutterView::render(fml::RefPtr<flutter::Scene> scene, std::optional<Size> size) {
    auto cfg = platform_dispatcher_.GetPlatformConfiguration();
    double width = size.has_value() ? size.value().width : view_metrics_.physical_width;
    double height = size.has_value() ? size.value().height : view_metrics_.physical_height;
    cfg->client()->Render(view_id_, scene.get(), width, height);

}

}

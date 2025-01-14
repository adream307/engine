#ifndef KEELS_LIB_UI_PLATFORM_DISPATCHER_H_
#define KEELS_LIB_UI_PLATFORM_DISPATCHER_H_

#include "flutter/lib/ui/window/platform_configuration.h"
#include "flutter/common/constants.h"
namespace keels{

class FlutterView;

class PlatformDispatcher{
private:
    PlatformDispatcher();

public:
    ~PlatformDispatcher()=default;
    static PlatformDispatcher& instance();

    std::shared_ptr<FlutterView> implicitView();

    void SetPlatformConfiguration(flutter::PlatformConfiguration* config) {config_ = config;}
    flutter::PlatformConfiguration* GetPlatformConfiguration() {return config_;}

    void SetOnBeginFrame(const std::function<void(int)>& fn) {on_begin_frame_ = fn;}
    const std::function<void(int)>& GetOnBeginFrame() {return on_begin_frame_;}

    void SetOnDrawFrame(const std::function<void()>& fn) {on_draw_frame_ = fn;}
    const std::function<void()>& GetOnDrawFrame() {return on_draw_frame_;}

    void SetOnUpdateFrameData(const std::function<void(int)>& fn) {on_update_frame_data_ = fn;}
    const std::function<void(int)>& GetOnUpdateFrameData() {return on_update_frame_data_;}

    void AddView(const int64_t view_id, const flutter::ViewportMetrics& view_metrics);

    int64_t GetImplicitViewId() const {return flutter::kFlutterImplicitViewId;}

private:
    flutter::PlatformConfiguration* config_ = nullptr;
    std::function<void(int microseconds)> on_begin_frame_;
    std::function<void()> on_draw_frame_;
    std::function<void(int frameNumber)> on_update_frame_data_;
    std::unordered_map<int64_t, std::shared_ptr<FlutterView>> views_;
};

}

#endif
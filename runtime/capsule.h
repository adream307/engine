#ifndef KEELS_RUNTIME_CAPSULE_H_
#define KEELS_RUNTIME_CAPSULE_H_

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_set>

#include "flutter/common/task_runners.h"
#include "flutter/fml/compiler_specific.h"
#include "flutter/fml/macros.h"
#include "flutter/fml/mapping.h"
#include "flutter/lib/ui/io_manager.h"
#include "flutter/lib/ui/snapshot_delegate.h"
#include "flutter/lib/ui/window/platform_configuration.h"
#include "flutter/runtime/dart_isolate.h"

namespace keels{

class Capsule :public std::enable_shared_from_this<Capsule> {

public:
    static std::weak_ptr<Capsule> CreateRunningCapsule (
        const flutter::Settings& settings,
        std::unique_ptr<flutter::PlatformConfiguration> platform_configuration,
        const fml::closure& capsule_create_callback
    );

    std::weak_ptr<Capsule> GetWeakPtr() {
        std::shared_ptr<Capsule> ptr = shared_from_this();
        return ptr;
    }

    flutter::PlatformConfiguration* platform_configuration() const {
        return platform_configuration_.get();
    }

private:
    std::unique_ptr<flutter::PlatformConfiguration> platform_configuration_;


};

}

#endif
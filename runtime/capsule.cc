#include "flutter/runtime/capsule.h"
#include <iostream>
#include <thread>

namespace keels{

std::weak_ptr<Capsule> Capsule::CreateRunningCapsule(
    const flutter::Settings& settings,
    std::unique_ptr<flutter::PlatformConfiguration> platform_configuration,
    const fml::closure& capsule_create_callback) {
    
    auto ptr = std::make_unique<std::shared_ptr<Capsule>>(std::make_shared<Capsule>());
    auto raw = ptr.get();
    ptr.release();
    (*raw)->platform_configuration_ = std::move(platform_configuration);
    
    return (*raw)->GetWeakPtr();
}


}
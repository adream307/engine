#include "flutter/lib/ui/text.h"

namespace keels{


ParagraphStyle::ParagraphStyle(const std::optional<TextDirection>& textDirection)
    :encoded_(encodeParagraphStyle(textDirection)){}

//TODO
std::vector<int32_t> ParagraphStyle::encodeParagraphStyle(
    const std::optional<TextDirection>& textDirection) {
    std::vector<int32_t> result;
    result.resize(7,0);

    if(textDirection.has_value()) {
        result[0] |= 1 << 2;
        result[2] = static_cast<int32_t>(textDirection.value());
    }
    return result;
}


}
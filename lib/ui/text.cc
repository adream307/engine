#include "flutter/lib/ui/text.h"

namespace keels{


//TODO
ParagraphStyle::ParagraphStyle(const std::optional<TextDirection>& textDirection)
    :encoded_(encodeParagraphStyle(textDirection)),
     leadingDistribution_(TextLeadingDistribution::proportional)
    {}

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

NativeParagraphBuilder::NativeParagraphBuilder(const ParagraphStyle &style)
    :defaultLeadingDistribution_(style.leadingDistribution())
{}


}
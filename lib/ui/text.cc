#include "flutter/lib/ui/text.h"

namespace keels{


//TODO
ParagraphStyle::ParagraphStyle(const std::optional<TextAlign>& textAlign,
                               const std::optional<TextDirection>& textDirection,
                               const std::optional<double>& fontSize)
    :encoded_(encodeParagraphStyle(textAlign,textDirection,fontSize)),
     fontSize_(fontSize),
     leadingDistribution_(TextLeadingDistribution::proportional)
    {}

//TODO
std::vector<int32_t> ParagraphStyle::encodeParagraphStyle(
    const std::optional<TextAlign>& textAlign,
    const std::optional<TextDirection>& textDirection,
    const std::optional<double>& fontSize) {
    std::vector<int32_t> result;
    result.resize(7,0);

    if (textAlign.has_value()) {
        result[0] |= 1 << 1;
        result[1] = static_cast<int32_t>(textAlign.value());
    }

    if(textDirection.has_value()) {
        result[0] |= 1 << 2;
        result[2] = static_cast<int32_t>(textDirection.value());
    }

    if (fontSize.has_value()) {
        result[0] |= 1 << 8;
    }

    return result;
}

NativeParagraphBuilder::NativeParagraphBuilder(const ParagraphStyle &style)
    :defaultLeadingDistribution_(style.leadingDistribution())
{}


}
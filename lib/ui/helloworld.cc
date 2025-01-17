#include "flutter/lib/ui/helloworld.h"
namespace keels{

TextSpan::TextSpan(const std::u16string& text): text_(text) {
}

void TextSpan::build(fml::RefPtr<flutter::ParagraphBuilder> builder) {
    builder->addText2(text_);
}

ParagraphStyle TextStyle::getParagraphStyle(const TextAlign &textAlign,
                                            const TextDirection &textDirection,
                                            const TextScaler &textScaler) {
    return ParagraphStyle(textAlign, textDirection, textScaler.scale(kDefaultFontSize));
}

TextPainter::TextPainter():
    textAlign_(TextAlign::start),
    textDirection_(TextDirection::rtl),
    textScaler_(TextScaler::LinearTextScaler(1.0))
{}

ParagraphStyle TextPainter::_createParagraphStyle(const std::optional<TextAlign> &textAlignOverride)
{
    TextStyle baseStyle;
    return baseStyle.getParagraphStyle(
        textAlignOverride.has_value() ? textAlignOverride.value() : textAlign_,
        textDirection_,
        textScaler_
    );
}

fml::RefPtr<flutter::ParagraphBuilder> TextPainter::_createParagraph(InlineSpan &text)
{
    auto style = _createParagraphStyle();
    auto builder = fml::MakeRefCounted<flutter::ParagraphBuilder>(style.encoded());
    text.build(builder);
    return builder;
}

}
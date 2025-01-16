#include "flutter/lib/ui/helloworld.h"
namespace keels{

TextSpan::TextSpan(const std::u16string& text): text_(text) {
}

void TextSpan::build(fml::RefPtr<flutter::ParagraphBuilder> builder) {
    builder->addText2(text_);
}




}
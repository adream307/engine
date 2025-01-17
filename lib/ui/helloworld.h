#ifndef KEELS_LIB_UI_HELLO_WORLD_H_
#define KEELS_LIB_UI_HELLO_WORLD_H_

#include <string>
#include "flutter/lib/ui/text.h"
#include "flutter/lib/ui/platform_dispatcher.h"
#include "flutter/lib/ui/text/paragraph_builder.h"
#include "flutter/lib/ui/painting/picture_recorder.h"
#include "flutter/lib/ui/painting/canvas.h"
// #include "flutter/lib/ui/floating_point.h"
#include "flutter/lib/ui/compositing/scene_builder.h"
#include "flutter/lib/ui/geometry.h"
#include "flutter/lib/ui/painting.h"

namespace keels
{

const inline double kDefaultFontSize = 14.0;

class InlineSpan {
public:
    InlineSpan() = default;
    ~InlineSpan() = default;
    virtual void build(fml::RefPtr<flutter::ParagraphBuilder> builder) = 0; 
};

class TextSpan : public InlineSpan{
public:
    TextSpan(const std::u16string& text);
    std::u16string& text() {return  text_;}
    void build(fml::RefPtr<flutter::ParagraphBuilder> builder) override;

private:
    std::u16string text_;
};

class TextScaler{
public:
    static const TextScaler LinearTextScaler(double factor){return TextScaler(factor);}
    TextScaler(double factor):textScaleFactor_(factor){} 
    double scale(double fontSize) const { return fontSize*textScaleFactor_;}
private:
    double textScaleFactor_;
};

class TextStyle{
public:
    ParagraphStyle getParagraphStyle(const TextAlign &textAlign,
                                     const TextDirection &textDirection,
                                     const TextScaler &textScaler);
};

class TextPainter {
public:
    TextPainter();
private:
    ParagraphStyle _createParagraphStyle(const std::optional<TextAlign> &textAlignOverride=std::nullopt);
    fml::RefPtr<flutter::ParagraphBuilder> _createParagraph(InlineSpan &text);
private:
    TextAlign textAlign_;
    TextDirection textDirection_;
    TextScaler textScaler_;
};


}



#endif

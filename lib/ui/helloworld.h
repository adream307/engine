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
#include <limits>

namespace keels
{

const inline double kDefaultFontSize = 14.0;

class InlineSpan {
public:
    InlineSpan() = default;
    virtual ~InlineSpan() = default;
    virtual void build(fml::RefPtr<flutter::ParagraphBuilder> builder) = 0; 
    virtual const std::u16string& toPlainText() const = 0;
};

class TextSpan : public InlineSpan{
public:
    TextSpan(const std::u16string& text);
    std::u16string& text() {return  text_;}
    void build(fml::RefPtr<flutter::ParagraphBuilder> builder) override;
    const std::u16string& toPlainText() const override {return text_;}

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
    TextPainter(std::shared_ptr<InlineSpan> &text, TextDirection textDirection);
    void layout(double minWidth, double maxWidth);
    const std::u16string& plainText() const {return text_->toPlainText();} 
private:
    ParagraphStyle _createParagraphStyle(const std::optional<TextAlign> &textAlignOverride=std::nullopt);
    fml::RefPtr<flutter::Paragraph> _createParagraph(std::shared_ptr<InlineSpan> &text);
    double _computePaintOffsetFraction(TextAlign textAlign, TextDirection textDirection);
private:
    TextAlign textAlign_;
    TextDirection textDirection_;
    TextScaler textScaler_;
    std::shared_ptr<InlineSpan> text_;
    bool rebuildParagraphForPaint_;
};


class BoxConstraints{
public:
    BoxConstraints(double minW=0,
                   double maxW=std::numeric_limits<double>::infinity(),
                   double minH=0,
                   double maxH=std::numeric_limits<double>::infinity())
                   :minWidth(minW),maxWidth(maxW),minHeight(minH),maxHeight(maxH){}
    double minWidth;
    double maxWidth;
    double minHeight;
    double maxHeight;
};

class RenderObject {
public:
    RenderObject()=default;
    virtual ~RenderObject()=default;
    void layout();
    virtual void performLayout() = 0;
    void _layoutWithoutResize() {performLayout();}
protected:
    BoxConstraints constraints_;
};

class RenderParagraph : public RenderObject{
public:
    RenderParagraph(std::shared_ptr<InlineSpan> &text, TextDirection textDirection);
    void performLayout() override;

private:
    void _layoutTextWithConstraints(BoxConstraints constraints);
    TextPainter textPainter_;
};

class RenderPositionedBox : public RenderObject{
public:
    RenderPositionedBox(std::shared_ptr<RenderObject> child):child_(child){}
    void performLayout() override;
private:
    std::shared_ptr<RenderObject> child_;
};

class RenderView : public RenderObject{
public:
    RenderView()=default;
    void performLayout() override;
private:
    std::shared_ptr<RenderObject> child_;
};

class PipelineOwner {
public:
    void flushLayout();
private:
    std::list<std::shared_ptr<RenderObject>> nodesNeedingLayout_;
};

void Helloworld();

}



#endif

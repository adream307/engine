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
class PipelineOwner;

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

class ViewConfiguration{
public:
    BoxConstraints physicalConstraints;
    BoxConstraints logicalConstraints;
    double devicePixelRatio;
};

class RenderObject : public std::enable_shared_from_this<RenderObject>{
public:
    RenderObject()=default;
    virtual ~RenderObject()=default;
    void layout() {performLayout();}
    virtual void performLayout() = 0;
    void _layoutWithoutResize() {performLayout();}
    void attach(std::shared_ptr<PipelineOwner> &owner) {owner_=owner;}
    void setChild(std::shared_ptr<RenderObject> child) {child_ = child;}
    std::shared_ptr<RenderObject>& child() {return child_;}
    void scheduleInitialLayout();
    void scheduleInitialPaint(); //TODO flutter/packages/flutter/lib/src/rendering/object.dart, RenderObject.void scheduleInitialPaint(ContainerLayer rootLayer)
protected:
    BoxConstraints constraints_;
    std::shared_ptr<PipelineOwner> owner_;
    std::shared_ptr<RenderObject> child_;
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
    RenderPositionedBox(std::shared_ptr<RenderObject> child);
    void performLayout() override;
};

class RenderView : public RenderObject{
public:
    RenderView(std::shared_ptr<FlutterView> &view);
    void performLayout() override;
    std::shared_ptr<FlutterView>& flutterView() {return view_;}
    void prepareInitialFrame();
    ViewConfiguration configuration;
private:
    std::shared_ptr<FlutterView> view_;
};

class PipelineOwner: public std::enable_shared_from_this<PipelineOwner> {
public:
    PipelineOwner()=default;
    ~PipelineOwner()=default;
    void flushLayout();
    std::shared_ptr<RenderObject>& rootNode() {return rootNode_;}
    void setRootNode(std::shared_ptr<RenderObject> node);
    std::list<std::shared_ptr<RenderObject>> nodesNeedingLayout;
    std::list<std::shared_ptr<RenderObject>> nodesNeedingPaint;
private:
    std::shared_ptr<RenderObject> rootNode_; //render view
};


class ViewRenderingFlutterBinding: public std::enable_shared_from_this<ViewRenderingFlutterBinding> {
public:
    ViewRenderingFlutterBinding(std::shared_ptr<RenderObject> root);
    std::shared_ptr<PipelineOwner> createRootPipelineOwner();
    std::shared_ptr<RenderView> initRenderView(std::shared_ptr<FlutterView>& view);
    ViewConfiguration createViewConfigurationFor(std::shared_ptr<RenderView> &view);
    void addRenderView(std::shared_ptr<RenderView> &);
    void scheduleFrame();
    void ensureFrameCallbacksRegistered();
    void handleDrawFrame();
private:
    std::shared_ptr<RenderObject> root_;
    std::shared_ptr<PipelineOwner> rootPipelineOwner_;
    std::unordered_map<int64_t, std::shared_ptr<RenderView>> viewIdToRenderView_;
};

void Helloworld();

}



#endif

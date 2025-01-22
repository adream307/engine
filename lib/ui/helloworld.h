#ifndef KEELS_LIB_UI_HELLO_WORLD_H_
#define KEELS_LIB_UI_HELLO_WORLD_H_

#include <string>
#include "flutter/lib/ui/text.h"
#include "flutter/lib/ui/platform_dispatcher.h"
#include "flutter/lib/ui/text/paragraph_builder.h"
#include "flutter/lib/ui/painting/picture_recorder.h"
#include "flutter/lib/ui/painting/canvas.h"
#include "flutter/lib/ui/compositing/scene_builder.h"
#include "flutter/lib/ui/geometry.h"
#include <limits>

namespace keels
{

const inline double kDefaultFontSize = 14.0;
class PipelineOwner;
class RenderObject;

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

struct TextLayout{
    TextLayout(const fml::RefPtr<flutter::Paragraph> &p,
               const TextDirection &w,
               const std::u16string &r): paragraph(p),writingDirection(w),rawString(r) {}
    double maxIntrinsicLineExtent() const {return paragraph->maxIntrinsicWidth();}
    double _contentWidthFor(double minWidth, double maxWidth);

    fml::RefPtr<flutter::Paragraph> paragraph;
    TextDirection writingDirection;
    std::u16string rawString;
};

struct TextPainterLayoutCacheWithOffset{
    TextPainterLayoutCacheWithOffset(const TextLayout&tl, double ta, double lm, double co):
        layout(tl), textAlignment(ta),layoutMaxWidth(lm), contentWidth(co){}
    Offset paintOffset() {return Offset(0.0,0.0);}
    TextLayout layout;
    double textAlignment;
    double layoutMaxWidth;
    double contentWidth;
};

class TextPainter {
public:
    TextPainter(std::shared_ptr<InlineSpan> &text, TextDirection textDirection);
    void layout(double minWidth, double maxWidth);
    const std::u16string& plainText() const {return text_->toPlainText();} 
    void paint(fml::RefPtr<flutter::Canvas> canvas, Offset & offset);
    double width() const {return layoutCache_->contentWidth;}
    double height() const {return layoutCache_->layout.paragraph->height();}
    Size size() const;
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
    std::shared_ptr<TextPainterLayoutCacheWithOffset> layoutCache_;
};


class PaintingContext {
public:
    static void repaintCompositedChild(std::shared_ptr<RenderObject> &child);
    void paintChild(std::shared_ptr<RenderObject>& child, Offset &offset);
    fml::RefPtr<flutter::Canvas> getCanvas();
private:
    fml::RefPtr<flutter::Canvas> canvas_ = nullptr;
    fml::RefPtr<flutter::PictureRecorder> recorder_ = nullptr;
    fml::RefPtr<flutter::Picture> picture_ = nullptr; 
};

class BoxConstraints{
public:
    BoxConstraints(double minW=0,
                   double maxW=std::numeric_limits<double>::infinity(),
                   double minH=0,
                   double maxH=std::numeric_limits<double>::infinity())
                   :minWidth(minW),maxWidth(maxW),minHeight(minH),maxHeight(maxH){}
    BoxConstraints loosen() {return BoxConstraints(0,maxWidth,0,maxHeight);}
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

class TransformLayer{
public:
    TransformLayer()=default;
    TransformLayer(const SkM44 &t):transform(t){}
    SkM44 transform;
    std::shared_ptr<RenderObject> owner;
};

class RenderObject : public std::enable_shared_from_this<RenderObject>{
public:
    RenderObject()=default;
    virtual ~RenderObject()=default;
    void layout(const BoxConstraints &constraints);
    virtual void performLayout() = 0;
    void _layoutWithoutResize() {performLayout();}
    void attach(std::shared_ptr<PipelineOwner> &owner) {owner_=owner;}
    void setChild(std::shared_ptr<RenderObject> child) {child_ = child;}
    std::shared_ptr<RenderObject>& child() {return child_;}
    Size size() const {return size_;}
    virtual void paint(PaintingContext &context, Offset &offset){context.paintChild(child_,offset);}
    void scheduleInitialLayout();
    void scheduleInitialPaint(TransformLayer &layer);
protected:
    BoxConstraints constraints_;
    std::shared_ptr<PipelineOwner> owner_;
    std::shared_ptr<RenderObject> child_;
    Size size_;
    TransformLayer rootLayer_;
};

class RenderParagraph : public RenderObject{
public:
    RenderParagraph(std::shared_ptr<InlineSpan> &text, TextDirection textDirection);
    void performLayout() override;
    void paint(PaintingContext &context, Offset &offset) override;
private:
    void _layoutTextWithConstraints(BoxConstraints constraints);
    TextPainter textPainter_;
};

class RenderPositionedBox : public RenderObject{
public:
    RenderPositionedBox(std::shared_ptr<RenderObject> child);
    void performLayout() override;
    void paint(PaintingContext &context, Offset &offset) override;
};

class RenderView : public RenderObject{
public:
    RenderView(std::shared_ptr<FlutterView> &view);
    void performLayout() override;
    std::shared_ptr<FlutterView>& flutterView() {return view_;}
    void prepareInitialFrame();
    ViewConfiguration configuration;
    void paint(PaintingContext &context, Offset &offset) override;
    void compositeFrame();
private:
    TransformLayer _updateMatricesAndCreateNewRootLayer();
private:
    SkM44 rootTransform_;
    std::shared_ptr<FlutterView> view_;
};

class PipelineOwner: public std::enable_shared_from_this<PipelineOwner> {
public:
    PipelineOwner()=default;
    ~PipelineOwner()=default;
    void flushLayout();
    void flushPaint();
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

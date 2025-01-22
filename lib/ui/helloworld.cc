#include "flutter/lib/ui/helloworld.h"
#include "flutter/fml/make_copyable.h"
#include "flutter/lib/ui/floating_point.h"
#include <iostream>
#include <thread>
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


double TextLayout::_contentWidthFor(double minWidth, double maxWidth) {
    double x =maxIntrinsicLineExtent();
    if(x<minWidth) return minWidth;
    if(x>maxWidth) return maxWidth;
    return x;
}

TextPainter::TextPainter(std::shared_ptr<InlineSpan>& text, TextDirection textDirection):
    textAlign_(TextAlign::start),
    textDirection_(textDirection),
    textScaler_(TextScaler::LinearTextScaler(1.0)),
    text_(text)
{
}

void TextPainter::layout(double minWidth, double maxWidth)
{
    double layoutMaxWidth = maxWidth;
    auto paragraph = _createParagraph(text_);
    paragraph->layout(layoutMaxWidth);

    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ", TextPainter::layout, width = " << layoutMaxWidth << std::endl;
    
    TextLayout layout(paragraph, textDirection_, plainText());
    double contentWidth = layout._contentWidthFor(minWidth, maxWidth);
    layoutCache_ = std::make_shared<TextPainterLayoutCacheWithOffset>(layout,0.0,layoutMaxWidth,contentWidth);

    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ", TextPainter::layout content width = " << contentWidth << std::endl;

    //TODO: flutter/packages/flutter/lib/src/painting/text_painter.dart: 1192
}

double TextPainter::_computePaintOffsetFraction(TextAlign textAlign, TextDirection textDirection)
{
    if(textAlign==TextAlign::left) return 0.0;
    if(textAlign==TextAlign::right) return 1.0;
    if(textAlign==TextAlign::center) return 0.5;
    if(textAlign==TextAlign::start || textAlign==TextAlign::justify) {
        if (textDirection==TextDirection::ltr) return 0.0;
        else if (textDirection==TextDirection::rtl) return 1.0;
    }
    if(textAlign==TextAlign::end) {
        if (textDirection==TextDirection::ltr) return 1.0;
        else if (textDirection==TextDirection::rtl) return 0.0;
    }
    return 0.0;
}

Size TextPainter::size() const {
    return Size(width(), height());
}

ParagraphStyle TextPainter::_createParagraphStyle(const std::optional<TextAlign> &textAlignOverride)
{
    TextStyle baseStyle;
    return baseStyle.getParagraphStyle(
        textAlignOverride.has_value() ? textAlignOverride.value() : textAlign_,
        textDirection_,
        textScaler_
    );
}

fml::RefPtr<flutter::Paragraph> TextPainter::_createParagraph(std::shared_ptr<InlineSpan>& text)
{
    auto style = _createParagraphStyle();
    auto builder = fml::MakeRefCounted<flutter::ParagraphBuilder>(style.encoded());
    text->build(builder);
    rebuildParagraphForPaint_=false;
    return builder->build2();
}

void TextPainter::paint(fml::RefPtr<flutter::Canvas> canvas, Offset & offset) {
    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ", TextPainter::paint::offset = " << offset.dx  << "-" << offset.dy << std::endl;
    layoutCache_->layout.paragraph->paint(canvas.get(), offset.dx, offset.dy);
}


void PaintingContext::repaintCompositedChild(std::shared_ptr<RenderObject> &child) {
    auto childContex = PaintingContext{};
    Offset zero(0.0,0.0);
    child->paint(childContex, zero);

}

void PaintingContext::paintChild(std::shared_ptr<RenderObject>& child, Offset &offset) {
    child->paint(*this, offset);
    picture_ = recorder_->endRecording2();
}

fml::RefPtr<flutter::Canvas> PaintingContext::getCanvas() {
    if(canvas_) {
        return canvas_;
    }
    recorder_ = fml::MakeRefCounted<flutter::PictureRecorder>();
    auto rect = Rect::largest();

    canvas_ = fml::MakeRefCounted<flutter::Canvas>(recorder_->BeginRecording(
                 SkRect::MakeLTRB(flutter::SafeNarrow(rect.left), 
                                  flutter::SafeNarrow(rect.top), 
                                  flutter::SafeNarrow(rect.right),
                                  flutter::SafeNarrow(rect.bottom))));
    recorder_->set_canvas(canvas_);
    return canvas_;
}

fml::RefPtr<flutter::Scene> TransformLayer::buildScene(fml::RefPtr<flutter::SceneBuilder> builder){
    return nullptr;
}

void RenderObject::scheduleInitialLayout() {
    owner_->nodesNeedingLayout.push_back(shared_from_this());
}

void RenderObject::scheduleInitialPaint(TransformLayer &layer) {
    rootLayer_ = layer;
    owner_->nodesNeedingPaint.push_back(shared_from_this());
}

void RenderObject::layout(const BoxConstraints &constraints) {
    constraints_ = constraints;
    performLayout();
}

RenderParagraph::RenderParagraph(std::shared_ptr<InlineSpan> &text, TextDirection textDirection)
    :textPainter_(text,textDirection) {
}

void RenderParagraph::performLayout() {
    _layoutTextWithConstraints(constraints_.loosen());
    size_ = textPainter_.size();
    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ", RenderParagraph::performLayout::size = " << size_.width << "-" << size_.height << std::endl;

}

void RenderParagraph::_layoutTextWithConstraints(BoxConstraints constraints) {
    textPainter_.layout(constraints.minWidth, constraints.maxWidth);
}

void RenderParagraph::paint(PaintingContext &context, Offset &offset) {
    auto canvas = context.getCanvas();
    textPainter_.paint(canvas, offset);
}

RenderPositionedBox::RenderPositionedBox(std::shared_ptr<RenderObject> child) {
    setChild(child);
}

void RenderPositionedBox::performLayout() {
    child_->layout(constraints_.loosen());
    size_ = Size(constraints_.maxWidth, constraints_.maxHeight);
    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ", RenderPositionedBox::performLayout::size = " << size_.width << "-" << size_.height << std::endl;
}

void RenderPositionedBox::paint(PaintingContext &context, Offset &offset)
{
    auto cs = child_->size();
    double dx = size_.width - cs.width;
    double dy = size_.height - cs.height;
    Offset o(dx/2.0+offset.dx,dy/2.0+offset.dy);
    context.paintChild(child_, o);
}

RenderView::RenderView(std::shared_ptr<FlutterView> &view):view_(view)
{
}

void RenderView::performLayout() {
    child_->layout(configuration.logicalConstraints);
}

void RenderView::prepareInitialFrame() {
    scheduleInitialLayout();
    auto layer = _updateMatricesAndCreateNewRootLayer();
    scheduleInitialPaint(layer);
}

TransformLayer RenderView::_updateMatricesAndCreateNewRootLayer() {
    rootTransform_ = SkM44(
            flutter::SafeNarrow(configuration.devicePixelRatio), flutter::SafeNarrow(0.0),                            flutter::SafeNarrow(0.0), flutter::SafeNarrow(0.0),
            flutter::SafeNarrow(0.0),                            flutter::SafeNarrow(configuration.devicePixelRatio), flutter::SafeNarrow(0.0), flutter::SafeNarrow(0.0),
            flutter::SafeNarrow(0.0),                            flutter::SafeNarrow(0.0),                            flutter::SafeNarrow(1.0), flutter::SafeNarrow(0.0),
            flutter::SafeNarrow(0.0),                            flutter::SafeNarrow(0.0),                            flutter::SafeNarrow(0.0), flutter::SafeNarrow(1.0)
    );
    auto rootLayer = TransformLayer(rootTransform_);
    rootLayer.owner = shared_from_this();
    return rootLayer;
}

void RenderView::paint(PaintingContext &context, Offset &offset)
{
    context.paintChild(child_,offset);
}

void RenderView::compositeFrame() {
    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ",RenderView::compositeFrame::SceneBuilder" << std::endl;
    fml::RefPtr<flutter::SceneBuilder> sceneBuilder = fml::MakeRefCounted<flutter::SceneBuilder>();
    auto scene = rootLayer_.buildScene(sceneBuilder);
}


void PipelineOwner::flushLayout() {
    for(auto & node : nodesNeedingLayout) {
        node->_layoutWithoutResize();
    }
}

void PipelineOwner::flushPaint() {
    for(auto & node : nodesNeedingPaint) {
        PaintingContext::repaintCompositedChild(node);
    }
}

void PipelineOwner::setRootNode(std::shared_ptr<RenderObject> node) {
    rootNode_ = node;
    std::shared_ptr<PipelineOwner> ptr = shared_from_this();
    node->attach(ptr);
}

ViewRenderingFlutterBinding::ViewRenderingFlutterBinding(std::shared_ptr<RenderObject> root): root_(root){
    rootPipelineOwner_=createRootPipelineOwner();
    auto flutterView = keels::PlatformDispatcher::instance().implicitView();
    auto renderView = initRenderView(flutterView);
    renderView->setChild(root);
    root_ = nullptr;
}

std::shared_ptr<PipelineOwner> ViewRenderingFlutterBinding::createRootPipelineOwner(){
    return std::make_shared<PipelineOwner>();
}

std::shared_ptr<RenderView> ViewRenderingFlutterBinding::initRenderView(std::shared_ptr<FlutterView>& view) {
    auto renderView = std::make_shared<RenderView>(view);
    rootPipelineOwner_->setRootNode(renderView);
    addRenderView(renderView);
    renderView->prepareInitialFrame();
    return renderView;
}

ViewConfiguration ViewRenderingFlutterBinding::createViewConfigurationFor(std::shared_ptr<RenderView> &view) {
    Size physicalSize = view->flutterView()->physicalSize();
    ViewConfiguration cfg;
    cfg.devicePixelRatio = view->flutterView()->devicePixelRatio();
    cfg.physicalConstraints.minWidth = physicalSize.width;
    cfg.physicalConstraints.maxWidth = physicalSize.width;
    cfg.physicalConstraints.minHeight = physicalSize.height;
    cfg.physicalConstraints.maxHeight = physicalSize.height;
    cfg.logicalConstraints.minWidth = physicalSize.width/cfg.devicePixelRatio;
    cfg.logicalConstraints.maxWidth = physicalSize.width/cfg.devicePixelRatio;
    cfg.logicalConstraints.minHeight = physicalSize.height/cfg.devicePixelRatio;
    cfg.logicalConstraints.maxHeight = physicalSize.height/cfg.devicePixelRatio;
    
    return cfg;
}

void ViewRenderingFlutterBinding::addRenderView(std::shared_ptr<RenderView> &view) {
    int64_t id = view->flutterView()->viewId();
    view->configuration = createViewConfigurationFor(view);
    viewIdToRenderView_[id]=view;
}

void ViewRenderingFlutterBinding::scheduleFrame() {
    ensureFrameCallbacksRegistered();
}

void ViewRenderingFlutterBinding::ensureFrameCallbacksRegistered() {
    std::shared_ptr<ViewRenderingFlutterBinding> ptr = shared_from_this();
    auto draw_frame = fml::MakeCopyable([ptr](){
        ptr->handleDrawFrame();
    });
    PlatformDispatcher::instance().SetOnDrawFrame(draw_frame);
}

void ViewRenderingFlutterBinding::handleDrawFrame() {
    rootPipelineOwner_->flushLayout();
    rootPipelineOwner_->flushPaint();
    for(auto & view : viewIdToRenderView_) {
        view.second->compositeFrame();
    }
}

void Helloworld() {
    std::cout << __FILE__ << ":" << __LINE__ << ":" << std::this_thread::get_id() << ",====================== run Helloworld ======================" << std::endl;
    std::shared_ptr<InlineSpan> text = std::make_shared<TextSpan>(u"hello world");
    std::shared_ptr<RenderParagraph> renderParagraph = std::make_shared<RenderParagraph>(text, TextDirection::ltr);
    std::shared_ptr<RenderPositionedBox> renderPositionedBox = std::make_shared<RenderPositionedBox>(renderParagraph);
    std::shared_ptr<ViewRenderingFlutterBinding> view = std::make_shared<ViewRenderingFlutterBinding>(renderPositionedBox);
    view->scheduleFrame();
}

}
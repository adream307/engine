#include "flutter/lib/ui/helloworld.h"
#include "flutter/fml/make_copyable.h"
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

void RenderObject::scheduleInitialLayout() {
    owner_->nodesNeedingLayout.push_back(shared_from_this());
}

void RenderObject::scheduleInitialPaint() {
    owner_->nodesNeedingPaint.push_back(shared_from_this());
}

RenderParagraph::RenderParagraph(std::shared_ptr<InlineSpan> &text, TextDirection textDirection)
    :textPainter_(text,textDirection) {
}

void RenderParagraph::performLayout() {
    _layoutTextWithConstraints(constraints_);
}

void RenderParagraph::_layoutTextWithConstraints(BoxConstraints constraints) {
    textPainter_.layout(constraints.minWidth, constraints.maxWidth);
}

RenderPositionedBox::RenderPositionedBox(std::shared_ptr<RenderObject> child) {
    setChild(child);
}

void RenderPositionedBox::performLayout() {
    child_->performLayout();
}

RenderView::RenderView(std::shared_ptr<FlutterView> &view):view_(view)
{
}

void RenderView::performLayout() {
    child_->performLayout();
}

void RenderView::prepareInitialFrame() {
    scheduleInitialLayout();
    scheduleInitialPaint();
} 

void PipelineOwner::flushLayout() {
    for(auto & node : nodesNeedingLayout) {
        node->_layoutWithoutResize();
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

void ViewRenderingFlutterBinding::ensureFrameCallbacksRegistered() {
    std::shared_ptr<ViewRenderingFlutterBinding> ptr = shared_from_this();
    auto draw_frame = fml::MakeCopyable([ptr](){
        ptr->handleDrawFrame();
    });
    PlatformDispatcher::instance().SetOnDrawFrame(draw_frame);
}

void ViewRenderingFlutterBinding::handleDrawFrame() {
    rootPipelineOwner_->flushLayout();
}

void Helloworld() {
    std::shared_ptr<InlineSpan> text = std::make_shared<TextSpan>(u"hello world");
    std::shared_ptr<RenderParagraph> renderParagraph = std::make_shared<RenderParagraph>(text, TextDirection::ltr);
    std::shared_ptr<RenderPositionedBox> renderPositionedBox = std::make_shared<RenderPositionedBox>(renderParagraph);
    std::shared_ptr<ViewRenderingFlutterBinding> view = std::make_shared<ViewRenderingFlutterBinding>(renderPositionedBox);
}

}
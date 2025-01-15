#ifndef KEELS_LIB_UI_TEXT_H_
#define KEELS_LIB_UI_TEXT_H_

#include<vector>
#include<optional>

namespace keels{

enum class TextDirection:int32_t {
  rtl,
  ltr
};

enum class TextLeadingDistribution:int32_t {
  proportional,
  even
};

class ParagraphStyle {
public:
    ParagraphStyle(const std::optional<TextDirection>& textDirection);
    const std::vector<int32_t>& encoded() const { return encoded_;}
    TextLeadingDistribution leadingDistribution() const { return leadingDistribution_;};

private:
    std::vector<int32_t> encodeParagraphStyle(const std::optional<TextDirection>& textDirection);

private:
    std::vector<int32_t> encoded_;
    TextLeadingDistribution leadingDistribution_;
};

// dart:ParagraphBuilder
class NativeParagraphBuilder {
public:
    NativeParagraphBuilder(const ParagraphStyle &style);
private:
    TextLeadingDistribution defaultLeadingDistribution_;
    int placeholderCount_ = 0;
    std::vector<double> placeholderScales_ = std::vector<double>{};
};

}

#endif
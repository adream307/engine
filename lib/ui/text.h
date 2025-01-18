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

enum class TextAlign {
  left,
  right,
  center,
  justify,
  start,
  end
};

class ParagraphConstraints {
public:
  ParagraphConstraints(double w):width(w){}
  double width;
};

class ParagraphStyle {
public:
    ParagraphStyle(const std::optional<TextAlign>& textAlign,
                   const std::optional<TextDirection>& textDirection,
                   const std::optional<double>& fontSize);

    const std::vector<int32_t>& encoded() const { return encoded_;}
    TextLeadingDistribution leadingDistribution() const { return leadingDistribution_;}

private:
    std::vector<int32_t> encodeParagraphStyle(const std::optional<TextAlign>& textAlign,
                                              const std::optional<TextDirection>& textDirection,
                                              const std::optional<double>& fontSize);

private:
    std::vector<int32_t> encoded_;
    std::optional<double> fontSize_;
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
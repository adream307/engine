#ifndef KEELS_LIB_UI_TEXT_H_
#define KEELS_LIB_UI_TEXT_H_

#include<vector>
#include<optional>

namespace keels{

enum class TextDirection:int32_t {
  rtl, // The text flows from right to left (e.g. Arabic, Hebrew).
  ltr  // The text flows from left to right (e.g., English, French).
};

enum class TextLeadingDistribution:int32_t {
  /// Distributes the [leading](https://en.wikipedia.org/wiki/Leading)
  /// of the text proportionally above and below the text, to the font's
  /// ascent/descent ratio.
  ///
  /// {@template dart.ui.leading}
  /// The leading of a text run is defined as
  /// `TextStyle.height * TextStyle.fontSize - TextStyle.fontSize`. When
  /// [TextStyle.height] is not set, the text run uses the leading specified by
  /// the font instead.
  /// {@endtemplate}
  proportional,

  /// Distributes the ["leading"](https://en.wikipedia.org/wiki/Leading)
  /// of the text evenly above and below the text (i.e. evenly above the
  /// font's ascender and below the descender).
  ///
  /// {@macro dart.ui.leading}
  ///
  /// The leading can become negative when [TextStyle.height] is smaller than
  /// 1.0.
  ///
  /// This is the default strategy used by CSS, known as
  /// ["half-leading"](https://www.w3.org/TR/css-inline-3/#half-leading).
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
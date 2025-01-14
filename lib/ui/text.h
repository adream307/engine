#ifndef KEELS_LIB_UI_TEXT_H_
#define KEELS_LIB_UI_TEXT_H_

#include<vector>
#include<optional>

namespace keels{

enum class TextDirection {
  rtl, // The text flows from right to left (e.g. Arabic, Hebrew).
  ltr  // The text flows from left to right (e.g., English, French).
};

class ParagraphStyle {
public:
    ParagraphStyle(const std::optional<TextDirection>& textDirection);

private:
    std::vector<int32_t> encodeParagraphStyle(const std::optional<TextDirection>& textDirection);

private:
    std::vector<int32_t > encoded_;
};

}

#endif
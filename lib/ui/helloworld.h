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

class TextSpan{
public:
    TextSpan(const std::u16string& text);
    std::u16string& text() {return  text_;}
    void build(fml::RefPtr<flutter::ParagraphBuilder> builder);

private:
    std::u16string text_;

};


}



#endif

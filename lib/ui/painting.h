#ifndef KEELS_LIB_UI_PAINTING_H_
#define KEELS_LIB_UI_PAINTING_H_

#include <cstdint>

namespace keels{

enum class Clip :int32_t {
  none,
  hardEdge,
  antiAlias,
  antiAliasWithSaveLayer
};

}

#endif


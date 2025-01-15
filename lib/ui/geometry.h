#ifndef KEELS_LIB_UI_GEOMETRY_H_
#define KEELS_LIB_UI_GEOMETRY_H_

namespace keels{

struct Size{
    Size(double w, double h): width(w), height(h){}
    double width;
    double height;
};

struct Rect{
    Rect(double l, double t, double r, double b):left(l),top(t),right(r),bottom(b){}
    double width() const {return right-left;}
    double height() const {return bottom-top;}
    Size size() const {return Size(width(), height());}

    double left;
    double top;
    double right;
    double bottom;
};

struct Offset{
    Offset(double x, double y):dx(x),dy(y){}
    double dx;
    double dy;
};

};

#endif
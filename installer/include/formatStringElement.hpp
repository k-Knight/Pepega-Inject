#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifndef PEPEGA_FSTRELEM
#define PEPEGA_FSTRELEM

struct formatStringElement {
    const char *text;
    int fontSize;
    wxFontFamily fontFamily;
    wxFontStyle fontStyle;
    wxFontWeight fontWeight;
    wxColor color;
    int height;
};

#endif
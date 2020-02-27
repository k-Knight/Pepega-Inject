#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <map>

enum class colors {
    frame,
    background,
    content,
    hover_red,
    hover_light,
    main_text,
    secondary_text,
    text_input,
    button,
    button_hover,
    progress_pending,
    progress
};

const std::map<colors, const wxColour> colorDef = {
    { colors::frame,            wxColour(  0,  71,  76) },
    { colors::background,       wxColour( 32,  34,  37) },
    { colors::content,          wxColour( 53,  57,  63) },
    { colors::hover_red,        wxColour(240,  71,  71) },
    { colors::hover_light,      wxColour( 42,  44,  48) },
    { colors::main_text,        wxColour(223, 255, 247) },
    { colors::secondary_text,   wxColour(134, 154, 149) },
    { colors::text_input,       wxColour( 64,  68,  75) },
    { colors::button,           wxColour(185, 187, 190) },
    { colors::button_hover,     wxColour(152, 218, 196) },
    { colors::progress_pending, wxColour( 18,  51,  46) },
    { colors::progress,         wxColour( 72, 205, 183) }
};
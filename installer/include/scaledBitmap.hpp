#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class scaledBitmap : public wxBitmap {
private:
    static wxImage scaleImage(wxImage &&image, size_t width, size_t height);
    scaledBitmap(wxImage &image, size_t width, size_t height);
public:
    scaledBitmap(const char *path, size_t width, size_t height);
    static scaledBitmap fromMemory(const void *data, size_t length, size_t width, size_t height);
};
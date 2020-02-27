#include "../include/scaledBitmap.hpp"

#include <algorithm>
#include <cmath>
#include <wx/mstream.h>
#include <utility>

using namespace std;

scaledBitmap::scaledBitmap(const char *path, size_t width, size_t height)
    : wxBitmap(scaleImage(wxImage(path, wxBITMAP_TYPE_ANY), width, height)) { }

scaledBitmap::scaledBitmap(wxImage &image, size_t width, size_t height)
    : wxBitmap(scaleImage(std::forward<wxImage&&>(image), width, height)) { }

wxImage scaledBitmap::scaleImage(wxImage &&image, size_t width, size_t height) {
    double ratio = min(
        double(width) / double(image.GetWidth()),
        double(height) / double(image.GetHeight())
    );

    return image.Scale(
        round(double(image.GetWidth()) * ratio),
        round(double(image.GetHeight()) * ratio),
        wxIMAGE_QUALITY_HIGH
    );
}

scaledBitmap scaledBitmap::fromMemory(const void *data, size_t length, size_t width, size_t height) {
    wxMemoryInputStream memStream(data, length);
    wxImage image(memStream, wxBITMAP_TYPE_ANY);

    return scaledBitmap(image, width, height);
}
#include "http/network/protocol/http.hpp"


namespace http
{


mime_type::value_type determine_content_type(const std::string_view& file_path) {
    if (file_path.ends_with("html") || file_path.ends_with("htm")) {
        return mime_type::MT_HTML;
    }
    else if (file_path.ends_with("css")) {
        return mime_type::MT_CSS;
    }
    else if (file_path.ends_with("gif")) {
        return mime_type::MT_GIF;
    }
    else if (file_path.ends_with("png")) {
        return mime_type::MT_PNG;
    }
    else if (file_path.ends_with("jpg") || file_path.ends_with("jpe") || file_path.ends_with("jpeg")) {
        return mime_type::MT_JPG;
    }
    else if (file_path.ends_with("tiff") || file_path.ends_with("tif")) {
        return mime_type::MT_TIFF;
    }
    else if (file_path.ends_with("bmp")) {
        return mime_type::MT_BMP;
    }
    else if (file_path.ends_with("js")) {
        return mime_type::MT_JS;
    }
    else if (file_path.ends_with("swf")) {
        return mime_type::MT_SWF;
    }
    return mime_type::MT_PLAIN;
}


} // namespace server
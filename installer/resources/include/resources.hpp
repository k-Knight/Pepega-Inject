#ifndef RESOURCES_HEADER
#define RESOURCES_HEADER

struct MemResource {
    const unsigned char *data;
    const unsigned long long size;
    const char *path;
};

namespace Resources {
    namespace display {
        enum names {
            background = 0,
            close = 1,
            custom_icon = 2,
            logo = 3,
            minimize = 4,
            RobotoSlab = 5,
            separator = 6
        };

        const long count = 7;
        extern const MemResource resources[];
    }

    namespace injector {
        enum names {
            index = 0,
            css_loader = 1,
            filework = 2,
            injector = 3,
            logger = 4,
            pepega_loader = 5,
            request = 6,
            settings = 7,
            style = 8,
            content_display_formatter = 9,
            content_formatter = 10,
            context_menu_manager = 11,
            download_formatter = 12,
            message_formatter = 13,
            video_formatter = 14,
            async_execute = 15,
            clipboard_operations = 16,
            downloader = 17,
            observer_array = 18,
            open_window = 19
        };

        const long count = 21;
        extern const MemResource resources[];
    }
}

#endif
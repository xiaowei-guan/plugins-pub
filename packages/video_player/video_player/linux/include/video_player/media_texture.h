#ifndef FLUTTER_PLUGIN_VIDEO_TEXTURE_H_
#define FLUTTER_PLUGIN_VIDEO_TEXTURE_H_
#include <glib-object.h>
#include <flutter_linux/flutter_linux.h>

G_DECLARE_FINAL_TYPE(MediaTexture,
                     fl_my_texture_gl,
                     FL,
                     MY_TEXTURE_GL,
                     FlTextureGL)

struct _MediaTexture
{
    FlPixelBufferTexture parent_instance;
    uint8_t *buffer;
    uint32_t name;
    uint32_t width;
    uint32_t height;
};


FlMyTextureGL *fl_my_texture_gl_new(uint32_t target,
                                    uint32_t name,
                                    uint32_t width,
                                    uint32_t height);
#endif // FLUTTER_PLUGIN_VIDEO_PLAYER_TEXTURE_H_
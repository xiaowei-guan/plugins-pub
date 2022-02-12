#ifndef FLUTTER_PLUGIN_VIDEO_PLAYER_TEXTURE_H_
#define FLUTTER_PLUGIN_VIDEO_PLAYER_TEXTURE_H_
#include <flutter_linux/flutter_linux.h>
#include <glib-object.h>

G_DECLARE_FINAL_TYPE(VideoPlayerTexture, video_player_texture, VIDEO_PLAYER,
                     TEXTURE, FlPixelBufferTexture)

struct _VideoPlayerTexture {
  FlPixelBufferTexture parent_instance;
  uint8_t *buffer;
  uint32_t width;
  uint32_t height;
};

VideoPlayerTexture *video_player_texture_new(uint8_t *buffer, uint32_t width,
                                             uint32_t height);
#endif  // FLUTTER_PLUGIN_VIDEO_PLAYER_TEXTURE_H_
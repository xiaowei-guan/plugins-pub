#include "include/video_player/video_player_texture.h"

G_DEFINE_TYPE(VideoPlayerTexture, video_player_texture,
              fl_pixel_buffer_texture_get_type())

static gboolean video_player_texture_copy_pixels(FlPixelBufferTexture *texture,
                                                 const uint8_t **out_buffer,
                                                 uint32_t *width,
                                                 uint32_t *height,
                                                 GError **error) {
  *out_buffer = VIDEO_PLAYER_TEXTURE(texture)->buffer;
  *height = VIDEO_PLAYER_TEXTURE(texture)->width;
  *width = VIDEO_PLAYER_TEXTURE(texture)->height;
  return true;
}

static void video_player_texture_dispose(GObject *object) {
  VideoPlayerTexture *self = VIDEO_PLAYER_TEXTURE(object);
  if (self->buffer) {
    delete self->buffer;
    self->buffer = NULL;
  }
}

static void video_player_texture_class_init(VideoPlayerTextureClass *klass) {
  FL_PIXEL_BUFFER_TEXTURE_CLASS(klass)->copy_pixels =
      video_player_texture_copy_pixels;
  G_OBJECT_CLASS(klass)->dispose = video_player_texture_dispose;
}

VideoPlayerTexture *video_player_texture_new(uint32_t width, uint32_t height) {
  auto r = VIDEO_PLAYER_TEXTURE(
      g_object_new(video_player_texture_get_type(), nullptr));
  r->width = width;
  r->height = height;
  r->buffer = new uint8_t[width * height * 4];
  return r;
}

static void video_player_texture_init(VideoPlayerTexture *self) {
  self->buffer = nullptr;
  self->width = 0;
  self->height = 0;
}

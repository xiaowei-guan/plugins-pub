#include "include/video_player/video_player.h"

#include <vlc/vlc.h>

#include "include/video_player/video_player_texture.h"

#define VIDEO_PLAYER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), video_player_get_type(), VideoPlayer))

struct _VideoPlayer {
  GObject parent_instance;
  libvlc_instance_t* instance;
  libvlc_media_t* media;
  libvlc_media_player_t* meida_player;
  unsigned video_width = 0;
  unsigned video_height = 0;
  FlTextureRegistrar* texture_registrar;
  VideoPlayerTexture* video_player_texture;
  int64_t texture_id = 0;
};

G_DEFINE_TYPE(VideoPlayer, video_player, g_object_get_type())

static void video_player_dispose(GObject* object) {
  VideoPlayer* self = VIDEO_PLAYER(object);
  if (self->meida_player) {
    libvlc_media_player_stop(self->meida_player);
    libvlc_media_player_release(self->meida_player);
    self->meida_player = nullptr;
  }

  if (self->media) {
    libvlc_media_release(self->media);
    self->media = nullptr;
  }

  if (self->instance) {
    libvlc_release(self->instance);
    self->instance = nullptr;
  }

  if (self->video_player_texture) {
    fl_texture_registrar_unregister_texture(
        self->texture_registrar, FL_TEXTURE(self->video_player_texture));
    g_object_unref(self->video_player_texture);
    self->video_player_texture = nullptr;
  }
}

static void video_player_init(VideoPlayer* self) {
  self->instance = nullptr;
  self->media = nullptr;
  self->meida_player = nullptr;
}

static void video_player_class_init(VideoPlayerClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = video_player_dispose;
}

static void* prepare_render_callback(void* opaque, void** planes) {
  VideoPlayer* self = reinterpret_cast<VideoPlayer*>(opaque);
  
  return nullptr;
}

static void post_render_callback(void* opaque, void* picture,
                                 void* const* planes) {}

static void video_display_callback(void* opaque, void* picture) {}

bool video_player_create(VideoPlayer* self, const char* path,
                         FlPluginRegistrar* registrar) {
  self->instance = libvlc_new(0, nullptr);
  if (self->instance == nullptr) {
    printf("libvlc new fail!\n");
    return false;
  }

  if (g_str_has_prefix(path, "http")) {
    self->media = libvlc_media_new_location(self->instance, path);
  } else {
    self->media = libvlc_media_new_path(self->instance, path);
  }
  if (self->media == nullptr) {
    printf("libvlc create media fail!\n");
    return false;
  }

  self->meida_player = libvlc_media_player_new_from_media(self->media);
  if (self->meida_player == nullptr) {
    printf("libvlc create media player fail!\n");
    return false;
  }

  if (!libvlc_video_get_size(self->meida_player, 0, &self->video_width,
                             &self->video_height)) {
    printf("libvlc video get size fail!\n");
    return false;
  }

  libvlc_video_set_callbacks(self->meida_player, prepare_render_callback,
                             post_render_callback, video_display_callback,
                             self);

  libvlc_video_set_format(self->meida_player, "RGBA", self->video_width,
                          self->video_height, self->video_width * 4);
  self->texture_registrar =
      fl_plugin_registrar_get_texture_registrar(registrar);
  self->video_player_texture =
      video_player_texture_new(self->video_width, self->video_height);
  bool success = fl_texture_registrar_register_texture(
      self->texture_registrar, FL_TEXTURE(self->video_player_texture));
  if (!success) {
    g_object_unref(self->video_player_texture);
    self->video_player_texture = nullptr;
  } else {
    self->texture_id =
        reinterpret_cast<int64_t>(FL_TEXTURE(self->video_player_texture));
  }
  return true;
}

int video_player_play(VideoPlayer* self) {
  return libvlc_media_player_play(self->meida_player);
}

void video_player_pause(VideoPlayer* self) {
  libvlc_media_player_pause(self->meida_player);
}

void video_player_set_looping(VideoPlayer* self, bool is_looping) {
  libvlc_media_add_option(self->media, is_looping ? "--loop" : "--no-loop");
}

int video_player_set_volume(VideoPlayer* self, double volume) {
  return libvlc_audio_set_volume(self->meida_player, (int)volume);
}

int video_player_set_play_back_speed(VideoPlayer* self, double speed) {
  return libvlc_media_player_set_rate(self->meida_player, speed);
}

void video_player_seek(VideoPlayer* self, int position) {
  libvlc_media_player_set_time(self->meida_player, position);
}

int video_player_get_position(VideoPlayer* self) {
  return libvlc_media_player_get_time(self->meida_player);
}


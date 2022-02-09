#include "include/video_player/video_player.h"

#include <vlc/vlc.h>

#define VIDEO_PLAYER_(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), video_player_get_type(), VideoPlayer))

struct _VideoPlayer {
  GObject parent_instance;
  libvlc_instance_t* instance;
  libvlc_media_t* media;
  libvlc_media_player_t* meida_player;
};

G_DEFINE_TYPE(VideoPlayer, video_player, g_object_get_type())

static void video_player_dispose(GObject* object) {}

static void video_player_init(VideoPlayer* self) {}

static void video_player_class_init(VideoPlayerClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = video_player_dispose;
}

bool video_player_create(VideoPlayer* self, const char* path) {
  self->instance = libvlc_new(0, NULL);
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

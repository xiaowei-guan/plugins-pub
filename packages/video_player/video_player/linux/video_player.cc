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

int video_player_play(VideoPlayer* player) {
  return libvlc_media_player_play(player->meida_player);
}

void video_player_pause(VideoPlayer* player) {}

void video_player_set_looping(VideoPlayer* player, bool is_looping) {}

void video_player_set_volume(VideoPlayer* player, double volume) {}

void video_player_set_play_back_speed(VideoPlayer* player, double speed) {}

void video_player_seek(VideoPlayer* player, int position) {}

int video_player_get_position(VideoPlayer* player) { return 0; }
#ifndef FLUTTER_PLUGIN_VIDEO_PLAYER_H_
#define FLUTTER_PLUGIN_VIDEO_PLAYER_H_

#include <flutter_linux/flutter_linux.h>

typedef struct _VideoPlayer VideoPlayer;

typedef struct {
  GObjectClass parent_class;
} VideoPlayerClass;

GType video_player_get_type();

int video_player_play(VideoPlayer* player);

void video_player_pause(VideoPlayer* player);

void video_player_set_looping(VideoPlayer* player, bool is_looping);

void video_player_set_volume(VideoPlayer* player, double volume);

void video_player_set_play_back_speed(VideoPlayer* player, double speed);

void video_player_seek(VideoPlayer* player, int position);

int video_player_get_position(VideoPlayer* player);

#endif
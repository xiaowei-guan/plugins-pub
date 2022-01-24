#ifndef FLUTTER_PLUGIN_VIDEO_PLAYER_H_
#define FLUTTER_PLUGIN_VIDEO_PLAYER_H_

typedef struct _VideoPlayer VideoPlayer;

typedef struct {
  GObjectClass parent_class;
} VideoPlayerClass;

GType video_player_plugin_get_type();

#endif
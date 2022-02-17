#include "include/video_player/video_player_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#include "include/video_player/video_player.h"

#define VIDEO_PLAYER_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), video_player_plugin_get_type(), \
                              VideoPlayerPlugin))

struct _VideoPlayerPlugin {
  GObject parent_instance;
  FlBinaryMessenger* messenger;
  GHashTable* video_table;
};

G_DEFINE_TYPE(VideoPlayerPlugin, video_player_plugin, g_object_get_type())

static void video_player_plugin_dispose(GObject* object) {
  VideoPlayerPlugin* self = VIDEO_PLAYER_PLUGIN(object);
  if (self->video_table) {
    g_hash_table_destroy(self->video_table);
  }
}

static void video_player_plugin_class_init(VideoPlayerPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = video_player_plugin_dispose;
}

static void create_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {
  // VideoPlayerPlugin* self = static_cast<VideoPlayerPlugin*>(user_data);
}

static void dispose_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void initialize_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void set_looping_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void set_volume_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void set_playback_speed_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void play_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void position_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void seek_to_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void pause_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void set_mix_with_others_message_channel_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void video_player_plugin_create_channel(
    VideoPlayerPlugin* self, const gchar* channel_name,
    FlBasicMessageChannelMessageHandler handler) {
  g_autoptr(FlStandardMessageCodec) codec = fl_standard_message_codec_new();
  g_autoptr(FlBasicMessageChannel) channel = fl_basic_message_channel_new(
      self->messenger, channel_name, FL_MESSAGE_CODEC(codec));
  fl_basic_message_channel_set_message_handler(
      channel, handler, g_object_ref(self), g_object_unref);
}

static void video_player_plugin_init(VideoPlayerPlugin* self) {
  self->video_table = g_hash_table_new(g_int64_hash, g_int_equal);
}

static void video_player_plugin_setup_channel(VideoPlayerPlugin* self) {
  video_player_plugin_create_channel(self,
                                     "dev.flutter.pigeon.VideoPlayerApi.create",
                                     create_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.dispose",
      dispose_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.initialize",
      initialize_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.setLooping",
      set_looping_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.setVolume",
      set_volume_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.setPlaybackSpeed",
      set_playback_speed_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.play", play_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.position",
      position_message_channel_cb);
  video_player_plugin_create_channel(self,
                                     "dev.flutter.pigeon.VideoPlayerApi.seekTo",
                                     seek_to_message_channel_cb);
  video_player_plugin_create_channel(self,
                                     "dev.flutter.pigeon.VideoPlayerApi.pause",
                                     pause_message_channel_cb);
  video_player_plugin_create_channel(
      self, "dev.flutter.pigeon.VideoPlayerApi.setMixWithOthers",
      set_mix_with_others_message_channel_cb);
}

void video_player_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  VideoPlayerPlugin* plugin = VIDEO_PLAYER_PLUGIN(
      g_object_new(video_player_plugin_get_type(), nullptr));
  plugin->messenger = fl_plugin_registrar_get_messenger(registrar);
  video_player_plugin_setup_channel(plugin);
  g_object_unref(plugin);
}

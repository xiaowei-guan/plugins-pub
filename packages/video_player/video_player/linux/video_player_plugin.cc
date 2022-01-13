#include "include/video_player/video_player_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#define VIDEO_PLAYER_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), video_player_plugin_get_type(), \
                              VideoPlayerPlugin))

struct _VideoPlayerPlugin {
  GObject parent_instance;
  GHashTable* plugin_channels;
};

G_DEFINE_TYPE(VideoPlayerPlugin, video_player_plugin, g_object_get_type())

static void video_player_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(video_player_plugin_parent_class)->dispose(object);
}

static void video_player_plugin_class_init(VideoPlayerPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = video_player_plugin_dispose;
}

static void create_message_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

static void video_player_plugin_init(VideoPlayerPlugin* self) {
  self->plugin_channels = g_hash_table_new(g_str_hash, g_str_equal);
}

static void video_player_plugin_setup_channel(VideoPlayerPlugin* self,
                                              FlPluginRegistrar* registrar) {
  g_autoptr(FlStandardMessageCodec) codec = fl_standard_message_codec_new();
  FlBinaryMessenger* messenger = fl_plugin_registrar_get_messenger(registrar)
      g_autoptr(FlBasicMessageChannel) create_channel =
          fl_basic_message_channel_new(
              messenger, "dev.flutter.pigeon.VideoPlayerApi.create",
              FL_MESSAGE_CODEC(codec));
  fl_basic_message_channel_set_message_handler(
      create_channel, create_message_cb, g_object_ref(self), g_object_unref);
}

void video_player_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  VideoPlayerPlugin* plugin = VIDEO_PLAYER_PLUGIN(
      g_object_new(video_player_plugin_get_type(), nullptr));
  video_player_plugin_setup_channel(plugin, registrar);
  g_object_unref(plugin);
}

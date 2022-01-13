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
};

G_DEFINE_TYPE(VideoPlayerPlugin, video_player_plugin, g_object_get_type())

static void video_player_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(video_player_plugin_parent_class)->dispose(object);
}

static void video_player_plugin_class_init(VideoPlayerPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = video_player_plugin_dispose;
}

static void video_player_plugin_init(VideoPlayerPlugin* self) {}

static void create_message_cb(
    FlBasicMessageChannel* channel, FlValue* message,
    FlBasicMessageChannelResponseHandle* response_handle, gpointer user_data) {}

void video_player_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  VideoPlayerPlugin* plugin = VIDEO_PLAYER_PLUGIN(
      g_object_new(video_player_plugin_get_type(), nullptr));
  g_autoptr(FlStandardMessageCodec) codec = fl_standard_message_codec_new();
  g_autoptr(FlBasicMessageChannel) create_channel =
      fl_basic_message_channel_new(fl_plugin_registrar_get_messenger(registrar),
                                   "dev.flutter.pigeon.VideoPlayerApi.create",
                                   FL_MESSAGE_CODEC(codec));
  fl_basic_message_channel_set_message_handler(
      create_channel, create_message_cb, g_object_ref(plugin), g_object_unref);
  g_object_unref(plugin);
}

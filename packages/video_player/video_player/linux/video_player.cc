#include "video_player.h"

#define VIDEO_PLAYER_(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), video_player_get_type(), \
                              VideoPlayer))

struct _VideoPlayer {
  GObject parent_instance;
  GHashTable* video_player_table;
};

G_DEFINE_TYPE(VideoPlayer, video_player, g_object_get_type())

static void video_player_dispose(GObject* object) {
  FlTextureRegistrarImpl* self = FL_TEXTURE_REGISTRAR_IMPL(object);
  g_clear_pointer(&self->textures, g_hash_table_unref);
  G_OBJECT_CLASS(video_player_parent_class)->dispose(object);
}

static void video_player_init(VideoPlayer* self) {
    g_autoptr(GHashTable) video_player_table = self->video_player_table;
    self->video_player_table = g_hash_table_new_full(g_direct_hash, g_direct_equal, nullptr,
                                         g_object_unref);
}

static void video_player_class_init(VideoPlayerClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = video_player_dispose;
}
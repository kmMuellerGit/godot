
#include "update_loop_server.h"
#include "modules/godot_tracy/tracy/public/tracy/Tracy.hpp"
#include "scene/main/scene_tree.h"

void UpdateLoopServer::_bind_methods() {
	ADD_SIGNAL(MethodInfo("frame_start_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("pre_process_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("post_process_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("pre_render_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("post_render_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("pre_script_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("post_script_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
	ADD_SIGNAL(MethodInfo("frame_end_update", PropertyInfo(Variant::BOOL, "is_editor"), PropertyInfo(Variant::FLOAT, "real_time"), PropertyInfo(Variant::FLOAT, "game_time")));
}

bool UpdateLoopServer::IsEditor() {
	SceneTree *scene_tree = SceneTree::get_singleton();
	bool invalid_scene = scene_tree == nullptr || scene_tree->is_suspended();
	return invalid_scene;
}

UpdateLoopServer *UpdateLoopServer::get_singleton() {
	return singleton;
};

void UpdateLoopServer::Emit_FrameStartUpdate(double real_time, double game_time) {
	ZoneScopedN("Update Server - frame_start_update");
	emit_signal("frame_start_update", IsEditor(), real_time, game_time);
}

void UpdateLoopServer::Emit_PreProcessUpdate(double real_time, double game_time) {
	ZoneScopedN("Update Server - pre_process_update");
	emit_signal("pre_process_update",IsEditor(), real_time, game_time);
}

void UpdateLoopServer::Emit_PostProcessUpdate(double real_time, double game_time) {
	ZoneScopedN("Update Server - post_process_update");
	emit_signal("post_process_update",IsEditor(), real_time, game_time);
}

void UpdateLoopServer::Emit_PreRenderUpdate(double real_time, double game_time) {
	ZoneScopedN("Update Server - pre_render_update");
	emit_signal("pre_render_update",IsEditor(), real_time, game_time);
}

void UpdateLoopServer::Emit_PostRenderUpdate(double real_time, double game_time) {
	ZoneScopedN("Update Server - post_render_update");
	emit_signal("post_render_update",IsEditor(), real_time, game_time);
}

void UpdateLoopServer::Emit_FrameEndUpdate(double real_time, double game_time) {
	ZoneScopedN("Update Server - frame_end_update");
	emit_signal("frame_end_update",IsEditor(), real_time, game_time);
}

UpdateLoopServer::UpdateLoopServer() {
	print_line("UpdateLoopServer singleton being set.");
	ERR_FAIL_COND_MSG(singleton != nullptr, "UpdateLoopServer already exists.  This indicates improper setup.");
	singleton = this;
};

UpdateLoopServer::~UpdateLoopServer() {
	if (singleton == this) {
		print_line("UpdateLoopServer singleton being removed.");
		singleton = nullptr;
	}
};

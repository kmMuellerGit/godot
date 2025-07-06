#ifndef UPDATE_LOOP_SERVER_H
#define UPDATE_LOOP_SERVER_H

#include "core/object/class_db.h"
#include "core/object/object.h"

/// \brief Signal hookin for frame stages.
class UpdateLoopServer : public Object {
	GDCLASS(UpdateLoopServer, Object);
	static inline UpdateLoopServer *singleton;
protected:
	static void _bind_methods();
	bool IsEditor();
public:
	void Emit_FrameStartUpdate(double real_time, double game_time);
	void Emit_PreProcessUpdate(double real_time, double game_time);
	void Emit_PostProcessUpdate(double real_time, double game_time);
	void Emit_PreRenderUpdate(double real_time, double game_time);
	void Emit_PostRenderUpdate(double real_time, double game_time);
	void Emit_FrameEndUpdate(double real_time, double game_time);
	static UpdateLoopServer *get_singleton() ;

	UpdateLoopServer();
	~UpdateLoopServer();
};

#endif // UPDATE_LOOP_SERVER_H

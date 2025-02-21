/**************************************************************************/
/*  update_loop_server.h                                                  */
/**************************************************************************/


#ifndef UPDATE_LOOP_SERVER_H
#define UPDATE_LOOP_SERVER_H

#include "core/object/class_db.h"
#include "core/variant/variant.h"

#include <functional>
#include <vector>

#include "scene/main/node.h"

/// \brief When inherited and constructor called, automatically begins OnUpdate.
class UpdateServer : public Node {
	GDCLASS(UpdateServer, Node);

protected:
	/// \brief Adds itself to UpdateLoopServer singleton.
    //void _notification(int p_what);

public:
	UpdateServer();
	~UpdateServer();
    //void _process();
	/// \brief Called after game logic has processed.
	/// \param realTime Real clock on wall time which has passed since last update.
	/// \param gameTime Real time * game speed
	virtual void OnUpdate(double realTime, double gameTime){};
	virtual void PrePhysicsUpdate(double realTime, double gameTime){};
	virtual void PhysicsUpdate(double realTime, double gameTime){};
	virtual void PostPhysicsUpdate(double realTime, double gameTime){};
	virtual void PreRenderUpdate(double realTime, double gameTime){};
	virtual void PostUpdate(double realTime, double gameTime){};
};

/// \brief Object in main loop which calls parent objects.
class UpdateLoopServer : public Object {
	GDCLASS(UpdateLoopServer, Object);

public:
	/// \brief Adds server to list of servers to call 'OnUpdate' on.
	void AddToUpdate(UpdateServer* server);
	void RemoveFromUpdate(UpdateServer* server);
	void Update(double realTime, double gameTime);
	//void OnUpdate(double realTime, double gameTime);
	void PrePhysicsUpdate(double realTime, double gameTime);
	void PhysicsUpdate(double realTime, double gameTime);
	void PostPhysicsUpdate(double realTime, double gameTime);
	void PreRenderUpdate(double realTime, double gameTime);
	void PostUpdate(double realTime, double gameTime);

	static UpdateLoopServer *get_singleton() ;//{ return singleton;}

	UpdateLoopServer();
	~UpdateLoopServer();

private:
	std::vector<UpdateServer*> updateList;
	static UpdateLoopServer *singleton;

};

#endif // UPDATE_LOOP_SERVER_H

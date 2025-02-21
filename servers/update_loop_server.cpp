
#include "update_loop_server.h"



UpdateServer::UpdateServer() {

	ERR_FAIL_COND_MSG(UpdateLoopServer::get_singleton() == nullptr, "Update loop server could not be created!");
	auto loopServer = UpdateLoopServer::get_singleton();
	loopServer->AddToUpdate(this);
}

UpdateServer::~UpdateServer() {

	auto loopServer = UpdateLoopServer::get_singleton();
	if (loopServer ) {
		loopServer->RemoveFromUpdate(this);
	}
}


void UpdateLoopServer::AddToUpdate(UpdateServer* server) {
	//If already added, reset.
	if (server==nullptr){return;}
	if (std::find(updateList.begin(), updateList.end(), server)==updateList.end()) {
		updateList.emplace_back(server);
		if (server->get_name() == "") {
			server->set_name("TempName" + itos( OS::get_singleton()->get_ticks_usec()));
		}

	}

}

void UpdateLoopServer::RemoveFromUpdate(UpdateServer* server) {
	if (server==nullptr){return;}
	auto foundIndex = std::find(updateList.begin(), updateList.end(),server);
	if (foundIndex != updateList.end()) {
		updateList.erase(foundIndex);
		print_line("UpdateLoopServer removing from updates the server [", server->get_name(), "]");

	}

}

void UpdateLoopServer::Update(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
	SceneTree *scene_tree = SceneTree::get_singleton();
	if (scene_tree == nullptr || scene_tree->is_suspended()) {
		return; // Do no work while disabled.
	}
	for (auto e : this->updateList) {
		if (e == nullptr)
		{
			removeList.emplace_back(e);
		}
		else {
			e->OnUpdate(realTime, gameTime);
		}
	}

	//Remove dead servers.
	for (auto e : removeList) {
		RemoveFromUpdate(e);
	}
}

UpdateLoopServer *UpdateLoopServer::singleton = nullptr;

UpdateLoopServer *UpdateLoopServer::get_singleton() {
	return singleton;
};

void UpdateLoopServer::PrePhysicsUpdate(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
	SceneTree *scene_tree = SceneTree::get_singleton();
	if (scene_tree == nullptr || scene_tree->is_suspended()) {
		return; // Do no work while disabled.
	}
	for (auto e : this->updateList) {
		if (e == nullptr)
		{
			removeList.emplace_back(e);
		}
		else {
			e->PrePhysicsUpdate(realTime, gameTime);
		}

	}
	for (auto e : removeList) {
		RemoveFromUpdate(e);
	}
}

void UpdateLoopServer::PhysicsUpdate(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
	SceneTree *scene_tree = SceneTree::get_singleton();
	if (scene_tree == nullptr || scene_tree->is_suspended()) {
		return; // Do no work while disabled.
	}
	for (auto e : this->updateList) {
		if (e == nullptr)
		{
			removeList.emplace_back(e);
		}
		else {
			e->PhysicsUpdate(realTime, gameTime);
		}

	}
	for (auto e : removeList) {
		RemoveFromUpdate(e);
	}
}

void UpdateLoopServer::PostPhysicsUpdate(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
	SceneTree *scene_tree = SceneTree::get_singleton();
	if (scene_tree == nullptr || scene_tree->is_suspended()) {
		return; // Do no work while disabled.
	}
	for (auto e : this->updateList) {
		if (e == nullptr)
		{
			removeList.emplace_back(e);
		}
		else {
			e->PostPhysicsUpdate(realTime, gameTime);
		}

	}
	for (auto e : removeList) {
		RemoveFromUpdate(e);
	}
}

void UpdateLoopServer::PreRenderUpdate(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
	SceneTree *scene_tree = SceneTree::get_singleton();
	if (scene_tree == nullptr || scene_tree->is_suspended()) {
		return; // Do no work while disabled.
	}
	for (auto e : this->updateList) {
		if (e == nullptr)
		{
			removeList.emplace_back(e);
		}
		else {
			e->PreRenderUpdate(realTime, gameTime);
		}

	}
	for (auto e : removeList) {
		RemoveFromUpdate(e);
	}
}

void UpdateLoopServer::PostUpdate(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
	SceneTree *scene_tree = SceneTree::get_singleton();
	if (scene_tree == nullptr || scene_tree->is_suspended()) {
		return; // Do no work while disabled.
	}
	for (auto e : this->updateList) {
		if (e == nullptr)
		{
			removeList.emplace_back(e);
		}
		else {
			e->PostUpdate(realTime, gameTime);
		}

	}
	for (auto e : removeList) {
		RemoveFromUpdate(e);
	}
}

UpdateLoopServer::UpdateLoopServer() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "UpdateLoopServer already existed - failing out");
	singleton = this;
};

UpdateLoopServer::~UpdateLoopServer() {
	if (singleton == this) {
		singleton = nullptr;
	}
};

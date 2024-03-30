
#include "update_loop_server.h"



UpdateServer::UpdateServer() {
	auto loopServer = UpdateLoopServer::get_singleton();
	if (loopServer ) {
		loopServer->AddToUpdate(this);
	}
}

UpdateServer::~UpdateServer() {

	auto loopServer = UpdateLoopServer::get_singleton();
	if (loopServer ) {
		loopServer->RemoveFromUpdate(this);
	}
}


void UpdateLoopServer::AddToUpdate(UpdateServer* server) {
	//If already added, reset.

	if (std::find(updateList.begin(), updateList.end(), server)==updateList.end()) {
		updateList.emplace_back(server);
	}

}

void UpdateLoopServer::RemoveFromUpdate(UpdateServer* server) {
	auto foundIndex = std::find(updateList.begin(), updateList.end(),server);
	if (foundIndex != updateList.end()) {
		updateList.erase(foundIndex);
	}

}

void UpdateLoopServer::Update(double realTime, double gameTime) {
	std::vector<UpdateServer*> removeList = {};
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
	singleton = this;
};

UpdateLoopServer::~UpdateLoopServer() {
	singleton = nullptr;
};

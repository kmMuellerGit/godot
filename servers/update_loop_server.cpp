/**************************************************************************/
/*  camera_server.cpp                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "update_loop_server.h"

////////////////////////////////////////////////////////
// UpdateLoopServer


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

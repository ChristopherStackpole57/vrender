#ifndef ENGINE_RUNTIME_SCHEDULER_H
#define ENGINE_RUNTIME_SCHEDULER_H

#include <algorithm>
#include <iterator>
#include <vector>

#include <EngineLayer/Core/IService.h>
//#include "GameObjects/GameObjectInterface.h"

#include <EngineLayer/Core/Schedule.h>

namespace vrender::engine
{
	class RuntimeScheduler
	{
	public:
		void Start();
		void Shutdown();
		void Tick(float dt);

		void SetServiceStartupPriority(IService* service, int priority);
		void SetServiceTickPriority(IService* service, int priority);

		//void SetObjectStartupPriority(IGameObject* game_object, int priority);
		//void SetObjectTickPriority(IGameObject* game_object, int priority);

		int GetGameTime();

		void Pause();
		void Unpause();
	private:
		vrender::engine::Schedule<IService*> service_startup_order;
		vrender::engine::Schedule<IService*> service_tick_order;

		//vrender::engine::Schedule<IGameObject*> object_startup_order;
		//vrender::engine::Schedule<IGameObject*> object_tick_order;

		bool paused = false;
		int game_time = 0;
	};

	// Startup Priority Categories
	constexpr int SCHEDULE_STARTUP_LEVEL_PRELOAD_CACHE = 1;
	constexpr int SCHEDULE_STARTUP_LEVEL_BASIC_SERVICE = 0;
	constexpr int SCHEDULE_STARTUP_LEVEL_DEPENDANT = -1;
	constexpr int SCHEDULE_STARTUP_LEVEL_BASIC_GAMEOBJECT = -2;

	// Tick Priority Categories
	constexpr int SCHEDULE_TICK_LEVEL_PREFRAME = 1;
	constexpr int SCHEDULE_TICK_LEVEL_BASIC = 0;
	constexpr int SCHEDULE_TICK_LEVEL_FRAMEPREP = -1;
	constexpr int SCHEDULE_TICK_LEVEL_FRAMERENDER = -2;
	constexpr int SCHEDULE_TICK_LEVEL_POSTFRAME = -3;

	// Shutdown Priority Categories
	constexpr int SCHEDULE_SHUTDOWN_LEVEL_BASIC = 0;
}

#endif
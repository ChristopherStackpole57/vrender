#include <iostream>

#include <EngineLayer/Core/RuntimeScheduler.h>

std::vector<int> merge_priority_list(std::vector<int> v1, std::vector<int> v2)
{
	std::vector<int> merge;
	std::set_union(
		v1.begin(), v1.end(),
		v2.begin(), v2.end(),
		std::back_inserter(merge)
	);

	return merge;
}

void vrender::engine::RuntimeScheduler::Start()
{
	std::vector<int> service_priorities = service_startup_order.GetPriorities();
	//std::vector<int> object_priorities = object_startup_order.GetPriorities();

	std::vector<int> merge = service_priorities;// merge_priority_list(service_priorities, object_priorities);

	for (int i = merge.size() - 1; i >= 0; i--)
	{
		int priority = merge[i];
		for (IService* service : service_startup_order.GetEntriesOfPriority(priority))
		{
			service->Start();
		}
		//for (IGameObject* game_object : object_startup_order.GetEntriesOfPriority(priority))
		//{
			//game_object->Start();
		//}
	}
}
void vrender::engine::RuntimeScheduler::Shutdown()
{

}
void vrender::engine::RuntimeScheduler::Tick(float dt)
{
	dt = (paused) ? 0 : dt;

	game_time += dt;

	std::vector<int> service_priorities = service_tick_order.GetPriorities();
	//std::vector<int> object_priorities = object_tick_order.GetPriorities();

	std::vector<int> merge = service_priorities;//merge_priority_list(service_priorities, object_priorities);

	for (int i = merge.size() - 1; i >= 0; i--)
	{
		int priority = merge[i];
		for (IService* service : service_tick_order.GetEntriesOfPriority(priority))
		{
			service->Tick(dt);
		}
		//for (IGameObject* game_object : object_tick_order.GetEntriesOfPriority(priority))
		//{
			//game_object->Tick(dt);
		//}
	}
}

void vrender::engine::RuntimeScheduler::SetServiceStartupPriority(IService* service, int priority)
{
	service_startup_order.Add(service, priority);

	if (game_time > 0)
	{
		service->Start();
	}
}
void vrender::engine::RuntimeScheduler::SetServiceTickPriority(IService* service, int priority)
{
	service_tick_order.Add(service, priority);
}
/*void RuntimeScheduler::SetObjectStartupPriority(IGameObject* game_object, int priority)
{
	// Add the provided service with to the object_startup_order CallOrder with the relevant priority
	object_startup_order.Add(game_object, priority);

	if (game_time > 0)
	{
		game_object->Start();
	}
}*/
/*void CallService::SetObjectTickPriority(IGameObject* game_object, int priority)
{
	// Add the provided service with to the object_tick_order CallOrder with the relevant priority
	object_tick_order.Add(game_object, priority);
}*/

int vrender::engine::RuntimeScheduler::GetGameTime()
{
	return game_time;
}

void vrender::engine::RuntimeScheduler::Pause()
{
	paused = true;
}
void vrender::engine::RuntimeScheduler::Unpause()
{
	paused = false;
}
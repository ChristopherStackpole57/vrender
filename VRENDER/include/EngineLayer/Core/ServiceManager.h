#ifndef ENGINE_SERVICE_MANAGER_H
#define ENGINE_SERVICE_MANAGER_H

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <EngineLayer/Core/IService.h>

class CallService;

namespace vrender::engine
{
	class ServiceManager
	{
	public:
		template<typename T>
		T* RegisterService()
		{
			static_assert(std::is_base_of<IService, T>::value, "ERROR: ServiceManager Attempted to Register Service That Does Not Derive From IService");

			auto service = std::make_unique<T>();
			T* service_pointer = service.get();

			registered_services[typeid(T)] = std::move(service);

			return service_pointer;
		}

		template<typename T>
		T* Get()
		{
			auto service_index = registered_services.find(typeid(T));

			if (service_index != registered_services.end())
			{
				return static_cast<T*>(service_index->second.get());
			}

			return nullptr;
		}
	private:
		vrender::engine::RuntimeScheduler runtime_scheduler;
		std::unordered_map<std::type_index, std::unique_ptr<IService>> registered_services;
	};

	inline ServiceManager& Services()
	{
		static ServiceManager service_manager;
		return service_manager;
	}
}

#endif
#ifndef ENGINE_SCHEDULE_H
#define ENGINE_SCHEDULE_H

#include <algorithm>
#include <map>
#include <vector>

namespace vrender::engine
{
	template <typename T>
	class Schedule
	{
	public:
		void Add(T entry, int priority)
		{
			collections[priority].push_back(entry);
		}
		void Remove(T entry)
		{
			for (auto& [priority, collection] : collections)
			{
				collection.erase(
					std::remove(
						collection.begin(),
						collection.end(),
						entry
					),
					collection.end()
				);
			}
		}

		std::vector<int> GetPriorities()
		{
			std::vector<int> priorities;
			for (const auto& pair : collections)
			{
				priorities.push_back(pair.first);
			}
			return priorities;
		}

		std::vector<T> GetEntriesOfPriority(int priority)
		{
			if (collections.find(priority) == collections.end())
			{
				return std::vector<T>();
			}
			return collections[priority];
		}

	private:
		std::map<int, std::vector<T>> collections;
	};
}

#endif
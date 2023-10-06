#pragma once
#include <unordered_map>
#include <functional>

namespace RoxEngine {
	// Meant for dynamic metadata not static, like information on a mesh from a custom renderer pipeline
	class Metadata {
	public:
		~Metadata() {
			for (auto& [key, data] : mData)
			{
				data.mDelete(data.ptr);
			}
		}

		template<typename T>
		void Add(T&& val) {
			RE_CORE_ASSERT(!Has<T>(), "Adding metadata that already exists");
			mData.insert({ typeid(T).name(),{new T(val), [](void* raw_value) {T* value = (T*)raw_value; delete raw_value; }} });
		}
		template<typename T>
		void AddOrReplace(T&& val) {
			if (Has<T>())
				Remove<T>();
			Add<T>(std::move(val));
		}
		template<typename T>
		bool Has() {
			return mData.find(typeid(T).name()) != mData.end();
		}
		template<typename T>
		T* Get() {
			RE_CORE_ASSERT(Has<T>(), "Getting metadata that doesn't exist");
			return (T*)mData.at(typeid(T).name()).ptr;
		}

		template<typename T>
		void Remove() {
			RE_CORE_ASSERT(Has<T>(), "Removing metadata that doesn't exist");
			delete Get<T>();
			mData.erase(typeid(T).name());
		}
	private:
		struct Data {
			void* ptr;
			std::function<void(void*)> mDelete;
		};

		std::unordered_map<std::string, Data> mData;
	};

}
#pragma once
#include <RoxEngine/core/Metadata.h>

namespace RoxEngine {
	struct Vertex {
		Vertex(glm::vec3 Position, glm::vec2 Uv = { 0,0 }, glm::vec3 Normal = { 0,0,0 }) {
			position = Position;
			uv = Uv;
			normal = Normal;
		}

		glm::vec3 position;
		glm::vec2 uv = { 0,0 };
		glm::vec3 normal = { 0,0,0 };
	};

	class Mesh {
	public:
		// Depending on the type some functions can't be called
		enum Type {
			STATIC,
			DYNAMIC,
		};

		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Type type = Type::DYNAMIC) : type(type) {
			mVertices = vertices;
			mIndices = indices;
		}
		Mesh() : type(Type::DYNAMIC) {
		}

		const std::vector<Vertex>& GetReadOnlyVertices() const {
			return mVertices;
		}
		const std::vector<uint32_t>& GetReadOnlyIndices() const {
			return mIndices;
		}

		std::vector<Vertex>& GetVertices() {
			RE_CORE_ASSERT(type == Type::DYNAMIC, "Cannot get vertices of static mesh.");
			return mVertices;
		}
		std::vector<uint32_t>& GetIndices() {
			RE_CORE_ASSERT(type == Type::DYNAMIC, "Cannot get indices of static mesh.");
			return mIndices;
		}
		// Returns the radius of the bounding box starting from the center
		glm::vec3 FindBoundingBox()
		{
			auto threads_count = std::thread::hardware_concurrency();

			std::function findboudbox_fn = [](std::vector<Vertex>& vertices, size_t startIndex, size_t endIndex, glm::vec3& data) {
				data = { 0,0,0 };
				for (int i = startIndex; i < endIndex; i++) {
					if (vertices[i].position.x > data.x)
						data.x = vertices[i].position.x;
					if (vertices[i].position.y > data.y)
						data.y = vertices[i].position.y;
					if (vertices[i].position.z > data.z)
						data.z = vertices[i].position.z;
				}
				};

			// this atleast gotta be larger than threads_count
			if (mVertices.size() < 500 && mVertices.size() > threads_count) {
				glm::vec3 data;
				findboudbox_fn(mVertices, 0, mVertices.size(), data);
				return data / 2.f;
			}
			std::vector<std::thread> threads;
			std::vector<glm::vec3> threads_data;



			threads.reserve(threads_count);

			const size_t partitionSize = mVertices.size() / threads_count;
			size_t currentSize = 0;


			threads.reserve(threads_count);
			threads_data.resize(threads_count);
			for (int i = 0; i < threads_count; i++)
			{
				auto start_index = currentSize;
				auto end_index = currentSize += partitionSize;
				if ((i + 1) == threads_count && currentSize != mVertices.size()) {
					end_index = mVertices.size();
				}
				threads.push_back(std::thread(findboudbox_fn, std::ref(mVertices), start_index, end_index, std::ref(threads_data.at(i))));
			}
			while (true) {
				bool allThreadsJoinable = threads[0].joinable();
				for (int i = 1; i < threads_count; i++)
				{
					allThreadsJoinable = allThreadsJoinable && threads[i].joinable();
				}
				if (allThreadsJoinable)
					break;
			}
			glm::vec3 finalData = { 0,0,0 };
			for (int i = 0; i < threads_count; i++)
			{
				threads[i].join();

				if (threads_data[i].x > finalData.x)
					finalData.x = threads_data[i].x;
				if (threads_data[i].y > finalData.y)
					finalData.y = threads_data[i].y;
				if (threads_data[i].z > finalData.z)
					finalData.z = threads_data[i].z;
			}
			return finalData / 2.f;
		}


		Metadata metadata;
		const Type type;
	private:
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
	};
}
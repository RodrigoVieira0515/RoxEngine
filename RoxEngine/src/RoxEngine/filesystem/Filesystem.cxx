module;
#include <string>
export module roxengine:filesystem;
import :utils;

export namespace RoxEngine::FileSystem {
	bool IsResourcePath(const std::string& path);
	bool Exists(const std::string& path);

	std::string GetFileName(const std::string& path,bool with_extension = true);
	size_t GetFileSize(const std::string& path);
	Buffer ReadFile(const std::string& path);
	std::string ReadTextFile(const std::string& path);
}
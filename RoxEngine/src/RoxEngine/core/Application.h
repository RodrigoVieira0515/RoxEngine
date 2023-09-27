#pragma once
#include <RoxEngine/core/Window.h>

int main(int argc, char** argv);

namespace RoxEngine {
	struct ApplicationSpec {
		WindowDesc windowdesc;
	};

	class Application
	{
	public:
		Application(const ApplicationSpec& spec);
		virtual ~Application();

	private:
		friend int ::main(int argc, char** argv);
		virtual void OnInit() = 0;
		virtual void OnRender() = 0;
		virtual void OnShutdown() = 0;
	
		std::shared_ptr<Window> mMainWindow;
	protected:
		void Run();
	};
}

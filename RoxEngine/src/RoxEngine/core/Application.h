#pragma once
#include <RoxEngine/core/Window.h>
#include <RoxEngine/events/Event.h>

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

	protected:
		friend int ::main(int argc, char** argv);
		virtual void OnInit() = 0;
		virtual void OnRender() = 0;
		virtual void OnShutdown() = 0;
		virtual void OnEvent(Event& event) = 0;

		std::shared_ptr<Window> mMainWindow;
	private:
		void Run();
	};
}

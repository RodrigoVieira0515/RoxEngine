#pragma once
#include <RoxEngine/core/Application.h>
#include <RoxEngine/core/Logger.h>

using namespace RoxEngine;

extern Application* createApp();

int main(int argc, char** argv)
{
	Logger::Init();

	Application* app = createApp();
	app->Run();
}
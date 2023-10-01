#include "Graphics.h"

namespace RoxEngine {
	static RendererPipeline* sPipeline = nullptr;
	void Graphics::Init()
	{
		RE_CORE_ASSERT(!sPipeline, "Graphics initilized more than once!");
		sPipeline = CreateRendererPipeline();
	}

	void Graphics::Shutdown()
	{
		RE_CORE_ASSERT(sPipeline, "Graphics::Shutdown() without ever calling Graphics::Init()!");
	}

	RendererPipeline* Graphics::GetRendererPipeline()
	{
		RE_CORE_ASSERT(sPipeline, "Graphics::GetRendererPipeline() without ever calling Graphics::Init()!");
		return sPipeline;
	}
}
#pragma once
#include "RendererApi.h"

namespace RoxEngine {
    struct RendererApiData {
        static API sAPI;
        static std::shared_ptr<RendererApi> sRendererAPI;
    };
}
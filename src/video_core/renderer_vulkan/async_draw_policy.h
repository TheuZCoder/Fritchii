// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "common/common_types.h"

namespace Vulkan {

/// State of a native (Switch 2) draw whose pipeline is still being built.
struct NativePendingDraw {
    bool depth_tested{};   ///< Depth buffer bound and depth test on.
    bool writes_color{};   ///< At least one color target with a non-zero write mask.
    bool target_redrawn{}; ///< Render target 0 was first written in an earlier frame.
};

constexpr bool CanDiscardPendingDraw(bool native, bool depth_enabled, u32 index_count,
                                    u32 vertex_count, NativePendingDraw native_draw = {}) {
    // Native draws can produce persistent resources, even with depth enabled. Only a depth-tested
    // color draw into a target the title redraws every frame is skipped: it is back on screen
    // once its pipeline is built, instead of stalling the frame while it builds.
    if (native) {
        return native_draw.depth_tested && native_draw.writes_color && native_draw.target_redrawn;
    }
    // Preserve the legacy async-shader heuristic.
    return depth_enabled || (index_count > 6 && vertex_count > 6);
}

} // namespace Vulkan

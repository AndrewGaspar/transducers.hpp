#pragma once

#include <atomic>

#include "escape_hatch.hpp"

namespace transducers {
    class atomic_escape_hatch : public details::escape_hatch<std::atomic_bool> {};
};
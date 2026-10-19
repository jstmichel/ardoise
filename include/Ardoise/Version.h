#pragma once

namespace Ardoise
{

// Returns the compiled framework version. The returned string has static lifetime.
[[nodiscard]] const char* version() noexcept;

} // namespace Ardoise

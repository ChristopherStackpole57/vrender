#ifndef RENDER_DEBUG_DIAGNOSTICS_H
#define RENDER_DEBUG_DIAGNOSTICS_H

#include <atomic>

struct DebugDiagnostics
{
	std::atomic<bool> validation_error_seen{ false };
};

#endif
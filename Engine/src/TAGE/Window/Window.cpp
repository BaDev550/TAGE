#include "tagepch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace TAGE {
	MEM::Scope<Window> Window::Create(const SWindowPropeties& props)
	{
		switch (Platform::GetPlatform())
		{
		case Platform::EPlatform::WINDOWS: return MEM::CreateScope<WindowsWindow>(props);
		case Platform::EPlatform::LINUX: return nullptr; CORE_LOG_ERROR("Linux is not supported for now");
		case Platform::EPlatform::MAC: return nullptr; CORE_LOG_ERROR("Mac is not supported for now");
		default:
			CORE_LOG_ERROR("Undefined Platform"); return nullptr;
			break;
		}
	}
}
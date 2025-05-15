#pragma once

#include "TAGE/Core/Utilities/Logger.h"
#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Core/Utilities/PlatformDetection.h"
#include "TAGE/Core/Utilities/Random.h"
#include "TAGE/Core/Utilities/RaycastUtils.h"
#include "TAGE/Core/Utilities/Timeline.h"

#include "TAGE/Core/Application/Application.h"
#include "TAGE/Core/Profiler/ProfilerLayer.h"
#include "TAGE/Window/Window.h"
#include "TAGE/Events/Input.h"
#include "TAGE/Layers/ImGuiLayer.h"
#include "TAGE/Layers/Layer.h"
#include "TAGE/Core/Defines.h"
#include "TAGE/ECS/Scene/IO/SceneSerializer.h"

#include "TAGE/ECS/Scene/World.h"

#include <btBulletDynamicsCommon.h>

using TEApplication = TAGE::Application;
using TEEngineMode = TAGE::ECS::SystemUpdateMode;
using TEWindow = TAGE::Window;
using TEInput = TAGE::Input;
using TEKey = TAGE::KeyCode;
using TEMouseButton = TAGE::MouseButton;
using TEProfilerLayer = TAGE::DEBUG::ProfilerLayer;
using TETimeLine = TAGE::Timeline;
using TESceneSL = TAGE::ECS::IO::SceneSerializer;
using TEScene = TAGE::ECS::Scene;

using TELayer = TAGE::Layer;
using TEImGuiLayer = TAGE::ImGuiLayer;
using TEWorld = TAGE::ECS::World;
inline TEWorld& GetWorld() { return TAGE::Application::Get().GetScene().GetWorld(); }
inline TEScene& GetScene() { return TAGE::Application::Get().GetScene(); }
inline TEApplication& GetApp() { return TAGE::Application::Get(); }
inline TEWindow& GetMainWindow() { return TAGE::Application::Get().GetWindow(); }
inline float GetDeltaTime() { return (float)TAGE::Application::Get().GetDeltaTime(); }
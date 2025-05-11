#pragma once

#include "TAGE/Events/Event.h"
#include "TAGE/Events/InputEvents.h"
#include "TAGE/Events/ApplicationEvents.h"

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Core/Utilities/Timer.h"
#include "TAGE/Core/Utilities/TimeStep.h"

#include "TAGE/Layers/LayerStack.h"
#include "TAGE/Layers/ImGuiLayer.h"

#include "TAGE/Window/Window.h"
#include "TAGE/Renderer/Renderer.h"
#include "TAGE/Renderer/Camera.h"

#include "TAGE/ECS/Scene/Scene.h"
#include "TAGE/ECS/ECS/System.h"

#include "TAGE/Thread/ThreadPool.h"

namespace TAGE
{
	class Application
	{
	public:
		Application(const char* AppName);
		virtual ~Application();

		void Run();
		void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() const { return *_Window; }
		ECS::Scene& GetScene() const { return *_TestScene; }
		ECS::PhysicsSystem& GetPhysicsSystem() const { return _TestScene->GetWorld().GetPhysicsSystem(); }
		ECS::RenderSystem& GetRenderSystem() const { return _TestScene->GetWorld().GetRenderSystem(); }
		RENDERER::Renderer& GetRenderer() const { return *_Renderer; }
		THREAD::ThreadPool& GetThreadPool() const { return *_AppThreadPool; }

		TimeStep GetDeltaTime() const { return _DeltaTime; }
		static Application& Get() { return *_Instance; }
		void UpdateLayers(float dt);
	private:
		bool _Running = true;
		float _LastFrameTime = 0.0f;
		TimeStep _DeltaTime = 0.0f;

		void OnEvent(Event& event);
		bool OnWindowsClose(WindowCloseEvent& e);
	private:
		LayerStack _LayerStack;
		MEM::Scope<Window> _Window;
		MEM::Scope<THREAD::ThreadPool> _AppThreadPool;
		MEM::Scope<RENDERER::Renderer> _Renderer;
		MEM::Scope<RENDERER::Camera> _Camera;
		MEM::Ref<ECS::Scene> _TestScene;
		MEM::Ref<ImGuiLayer> _ImGuiLayer;
		ECS::SystemUpdateMode _AppStat;

		static Application* _Instance;
	};

	Application* CreateApplication();
}
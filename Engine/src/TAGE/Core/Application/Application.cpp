#include "tagepch.h"
#include "Application.h"
#include "imgui.h"

namespace TAGE
{
	Application* Application::_Instance = nullptr;

	Application::Application(const char* AppName)
	{
		ENGINE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		CREATE_LOGGER();
		CORE_LOG_INFO("Application {0} created!", AppName);

		SWindowPropeties windowProps{ AppName, 1280, 720, };
		_Window = Window::Create(windowProps);
		_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		_Renderer = MEM::CreateScope<RENDERER::Renderer>();
		_Renderer->Init(_Window.get());

		_TestScene = MEM::CreateRef<ECS::Scene>("Test Map");
		if (_TestScene) {
			MEM::Ref<ECS::RenderSystem> renderSystem =  MEM::CreateRef<ECS::RenderSystem>(_Renderer.get());
			MEM::Ref<ECS::PhysicsSystem> physicsSystem = MEM::CreateRef<ECS::PhysicsSystem>(_TestScene->GetWorld().GetPhysicsWorld());
			MEM::Ref<ECS::TransformSystem> transformSystem = MEM::CreateRef<ECS::TransformSystem>();
			_TestScene->GetWorld().AddSystem(renderSystem);
			_TestScene->GetWorld().AddSystem(physicsSystem);
			_TestScene->GetWorld().AddSystem(transformSystem);
		}

		_AppThreadPool = MEM::CreateScope<THREAD::ThreadPool>();
		_ImGuiLayer = MEM::CreateRef<ImGuiLayer>();
		PushOverlay(_ImGuiLayer.get());
	}

	Application::~Application()
	{
		CORE_LOG_WARN("Application terminated!");

		_LayerStack.Clear();

		CORE_LOG_INFO("Exiting...");
	}

	void Application::UpdateLayers(float dt) {
		for (Layer* layer : _LayerStack)
			layer->OnUpdate(dt);

		_ImGuiLayer->Begin();
		for (Layer* layer : _LayerStack)
			layer->OnImGuiRender();

		_ImGuiLayer->End();
	}

	void Application::Run()
	{
		Timer timer;
		while (_Running && !_Window->ShoudClose())
		{
			float time = timer.Elapsed();
			_DeltaTime = time - _LastFrameTime;
			_LastFrameTime = time;

			_TestScene->OnUpdate(_DeltaTime, _EngineMode);

			_Window->SwapBuffers();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		_LayerStack.PushLayer(layer);
		CORE_LOG_INFO("Layer {0} pushed!", layer->GetName());
	}

	void Application::PushOverlay(Layer* layer)
	{
		_LayerStack.PushOverlay(layer);
		CORE_LOG_INFO("Overlay {0} pushed!", layer->GetName());
	}

	void Application::Close()
	{
		_Running = false;
	}

	void Application::OnEvent(Event& event)
	{
		for (auto it = _LayerStack.end(); it != _LayerStack.begin();) {
			(*--it)->OnEvent(event);
			if (event.bIsHandled)
				break;
		}
	}

	bool Application::OnWindowsClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}
}
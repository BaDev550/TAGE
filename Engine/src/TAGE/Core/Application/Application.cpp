#include "tagepch.h"
#include "Application.h"
#include "imgui.h"
#include "TAGE/ECS/GameFactory/BaseClasses/DefaultGameMode.h"

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

		_GameInstance = MEM::CreateScope<TAGE::GAMEFACTORY::GameInstance>();

		_TestScene = MEM::CreateRef<ECS::Scene>();
		_TestScene->Init();

		if (_TestScene) {
			MEM::Ref<ECS::RenderSystem> renderSystem =  MEM::CreateRef<ECS::RenderSystem>();
			MEM::Ref<ECS::PhysicsSystem> physicsSystem = MEM::CreateRef<ECS::PhysicsSystem>();
			MEM::Ref<ECS::TransformSystem> transformSystem = MEM::CreateRef<ECS::TransformSystem>();
			_TestScene->AddSystem(renderSystem);
			_TestScene->AddSystem(physicsSystem);
			_TestScene->AddSystem(transformSystem);
		}

		_GameInstance->Init();
		_GameInstance->GetCurrentWorld()->SetScene(_TestScene);
		_GameInstance->SetGameMode<DefaultGameMode>();

		_AppThreadPool = MEM::CreateScope<THREAD::ThreadPool>();
		_ImGuiLayer = MEM::CreateRef<ImGuiLayer>();
		PushOverlay(_ImGuiLayer.get());

		ASSET::AssetManager::RegisterImporter(MEM::CreateRef<ASSET::ModelImporter>());
		ASSET::AssetManager::RegisterImporter(MEM::CreateRef<ASSET::TextureImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::MaterialImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::ShaderImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::AnimationImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::SceneImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::AudioImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::FontImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::ScriptImporter>());
		//ASSET::AssetManager::RegisterImporter(MEM::CreateScope<ASSET::PrefabImporter>());
	}

	Application::~Application()
	{
		CORE_LOG_WARN("Application terminated!");

		_GameInstance->Shutdown();
		_LayerStack.Clear();

		CORE_LOG_INFO("Exiting...");
	}

	void Application::UpdateLayers(float dt) {
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

			for (Layer* layer : _LayerStack)
				layer->OnUpdate(_DeltaTime);

			//if (_Window->IsWindowIconifyed()) {
			//	SetEngineMode(ECS::SystemUpdateMode::LOADING);
			//}
			//else if (!_Window->IsWindowIconifyed()) {
			//	SetEngineMode(ECS::SystemUpdateMode::EDITOR);
			//}
			_GameInstance->Update(_DeltaTime);

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
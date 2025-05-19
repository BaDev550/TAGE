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

#include "TAGE/ECS/GameFactory/GameInstance.h"

#include "TAGE/Thread/ThreadPool.h"
#include "TAGE/Core/Profiler/Profiler.h"

#include "TAGE/Core/AssetManager/AssetManager.h"
#include "TAGE/Core/AssetManager/Importers/ModelImporter.h"
#include "TAGE/Core/AssetManager/Importers/TextureImporter.h"
//#include "TAGE/Core/AssetManager/Importers/MaterialImporter.h"
//#include "TAGE/Core/AssetManager/Importers/ShaderImporter.h"
//#include "TAGE/Core/AssetManager/Importers/AnimationImporter.h"
//#include "TAGE/Core/AssetManager/Importers/SceneImporter.h"
//#include "TAGE/Core/AssetManager/Importers/AudioImporter.h"
//#include "TAGE/Core/AssetManager/Importers/FontImporter.h"
//#include "TAGE/Core/AssetManager/Importers/ScriptImporter.h"
//#include "TAGE/Core/AssetManager/Importers/PrefabImporter.h"

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
		GAMEFACTORY::GameInstance& GetGameInstance() const { return *_GameInstance; }
		ECS::PhysicsSystem& GetPhysicsSystem() const { return _GameInstance->GetCurrentWorld()->GetScene()->GetPhysicsSystem(); }
		ECS::RenderSystem& GetRenderSystem() const { return _GameInstance->GetCurrentWorld()->GetScene()->GetRenderSystem(); }
		RENDERER::Renderer& GetRenderer() const { return *_Renderer; }
		THREAD::ThreadPool& GetThreadPool() const { return *_AppThreadPool; }
		TimeStep GetDeltaTime() const { return _DeltaTime; }
		static Application& Get() { return *_Instance; }
		void UpdateLayers(float dt);
		void SetEngineMode(ECS::SystemUpdateMode mode) { _EngineMode = mode; }
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
		MEM::Ref<GAMEFACTORY::GameInstance> _GameInstance;
		MEM::Ref<ECS::Scene> _TestScene;
		MEM::Ref<ImGuiLayer> _ImGuiLayer;
		ECS::SystemUpdateMode _EngineMode = ECS::SystemUpdateMode::GAME;

		static Application* _Instance;
	};

	Application* CreateApplication();
}
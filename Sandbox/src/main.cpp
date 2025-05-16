#include "TAGE/TAGE.h"
#include "TAGE/TComponents.h"
#include "TAGE/TObjects.h"
#include "TAGE/TEvents.h"

#include "LevelEditor/LevelEditorLayer.h"

#include "imgui.h"

class Sandbox : public TEApplication {
public:
	Sandbox() : Application("Sandbox") {
        PushLayer(new LEVELEDITOR::LevelEditorLayer());
		PushLayer(new TEProfilerLayer());
    }

    ~Sandbox() override = default;
};

TEApplication* CreateApplication() {
    return new Sandbox();
}

int main() {
    auto app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}
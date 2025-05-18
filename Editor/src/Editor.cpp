#include "TAGE/TAGE.h"
#include "EditorLayer/EditorLayer.h"

class TAGEEditor : public TEApplication {
public:
    TAGEEditor() : Application("Tourqe Advanced Game Engine Editor") {
	    TEProjectCreator::CreateProject("TAGEProject");
        PushLayer(new TAGE::EDITOR::EditorLayer());
    }

    ~TAGEEditor() override = default;
};

TEApplication* CreateApplication() {
    return new TAGEEditor();
}

int main() {
    auto app = CreateApplication();
    app->Run();
    delete app;
    return 0;
}
#include "TAGE/TAGE.h"
#include "TAGE/TComponents.h"
#include "TAGE/TObjects.h"
#include "TAGE/TEvents.h"
#include "TAGE/TPhysics.h"

#include "entitys/player/entity_player.h"

class FPSGameLayer : public TELayer {
public:
	FPSGameLayer() : TELayer("FPS") {
		GetMainWindow().EnableCursor(false);
		GetApp().SetEngineMode(TEEngineMode::GAME);
		TERenderer::_RenderMode = TERenderMode::FULL;
	}
	~FPSGameLayer() {
	}

	virtual void OnUpdate(float dt) override {
	}

	bool OnKeyPressed(TEKeyPressedE& event) {
		if (event.GetKey() == TEKey::Escape) {
			GetApp().Close();
			return true;
		}
		if (event.GetKey() == TEKey::F1) {
			_Cursor = !_Cursor;
			GetMainWindow().EnableCursor(_Cursor);
		}

		if (event.GetKey() == TEKey::F2) {
			GetApp().GetSceneSerializer().Save("scene.tagescene");
		}
		if (event.GetKey() == TEKey::F3) {
			GetApp().GetSceneSerializer().Load("scene.tagescene");
		}
		return false;
	}

	void OnEvent(TEEvent& event) override {
		TEEventDistpacher dispatcher(event);
		dispatcher.Dispatch<TEKeyPressedE>(BIND_EVENT_FN(FPSGameLayer::OnKeyPressed));
	}

private:
	bool _Cursor = false;

	entity_player player;
};
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TAGE/Events/Input.h"

namespace TAGE::RENDERER {

	class Camera {
	public:
		virtual ~Camera() = default;

		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual void SetAspectRatio(float ar) = 0;

		virtual void SetPosition(const glm::vec3& position) = 0;
		virtual void SetRotation(const glm::vec3& rotation) = 0;
		virtual const glm::vec3& GetPosition() const = 0;
		virtual const glm::vec3& GetRotation() const = 0;
		virtual void OnUpdate(float deltaTime) = 0;
	};

	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);
		PerspectiveCamera() : _FOV(45.0f), _AspectRatio(1.0f), _NearClip(0.1f), _FarClip(100.0f) {}

		void SetPosition(const glm::vec3& position) override;
		void SetRotation(const glm::vec3& rotation) override;
		void OnUpdate(float deltaTime) override;

		const glm::mat4& GetViewMatrix() const override { return _ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const override { return _ProjectionMatrix; }
		void SetAspectRatio(float ar) override { _AspectRatio = ar; }

		const glm::vec3& GetPosition() const override { return _Position; }
		const glm::vec3& GetRotation() const override { return _Rotation; }

	protected:
		void UpdateViewMatrix();

		glm::vec3 _Position;
		glm::vec3 _Rotation;

		float _FOV, _AspectRatio, _NearClip, _FarClip;

		glm::mat4 _ViewMatrix;
		glm::mat4 _ProjectionMatrix;
	};

	class EditorCamera : public PerspectiveCamera {
	public:
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
			: PerspectiveCamera(fov, aspectRatio, nearClip, farClip) {
		}
		EditorCamera()
		{
		}

		void OnUpdate(float deltaTime) override;
	private:
		float _Distance = 10.0f;
		glm::vec3 _FocalPoint = glm::vec3(0.0f);
		glm::vec2 _LastMousePosition = glm::vec2(0.0f);

		float _MovementSpeed = 10.0f;
		float _RotationSpeed = 0.3f;
		float _ZoomSpeed = 2.0f;
	};
}

#pragma once

#include "TAGE/Core/Utilities/Logger.h"
#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Core/Utilities/PlatformDetection.h"
#include "TAGE/Core/Utilities/Random.h"
#include "TAGE/Core/Utilities/RaycastUtils.h"
#include "TAGE/Core/Defines.h"

#include "TAGE/Core/Application/Application.h"
#include "TAGE/Events/Event.h"
#include "TAGE/Events/Input.h"
#include "TAGE/Events/ApplicationEvents.h"
#include "TAGE/Events/InputEvents.h"

#include "TAGE/Window/Window.h"
#include "TAGE/Layers/ImGuiLayer.h"
#include "TAGE/Layers/Layer.h"

#include "TAGE/ECS/Objects/Actor.h"
#include "TAGE/ECS/ECS/Components.h"
#include "TAGE/ECS/Scene/World.h"

#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Animator.h"

#include "TAGE/Physics/Raycaster.h"

#include <btBulletDynamicsCommon.h>

using TEApplication = TAGE::Application;
using TEWindow = TAGE::Window;
using TEInput = TAGE::Input;

using TEEvent = TAGE::Event;
using TEEventDistpacher = TAGE::EventDispatcher;
using TEKey = TAGE::KeyCode;
using TEMouseButton = TAGE::MouseButton;
using TEKeyPressedE = TAGE::KeyPressedEvent;
using TEKeyReleasedE = TAGE::KeyReleasedEvent;
using TEMouseButtonPressedE = TAGE::MouseButtonPressedEvent;

using TELayer = TAGE::Layer;
using TEImGuiLayer = TAGE::ImGuiLayer;
using TEWorld = TAGE::ECS::World;

using TEMesh = TAGE::RENDERER::Model;
using TEAnim = TAGE::RENDERER::Animation;
using TEAnimator = TAGE::RENDERER::Animator;

using TActor = TAGE::ECS::Actor;
using TStaticMeshComponent = TAGE::ECS::StaticMeshComponent;
using TSkeletalMeshComponent = TAGE::ECS::SkeletalMeshComponent;
using TAnimatorComponent = TAGE::ECS::AnimatorComponent;
using TCameraComponent = TAGE::ECS::CameraComponent;
using TCamType = TAGE::ECS::ECameraType;
using TTagComponent = TAGE::ECS::TagComponent;
using TTransformComponent = TAGE::ECS::TransformComponent;
using TRigidBodyComponent = TAGE::ECS::RigidBodyComponent;

using TEHitResult = TAGE::PHYSICS::RAYCAST::RaycastHit;
using TERayDrawType = TAGE::PHYSICS::RAYCAST::RayDrawType;
using TECast = TAGE::PHYSICS::RAYCAST::Raycaster;
#pragma once

#include "TAGE/ECS/Components/PhysicsComponents.h"
#include "TAGE/ECS/Components/RenderingComponents.h"
#include "TAGE/ECS/Components/BaseComponents.h"

using TStaticMeshComponent = TAGE::ECS::StaticMeshComponent;
using TSkeletalMeshComponent = TAGE::ECS::SkeletalMeshComponent;
using TAnimatorComponent = TAGE::ECS::AnimatorComponent;
using TCameraComponent = TAGE::ECS::CameraComponent;
using TTagComponent = TAGE::ECS::TagComponent;
using TTransformComponent = TAGE::ECS::TransformComponent;
using TRigidBodyComponent = TAGE::ECS::RigidBodyComponent;
using TColliderComponent = TAGE::ECS::ColliderComponent;
using TRelationshipComponent = TAGE::ECS::RelationshipComponent;

using TCamType = TAGE::ECS::ECameraType;
using TColliderShapeType = TAGE::ECS::ColliderShapeType;
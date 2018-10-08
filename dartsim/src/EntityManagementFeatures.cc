/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include "EntityManagementFeatures.hh"

#include <dart/config.hpp>
#include <dart/collision/fcl/FCLCollisionDetector.hpp>
#include <dart/constraint/ConstraintSolver.hpp>
#include <dart/dynamics/FreeJoint.hpp>

#include <string>

namespace ignition {
namespace physics {
namespace dartsim {

/////////////////////////////////////////////////
const std::string &EntityManagementFeatures::GetEngineName(
    const std::size_t /*_engineID*/) const
{
  static const std::string engineName = "dartsim-" DART_VERSION;
  return engineName;
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetEngineIndex(
    const std::size_t /*_engineID*/) const
{
  // The dartsim plugin does not make a distinction between different engine
  // indexes.
  return 0;
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetWorldCount(
    const std::size_t /*_engineID*/) const
{
  return worlds.size();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetWorld(
    const std::size_t, std::size_t _worldIndex) const
{
  const std::size_t id =
      this->worlds.indexInContainerToID.begin()->second[_worldIndex];
  return this->GenerateIdentity(id, this->worlds.idToObject.at(id));
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetWorld(
    const std::size_t, const std::string &_worldName) const
{
  const std::size_t id = this->worlds.IdentityOf(_worldName);
  return this->GenerateIdentity(id, this->worlds.idToObject.at(id));
}

/////////////////////////////////////////////////
const std::string &EntityManagementFeatures::GetWorldName(
    const std::size_t _worldID) const
{
  return this->worlds.at(_worldID)->getName();
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetWorldIndex(
    const std::size_t _worldID) const
{
  return this->worlds.idToIndexInContainer.at(_worldID);
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetEngineOfWorld(
    const std::size_t /*_worldID*/) const
{
  return this->GenerateIdentity(0);
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetModelCount(
    const std::size_t _worldID) const
{
  return this->worlds.at(_worldID)->getNumSkeletons();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetModel(
    const std::size_t _worldID, const std::size_t _modelIndex) const
{
  const DartSkeletonPtr &model =
      this->worlds.at(_worldID)->getSkeleton(_modelIndex);

  return this->GenerateIdentity(this->models.IdentityOf(model), model);
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetModel(
    const std::size_t _worldID, const std::string &_modelName) const
{
  const DartSkeletonPtr &model =
      this->worlds.at(_worldID)->getSkeleton(_modelName);

  return this->GenerateIdentity(this->models.IdentityOf(model), model);
}

/////////////////////////////////////////////////
const std::string &EntityManagementFeatures::GetModelName(
    const std::size_t _modelID) const
{
  return this->models.at(_modelID).model->getName();
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetModelIndex(
    const std::size_t _modelID) const
{
  return this->models.idToIndexInContainer.at(_modelID);
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetWorldOfModel(
    const std::size_t _modelID) const
{
  const std::size_t worldID = this->models.idToContainerID.at(_modelID);
  return this->GenerateIdentity(worldID, this->worlds.at(worldID));
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetLinkCount(
    const std::size_t _modelID) const
{
  return this->models.at(_modelID).model->getNumBodyNodes();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetLink(
    const std::size_t _modelID, const std::size_t _linkIndex) const
{
  DartBodyNode * const bn =
      this->models.at(_modelID).model->getBodyNode(_linkIndex);

  // TODO(MXG): Return a reference counter with this Identity
  return this->GenerateIdentity(this->links.IdentityOf(bn));
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetLink(
    const std::size_t _modelID, const std::string &_linkName) const
{
  DartBodyNode * const bn =
      this->models.at(_modelID).model->getBodyNode(_linkName);

  // TODO(MXG): Return a reference counter with this Identity
  return this->GenerateIdentity(this->links.IdentityOf(bn));
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetJointCount(
    const std::size_t _modelID) const
{
  return this->models.at(_modelID).model->getNumJoints();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetJoint(
    const std::size_t _modelID, const std::size_t _jointIndex) const
{
  DartJoint * const joint =
      this->models.at(_modelID).model->getJoint(_jointIndex);

  // TODO(MXG): Return a reference counter with this Identity
  return this->GenerateIdentity(this->joints.IdentityOf(joint));
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetJoint(
    const std::size_t _modelID, const std::string &_jointName) const
{
  DartJoint * const joint =
      this->models.at(_modelID).model->getJoint(_jointName);

  // TODO(MXG): Return a reference counter with this Identity
  return this->GenerateIdentity(this->joints.IdentityOf(joint));
}

/////////////////////////////////////////////////
const std::string &EntityManagementFeatures::GetLinkName(
    const std::size_t _linkID) const
{
  return this->links.at(_linkID)->getName();
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetLinkIndex(
    const std::size_t _linkID) const
{
  return this->links.at(_linkID)->getIndexInSkeleton();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetModelOfLink(
    const std::size_t _linkID) const
{
  const DartSkeletonPtr &skeleton = this->links.at(_linkID)->getSkeleton();
  return this->GenerateIdentity(this->models.IdentityOf(skeleton), skeleton);
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetShapeCount(
    const std::size_t _linkID) const
{
  return this->links.at(_linkID)->getNumShapeNodes();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetShape(
    const std::size_t _linkID, const std::size_t _shapeIndex) const
{
  return this->GenerateIdentity(
        this->shapes.IdentityOf(
          this->links.at(_linkID)->getShapeNode(_shapeIndex)));
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetShape(
    const std::size_t _linkID, const std::string &_shapeName) const
{
  DartBodyNode * const bn = this->links.at(_linkID);
  return this->GenerateIdentity(
        this->shapes.IdentityOf(
          bn->getSkeleton()->getShapeNode(bn->getName() + ":" + _shapeName)));
}

/////////////////////////////////////////////////
const std::string &EntityManagementFeatures::GetJointName(
    const std::size_t _jointID) const
{
  return this->joints.at(_jointID)->getName();
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetJointIndex(
    const std::size_t _jointID) const
{
  return this->joints.at(_jointID)->getJointIndexInSkeleton();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetModelOfJoint(
    const std::size_t _jointID) const
{
  const DartSkeletonPtr &skeleton = this->links.at(_jointID)->getSkeleton();
  return this->GenerateIdentity(this->models.IdentityOf(skeleton), skeleton);
}

/////////////////////////////////////////////////
const std::string &EntityManagementFeatures::GetShapeName(
    const std::size_t _shapeID) const
{
  return this->shapes.at(_shapeID).name;
}

/////////////////////////////////////////////////
std::size_t EntityManagementFeatures::GetShapeIndex(
    const std::size_t _shapeID) const
{
  return this->shapes.at(_shapeID).node->getIndexInBodyNode();
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::GetLinkOfShape(
    const std::size_t _shapeID) const
{
  return this->GenerateIdentity(
        this->links.IdentityOf(
          this->shapes.at(_shapeID).node->getBodyNodePtr()));
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::ConstructEmptyWorld(
    const std::size_t /*_engineID*/, const std::string &_name)
{
  const auto &world = std::make_shared<dart::simulation::World>(_name);

#if FCL_VERSION_AT_LEAST(0,4,0)
  auto fcl = dynamic_cast<dart::collision::FCLCollisionDetector*>(
        world->getConstraintSolver()->getCollisionDetector().get());
  if (fcl)
  {
    fcl->setPrimitiveShapeType(
          dart::collision::FCLCollisionDetector::PRIMITIVE);
  }
#endif

  return this->GenerateIdentity(this->AddWorld(world, _name), world);
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::ConstructEmptyModel(
    const std::size_t _worldID, const std::string &_name)
{
  dart::dynamics::SkeletonPtr model = dart::dynamics::Skeleton::create(_name);

  dart::dynamics::SimpleFramePtr modelFrame =
      dart::dynamics::SimpleFrame::createShared(
        dart::dynamics::Frame::World(),
        _name + "_frame");

  auto [modelID, ModelInfo] = this->AddModel({model, modelFrame}, _worldID); // NOLINT

  return this->GenerateIdentity(modelID, model);
}

/////////////////////////////////////////////////
Identity EntityManagementFeatures::ConstructEmptyLink(
    const std::size_t _modelID, const std::string &_name)
{
  const DartSkeletonPtr &model = this->models.at(_modelID).model;

  dart::dynamics::FreeJoint::Properties prop_fj;
  prop_fj.mName = _name + "_FreeJoint";

  DartBodyNode::Properties prop_bn;
  prop_bn.mName = _name;

  DartBodyNode *bn =
      model->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(
        nullptr, prop_fj, prop_bn).second;

  return this->GenerateIdentity(this->AddLink(bn));
}

}
}
}

/*
 * Copyright (C) 2017 Open Source Robotics Foundation
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

#include <ignition/physics/Feature.hh>
#include <ignition/physics/FrameSemantics.hh>
#include <ignition/physics/ForwardStep.hh>

using namespace ignition::physics;

class Conflict : public virtual Feature { };

class AnotherFeature : public virtual FeatureWithConflicts<Conflict> { };

using SomeList = FeatureList<
    FrameSemantics,
    Conflict,
    ForwardStep,
    SetState,
    AnotherFeature>;

int main()
{
  SomeList::Link<FeaturePolicy3d> link;
}

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

#include "ignition/physics/DataStatusMask.hh"

namespace ignition
{
  namespace physics
  {
    /////////////////////////////////////////////////
    DataStatusMask::DataStatusMask(const Condition e,
                                   const Condition q,
                                   const Condition r)
      : exist(e),
        queried(q),
        required(r)
    {
      // Do nothing
    }

    /////////////////////////////////////////////////
    bool DataStatusMask::ConditionSatisfied(
        const DataStatusMask::Condition condition,
        const bool value)
    {
      return ((DataStatusMask::MUST == condition && value)
           || (DataStatusMask::MUST_NOT == condition && !value)
           || (DataStatusMask::EITHER == condition));
    }

    /////////////////////////////////////////////////
    bool DataStatusMask::Satisfied(
        const CompositeData::DataStatus &status) const
    {
      return (ConditionSatisfied(this->exist, status.exists)
           && ConditionSatisfied(this->queried, status.queried)
           && ConditionSatisfied(this->required, status.required));
    }
  }
}

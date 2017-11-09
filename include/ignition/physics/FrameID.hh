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

#ifndef IGNITION_PHYSICS_FRAMEID_HH_
#define IGNITION_PHYSICS_FRAMEID_HH_

#include <memory>

namespace ignition
{
  namespace physics
  {
    // Forward declaration
    class FrameSemanticsBase;

    /////////////////////////////////////////////////
    /// \brief Container for specifying Frame IDs. We do not want to use a
    /// generic integer type for this, because it may lead to bugs where a plain
    /// integer is mistaken for a FrameID. This also allows the compiler to
    /// always perform argument deduction successfully.
    ///
    /// Physics engines may embed a std::shared_ptr which can be used for
    /// reference counting. That way, a FrameID will remain valid for as long as
    /// the user is referencing it. The member function IsReferenceCounted() can
    /// be used to check whether the physics engine is performing reference
    /// counting. The behavior of a frame which has been removed from simulation
    /// but not deleted is undefined.
    class FrameID final
    {
      // Comparison operators
      public: bool operator ==(const FrameID &_other) const;
      public: bool operator <(const FrameID &_other) const;
      public: bool operator >(const FrameID &_other) const;
      public: bool operator <=(const FrameID &_other) const;
      public: bool operator >=(const FrameID &_other) const;
      public: bool operator !=(const FrameID &_other) const;

      /// \brief Get a reference to the world Frame.
      public: static const FrameID &World();

      /// \brief The numerical value of this FrameID's ID number.
      public: std::size_t ID() const;

      /// \brief This will return true if this Frame is being reference-counted,
      /// and false otherwise.
      ///
      /// FrameIDs that are not reference-counted may become invalid before the
      /// end of their lifetime.
      ///
      /// FrameIDs that are reference-counted will remain valid throughout their
      /// lifetime, but if the frames that they correspond to are removed from
      /// the simulation, then the behavior of those frames will depend on the
      /// physics engine implementation. Physics engines are allowed to refuse
      /// to remove any frames which are actively being referenced.
      ///
      /// Note that physics engines may choose to reference count some of the
      /// FrameIDs and not others, so this property must be accounted for per
      /// FrameID. For example, some engines may support reference-counting for
      /// Link frames but not Joint frames. Engine features can be used to
      /// ensure that your physics engine of choice will behave as desired.
      public: bool IsReferenceCounted() const;

      /// \brief Private constructor. To obtain a FrameID, it must be given by
      /// the physics engine. Different physics engines may provide FrameIDs
      /// through different interfaces.
      ///
      /// For physics engine developers: You may generate FrameIDs by calling
      /// the FrameSemantics::SpawnFrameID(~,~) function.
      private: FrameID(
        const std::size_t _id,
        const std::shared_ptr<const void> &_ref);

      // Friendship declaration
      friend class FrameSemanticsBase;

      /// \brief Unique identifying integer that indicates which frame this
      /// FrameID is referencing.
      private: std::size_t id;

      /// \brief Optional reference-counting smart pointer which will guarantee
      /// that the referenced frame remains valid.
      private: std::shared_ptr<const void> ref;
    };
  }
}

#endif

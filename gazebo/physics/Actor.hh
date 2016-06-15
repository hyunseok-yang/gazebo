/*
 * Copyright (C) 2012-2016 Open Source Robotics Foundation
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
#ifndef GAZEBO_PHYSICS_ACTOR_HH_
#define GAZEBO_PHYSICS_ACTOR_HH_

#include <string>
#include <map>
#include <vector>

#include "gazebo/physics/Model.hh"
#include "gazebo/common/Time.hh"
#include "gazebo/common/Animation.hh"
#include "gazebo/util/system.hh"

namespace gazebo
{
  namespace common
  {
    class Mesh;
    class Color;
    class Skeleton;
  }

  namespace physics
  {
    /// \brief Information about a trajectory for an Actor.
    /// This doesn't contain the keyframes information itself.
    class GZ_PHYSICS_VISIBLE TrajectoryInfo
    {
      /// \brief Constructor.
      public: TrajectoryInfo();

      /// \brief ID of the trajectory.
      public: unsigned int id;

      /// \brief Type of trajectory. If it matches the name of a skeleton
      /// animation, they will be played together.
      public: std::string type;

      /// \brief Duration of this keyframe in seconds.
      public: double duration;

      /// \brief Start time of this keyframe within the trajectory, in seconds.
      public: double startTime;

      /// \brief End time of this keyframe within the trajectory, in seconds.
      public: double endTime;

      /// \brief True if the trajectory is translated -- what??.
      /// It's true if this came from waypoints, false if the duration is taken
      /// from the skeleton animation.
      public: bool translated;
    };

    /// \addtogroup gazebo_physics
    /// \{

    /// \class Actor Actor.hh physics/physics.hh
    /// \brief Actor class enables GPU based mesh model / skeleton
    /// scriptable animation.
    class GZ_PHYSICS_VISIBLE Actor : public Model
    {
      /// \brief Typedef the skeleton animation map indexed by their names.
      public: typedef std::map<std::string, common::SkeletonAnimation *>
              SkeletonAnimation_M;

      /// \brief Constructor
      /// \param[in] _parent Parent object
      public: explicit Actor(BasePtr _parent);

      /// \brief Destructor
      public: virtual ~Actor();

      /// \brief Load the actor
      /// \param[in] _sdf SDF parameters
      public: void Load(sdf::ElementPtr _sdf);

      /// \brief Initialize the actor
      public: virtual void Init();

      /// \brief Start playing the script
      public: virtual void Play();

      /// \brief Stop playing the script
      public: virtual void Stop();

      /// \brief Returns true when actor is playing animation
      /// \return True if animation is being played.
      public: virtual bool IsActive() const;

      /// \brief Update the actor
      public: void Update();

      /// \brief Finalize the actor
      public: virtual void Fini();

      /// \brief Update the parameters using new sdf values.
      /// \param[in] _sdf SDF values to update from.
      public: virtual void UpdateParameters(sdf::ElementPtr _sdf);

      /// \brief Get the SDF values for the actor.
      /// \return Pointer to the SDF values.
      public: virtual const sdf::ElementPtr GetSDF();

      /// \brief Set the current script time.
      /// \param[in] _time Time in seconds from the beginning of the current
      /// script loop.
      /// \sa ScriptTime
      public: void SetScriptTime(const double _time);

      /// \brief Get the current script time.
      /// \return _time Time in seconds from the beginning of the current
      /// script loop.
      /// \sa SetScriptTime
      public: double ScriptTime() const;

      /// \brief Returns a dictionary of all the skeleton animations associated
      /// with the actor.
      /// \return A map of SkeletonAnimation, indexed by their name.
      public: const SkeletonAnimation_M &SkeletonAnimations() const;

      /// \brief Set a custom trajectory for the actor. This will override any
      /// trajectories previously defined.
      /// \param[in] _trajInfo Information about custom trajectory.
      public: void SetCustomTrajectory(TrajectoryInfoPtr &_trajInfo);

      /// \brief Reset custom trajectory of the actor.
      public: void ResetCustomTrajectory();

      /// \brief Get whether the links in the actor can collide with each other.
      /// This is always false for actors.
      /// \return False, because actors can't self-collide.
      /// \sa SetSelfCollide()
      public: virtual bool GetSelfCollide() const;

      /// \brief Override set self collide method to make it impossible to set
      /// it to true for actors.
      /// \param [in] _selfCollide Whether this can collide with itself, will be
      /// false for actors regardless of the input.
      /// \sa GetSelfCollide()
      public: virtual void SetSelfCollide(bool _selfCollide);

      /// \brief Add inertia for a sphere.
      /// \param[in] _linkSdf The link to add the inertia to.
      /// \param[in] _pose Pose of the inertia.
      /// \param[in] _mass Mass of the inertia.
      /// \param[in] _radius Radius of the sphere.
      private: void AddSphereInertia(const sdf::ElementPtr &_linkSdf,
                   const ignition::math::Pose3d &_pose,
                   const double _mass, const double _radius);

      /// \brief Add a spherical collision object.
      /// \param[in] _linkSdf Link to add the collision to.
      /// \param[in] _name Name of the collision object.
      /// \param[in] _pose Pose of the collision object.
      /// \param[in] _radius Radius of the collision object.
      private: void AddSphereCollision(const sdf::ElementPtr &_linkSdf,
                   const std::string &_name,
                   const ignition::math::Pose3d &_pose,
                   const double _radius);

      /// \brief Add a spherical visual object.
      /// \param[in] _linkSdf Link to add the visual to.
      /// \param[in] _name Name of the visual object.
      /// \param[in] _pose Pose of the visual object.
      /// \param[in] _radius Radius of the visual object.
      /// \param[in] _material Name of the visual material.
      /// \param[in] _ambient Ambient color.
      private: void AddSphereVisual(const sdf::ElementPtr &_linkSdf,
                   const std::string &_name,
                   const ignition::math::Pose3d &_pose, const double _radius,
                   const std::string &_material, const common::Color &_ambient);

      /// \brief Add a box visual object.
      /// \param[in] _linkSdf Link to add the visual to.
      /// \param[in] _name Name of the visual object.
      /// \param[in] _pose Pose of the visual object.
      /// \param[in] _size Dimensions of the visual object.
      /// \param[in] _material Name of the visual material.
      /// \param[in] _ambient Ambient color.
      private: void AddBoxVisual(const sdf::ElementPtr &_linkSdf,
                   const std::string &_name,
                   const ignition::math::Pose3d &_pose,
                   const ignition::math::Vector3d &_size,
                   const std::string &_material,
                   const common::Color &_ambient);

      /// \brief Add a visual to the given link which holds the actor's skin.
      /// \param[in] _linkSdf Link to add the visual to.
      /// \param[in] _name Name of the visual.
      /// \param[in] _pose Pose of the visual.
      private: void AddActorVisual(const sdf::ElementPtr &_linkSdf,
                   const std::string &_name,
                   const ignition::math::Pose3d &_pose);

      /// \brief Load a skin from SDF. These skins generate skeletons which can
      /// be animated.
      /// \param[in] _sdf SDF element containing the skin.
      /// \sa LoadAnimation
      private: bool LoadSkin(sdf::ElementPtr _sdf);

      /// \brief Load an animation from SDF. These are the animations which
      /// will be applied to the skeletons defined in the skin.
      /// The animation may be described in COLLADA or BVH formats.
      /// \param[in] _sdf SDF element containing the animation.
      /// \sa LoadSkin
      private: void LoadAnimation(sdf::ElementPtr _sdf);

      /// \brief Load a trajectory script from SDF.
      /// \param[in] _sdf SDF element containing the trajectory script.
      private: void LoadScript(sdf::ElementPtr _sdf);

      /// \brief Set the actor's pose, this sets the pose for each bone in the
      /// skeleton and also the actor's pose based on the trajectory.
      /// \param[in] _frame Each frame name and transform.
      /// \param[in] _skelMap Map of bone relationships.
      /// \param[in] _time Time over which to animate the set pose.
      private: void SetPose(
          std::map<std::string, ignition::math::Matrix4d> _frame,
          std::map<std::string, std::string> _skelMap, const double _time);

      /// \brief Pointer to the actor's mesh.
      protected: const common::Mesh *mesh = nullptr;

      /// \brief The actor's skeleton.
      protected: common::Skeleton *skeleton = nullptr;

      /// \brief Filename for the skin.
      protected: std::string skinFile;

      /// \brief Scaling factor to apply to the skin.
      protected: double skinScale;

      /// \brief Amount of time to delay start by.
      protected: double startDelay;

      /// \brief Total time length of the script, in seconds.
      protected: double scriptLength;

      /// \brief True if the animation should loop.
      protected: bool loop;

      /// \brief True if the actor is being updated.
      protected: bool active;

      /// \brief True if the actor should start running automatically.
      /// How does this play with startDelay?
      protected: bool autoStart;

      /// \brief Pointer to the actor's canonical link.
      protected: LinkPtr mainLink;

      /// \brief Time of the previous frame.
      protected: common::Time prevFrameTime;

      /// \brief Time when the animation was started.
      protected: common::Time playStartTime;

      /// \brief Map of all the trajectories (pose animations) and their
      /// indices. The indices here match those within trajInfo.
      /// \sa trajInfo
      protected: std::map<unsigned int, common::PoseAnimation *> trajectories;

      /// \brief A vector of trajectory information, which contains information
      /// such as their durations, uniquely identifiable by their IDs. The IDs
      /// here match those on the `trajectories` vector.
      /// \sa trajectories
      protected: std::vector<TrajectoryInfo> trajInfo;

      /// \brief Map of skeleton animations, indexed by their names. The names
      /// match those in interpolateX and skelNodesMap (?).
      /// \sa interpolateX
      /// \sa skelNodesMap
      protected: SkeletonAnimation_M skelAnimation;

      /// \brief Skeleton to node map:
      /// * Skin filename -- or animName?!
      ///     * Skeleton node names
      ///     * Skeleton animation node names
      /// \sa interpolateX
      /// \sa skelAnimation
      protected: std::map<std::string, std::map<std::string, std::string> >
                                                            skelNodesMap;

      /// \brief Map of animation types (the same name as in skelAnimation and
      /// skelNodesMap) and whether they should be interpolated along x
      // direction. Does false mean to interpolate on Y?
      /// \sa skelAnimation
      /// \sa skelNodesMap
      protected: std::map<std::string, bool> interpolateX;

      /// \brief Last position of the actor.
      protected: ignition::math::Vector3d lastPos;

      /// \brief Length of the actor's path, to be used to find something in the
      /// skeleton animation.
      protected: double pathLength;

      /// \brief Id of the last trajectory
      protected: unsigned int lastTraj;

      /// \brief Name of the visual representing the skin.
      protected: std::string visualName;

      /// \brief ID for the visual representing the skin.
      protected: uint32_t visualId;

      /// \brief Publisher to send bone info.
      protected: transport::PublisherPtr bonePosePub;

      /// \brief Current time within the script, which is the current time minus
      /// the time when the script started.
      private: double scriptTime;

      /// \brief Custom trajectory.
      /// Used to control an actor with a plugin.
      private: TrajectoryInfoPtr customTrajectoryInfo;
    };
    /// \}
  }
}
#endif

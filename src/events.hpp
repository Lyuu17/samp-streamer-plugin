/*
 * Copyright (C) 2017 Incognito
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
 */

#pragma once

#include <Server/Components/Actors/actors.hpp>
#include <Server/Components/Classes/classes.hpp>
#include <Server/Components/Checkpoints/checkpoints.hpp>
#include <Server/Components/Pickups/pickups.hpp>
#include <Server/Components/Objects/objects.hpp>

class PlayerEvents : public PlayerConnectEventHandler, public PlayerSpawnEventHandler, public PlayerShotEventHandler, public Singleton<PlayerEvents> {
    void onPlayerConnect(IPlayer& player) override;

    void onPlayerSpawn(IPlayer& player) override;

    void onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason) override;

    bool onPlayerShotPlayerObject(IPlayer& player, IPlayerObject& target, const PlayerBulletData& bulletData) override;
};

class ActorEvents : public ActorEventHandler, public Singleton<ActorEvents> {
    void onPlayerGiveDamageActor(IPlayer& player, IActor& actor, float amount, unsigned weapon, BodyPart part) override;

    void onActorStreamIn(IActor& actor, IPlayer& forPlayer) override;

    void onActorStreamOut(IActor& actor, IPlayer& forPlayer) override;
};

class ClassEvents : public ClassEventHandler, public Singleton<ClassEvents> {
    bool onPlayerRequestClass(IPlayer& player, unsigned int classId) override;
};

class CheckpointEvents : public PlayerCheckpointEventHandler, public Singleton<CheckpointEvents> {
    void onPlayerEnterCheckpoint(IPlayer& player) override;

    void onPlayerLeaveCheckpoint(IPlayer& player) override;

    void onPlayerEnterRaceCheckpoint(IPlayer& player) override;

    void onPlayerLeaveRaceCheckpoint(IPlayer& player) override;
};

class PickupEvents : public PickupEventHandler, public Singleton<PickupEvents> {
    void onPlayerPickUpPickup(IPlayer& player, IPickup& pickup) override;
};

class ObjectEvents : public ObjectEventHandler, public Singleton<ObjectEvents> {
    void onPlayerObjectEdited(IPlayer& player, IPlayerObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation) override;

    void onPlayerObjectSelected(IPlayer& player, IPlayerObject& object, int model, Vector3 position) override;
};

class EventHandler {
public:
    EventHandler()
    {}

    void Init(IComponentList* components, IPlayerPool* playerPool);

    ~EventHandler();

    void addEvents() const;

private:
    IPlayerPool*           players     = nullptr;
    IPickupsComponent*     pickups     = nullptr;
    IObjectsComponent*     objects     = nullptr;
    ICheckpointsComponent* checkpoints = nullptr;
    IClassesComponent*     classes     = nullptr;
    IActorsComponent*      actors      = nullptr;
};

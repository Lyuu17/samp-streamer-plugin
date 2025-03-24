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

#include "main.h"
#include "core.h"
#include "OmpStreamerComponent.hpp"

#include "events.hpp"

void PlayerEvents::onPlayerConnect(IPlayer& player)
{
    int playerid = player.getID();
    if (playerid >= 0 && playerid < PLAYER_POOL_SIZE)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerid);
        if (p == core->getData()->players.end())
        {
            Player player(playerid);
            core->getData()->players.insert(std::make_pair(playerid, player));
        }
    }
}

void PlayerEvents::onPlayerSpawn(IPlayer& player)
{
    int                                       playerid = player.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        p->second.requestingClass = false;
    }
}

void PlayerEvents::onPlayerDisconnect(IPlayer& player, PeerDisconnectReason reason)
{
    int playerid = player.getID();
    core->getData()->players.erase(playerid);
}

bool PlayerEvents::onPlayerShotPlayerObject(IPlayer& player, IPlayerObject& target, const PlayerBulletData& bulletData)
{
    int                                       playerid = player.getID();
    bool                                      retVal   = true;
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        for (std::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
        {
            if (i->second == bulletData.hitID)
            {
                int objectid = i->first;

                auto& objIter = core->getData()->objects.find(objectid);
                if (objIter != core->getData()->objects.end())
                    OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerShootDynamicObject, player, objIter->second, bulletData);

                for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
                {
                    int  amxIndex  = 0;
                    cell amxRetVal = 0;
                    if (!amx_FindPublic(*a, "OnPlayerShootDynamicObject", &amxIndex))
                    {
                        amx_Push(*a, amx_ftoc(bulletData.offset.z));
                        amx_Push(*a, amx_ftoc(bulletData.offset.y));
                        amx_Push(*a, amx_ftoc(bulletData.offset.x));
                        amx_Push(*a, static_cast<cell>(objectid));
                        amx_Push(*a, static_cast<cell>(bulletData.weapon));
                        amx_Push(*a, static_cast<cell>(playerid));
                        amx_Exec(*a, &amxRetVal, amxIndex);
                        if (!amxRetVal)
                        {
                            retVal = false;
                        }
                    }
                }
                break;
            }
        }
    }
    return retVal;
}

void ActorEvents::onPlayerGiveDamageActor(IPlayer& player, IActor& actor, float amount, unsigned weapon, BodyPart part)
{
    int playerid = player.getID();
    int actorid  = actor.getID();
    for (std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i)
    {
        if (i->second == actorid)
        {
            int dynActorId = i->first.first;

            auto& actorIter = core->getData()->actors.find(dynActorId);
            if (actorIter != core->getData()->actors.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerGiveDamageDynamicActor, player, actorIter->second, amount, weapon, part);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int  amxIndex  = 0;
                cell amxRetVal = 0;
                if (!amx_FindPublic(*a, "OnPlayerGiveDamageDynamicActor", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(part));
                    amx_Push(*a, static_cast<cell>(weapon));
                    amx_Push(*a, amx_ftoc(amount));
                    amx_Push(*a, static_cast<cell>(dynActorId));
                    amx_Push(*a, static_cast<cell>(playerid));
                    amx_Exec(*a, &amxRetVal, amxIndex);
                    if (amxRetVal)
                    {
                        break;
                    }
                }
            }
        }
    }
}

void ActorEvents::onActorStreamIn(IActor& actor, IPlayer& forPlayer)
{
    int actorid     = actor.getID();
    int forplayerid = forPlayer.getID();
    for (std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i)
    {
        if (i->second == actorid)
        {
            int dynActorId = i->first.first;

            auto& actorIter = core->getData()->actors.find(dynActorId);
            if (actorIter != core->getData()->actors.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onDynamicActorStreamIn, forPlayer, actorIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnDynamicActorStreamIn", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(forplayerid));
                    amx_Push(*a, static_cast<cell>(dynActorId));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
            break;
        }
    }
}

void ActorEvents::onActorStreamOut(IActor& actor, IPlayer& forPlayer)
{
    int actorid     = actor.getID();
    int forplayerid = forPlayer.getID();
    for (std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalActors.begin(); i != core->getData()->internalActors.end(); ++i)
    {
        if (i->second == actorid)
        {
            int dynActorId = i->first.first;

            auto& actorIter = core->getData()->actors.find(dynActorId);
            if (actorIter != core->getData()->actors.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onDynamicActorStreamOut, forPlayer, actorIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnDynamicActorStreamOut", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(forplayerid));
                    amx_Push(*a, static_cast<cell>(dynActorId));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
            break;
        }
    }
}

bool ClassEvents::onPlayerRequestClass(IPlayer& player, unsigned int classId)
{
    int                                       playerid = player.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        p->second.requestingClass = true;
    }
    return true;
};

void CheckpointEvents::onPlayerEnterCheckpoint(IPlayer& player)
{
    int                                       playerid = player.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        if (p->second.activeCheckpoint != p->second.visibleCheckpoint)
        {
            int checkpointid           = p->second.visibleCheckpoint;
            p->second.activeCheckpoint = checkpointid;

            auto& cpIter = core->getData()->checkpoints.find(checkpointid);
            if (cpIter != core->getData()->checkpoints.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerEnterDynamicCP, player, cpIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnPlayerEnterDynamicCP", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(checkpointid));
                    amx_Push(*a, static_cast<cell>(playerid));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
        }
    }
}

void CheckpointEvents::onPlayerLeaveCheckpoint(IPlayer& player)
{
    int                                       playerid = player.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        if (p->second.activeCheckpoint == p->second.visibleCheckpoint)
        {
            int checkpointid           = p->second.activeCheckpoint;
            p->second.activeCheckpoint = 0;

            auto& cpIter = core->getData()->checkpoints.find(checkpointid);
            if (cpIter != core->getData()->checkpoints.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerLeaveDynamicCP, player, cpIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnPlayerLeaveDynamicCP", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(checkpointid));
                    amx_Push(*a, static_cast<cell>(playerid));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
        }
    }
}

void CheckpointEvents::onPlayerEnterRaceCheckpoint(IPlayer& player)
{
    int                                       playerid = player.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        if (p->second.activeRaceCheckpoint != p->second.visibleRaceCheckpoint)
        {
            int checkpointid               = p->second.visibleRaceCheckpoint;
            p->second.activeRaceCheckpoint = checkpointid;

            auto& cpIter = core->getData()->raceCheckpoints.find(checkpointid);
            if (cpIter != core->getData()->raceCheckpoints.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerEnterDynamicRaceCP, player, cpIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnPlayerEnterDynamicRaceCP", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(checkpointid));
                    amx_Push(*a, static_cast<cell>(playerid));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
        }
    }
}

void CheckpointEvents::onPlayerLeaveRaceCheckpoint(IPlayer& player)
{
    int                                       playerid = player.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        if (p->second.activeRaceCheckpoint == p->second.visibleRaceCheckpoint)
        {
            int checkpointid               = p->second.activeRaceCheckpoint;
            p->second.activeRaceCheckpoint = 0;

            auto& cpIter = core->getData()->raceCheckpoints.find(checkpointid);
            if (cpIter != core->getData()->raceCheckpoints.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerLeaveDynamicRaceCP, player, cpIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnPlayerLeaveDynamicRaceCP", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(checkpointid));
                    amx_Push(*a, static_cast<cell>(playerid));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
        }
    }
}

void PickupEvents::onPlayerPickUpPickup(IPlayer& player, IPickup& pickup)
{
    int playerid = player.getID();
    int pickupid = pickup.getID();
    for (std::unordered_map<std::pair<int, int>, int, pair_hash>::iterator i = core->getData()->internalPickups.begin(); i != core->getData()->internalPickups.end(); ++i)
    {
        if (i->second == pickupid)
        {
            int dynPickupId = i->first.first;

            auto& pickupIter = core->getData()->pickups.find(dynPickupId);
            if (pickupIter != core->getData()->pickups.end())
                OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerPickUpDynamicPickup, player, pickupIter->second);

            for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
            {
                int amxIndex = 0;
                if (!amx_FindPublic(*a, "OnPlayerPickUpDynamicPickup", &amxIndex))
                {
                    amx_Push(*a, static_cast<cell>(dynPickupId));
                    amx_Push(*a, static_cast<cell>(playerid));
                    amx_Exec(*a, NULL, amxIndex);
                }
            }
            break;
        }
    }
}

void ObjectEvents::onPlayerObjectEdited(IPlayer& player, IPlayerObject& object, ObjectEditResponse response, Vector3 offset, Vector3 rotation)
{
    int                                       playerid = player.getID();
    int                                       objectid = object.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        for (std::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
        {
            if (i->second == objectid)
            {
                int dynObjectId = i->first;
                if (response == ObjectEditResponse::ObjectEditResponse_Cancel || response == ObjectEditResponse::ObjectEditResponse_Final)
                {
                    std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(dynObjectId);
                    if (o != core->getData()->objects.end())
                    {
                        if (o->second->comparableStreamDistance < STREAMER_STATIC_DISTANCE_CUTOFF && o->second->originalComparableStreamDistance > STREAMER_STATIC_DISTANCE_CUTOFF)
                        {
                            o->second->comparableStreamDistance         = o->second->originalComparableStreamDistance;
                            o->second->originalComparableStreamDistance = -1.0f;
                        }
                    }
                }

                auto& objIter = core->getData()->objects.find(dynObjectId);
                if (objIter != core->getData()->objects.end())
                    OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerEditDynamicObject, player, objIter->second, response, offset, rotation);

                for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
                {
                    int  amxIndex  = 0;
                    cell amxRetVal = 0;
                    if (!amx_FindPublic(*a, "OnPlayerEditDynamicObject", &amxIndex))
                    {
                        amx_Push(*a, amx_ftoc(rotation.z));
                        amx_Push(*a, amx_ftoc(rotation.y));
                        amx_Push(*a, amx_ftoc(rotation.x));
                        amx_Push(*a, amx_ftoc(offset.z));
                        amx_Push(*a, amx_ftoc(offset.y));
                        amx_Push(*a, amx_ftoc(offset.x));
                        amx_Push(*a, static_cast<cell>(response));
                        amx_Push(*a, static_cast<cell>(dynObjectId));
                        amx_Push(*a, static_cast<cell>(playerid));
                        amx_Exec(*a, &amxRetVal, amxIndex);
                        if (amxRetVal)
                        {
                            break;
                        }
                    }
                }
                return;
            }
        }
    }
}

void ObjectEvents::onPlayerObjectSelected(IPlayer& player, IPlayerObject& object, int model, Vector3 position)
{
    int                                       playerid = player.getID();
    int                                       objectid = object.getID();
    std::unordered_map<int, Player>::iterator p        = core->getData()->players.find(playerid);
    if (p != core->getData()->players.end())
    {
        for (std::unordered_map<int, int>::iterator i = p->second.internalObjects.begin(); i != p->second.internalObjects.end(); ++i)
        {
            if (i->second == objectid)
            {
                int dynObjectId = i->first;

                auto& objIter = core->getData()->objects.find(dynObjectId);
                if (objIter != core->getData()->objects.end())
                    OmpStreamerComponent::instance->eventDispatcher.dispatch(&streamer::StreamerEventHandler::onPlayerSelectDynamicObject, player, objIter->second);

                for (std::set<AMX*>::iterator a = core->getData()->interfaces.begin(); a != core->getData()->interfaces.end(); ++a)
                {
                    int  amxIndex  = 0;
                    cell amxRetVal = 0;
                    if (!amx_FindPublic(*a, "OnPlayerSelectDynamicObject", &amxIndex))
                    {
                        amx_Push(*a, amx_ftoc(position.z));
                        amx_Push(*a, amx_ftoc(position.y));
                        amx_Push(*a, amx_ftoc(position.x));
                        amx_Push(*a, static_cast<cell>(model));
                        amx_Push(*a, static_cast<cell>(dynObjectId));
                        amx_Push(*a, static_cast<cell>(playerid));
                        amx_Exec(*a, &amxRetVal, amxIndex);
                        if (amxRetVal)
                        {
                            break;
                        }
                    }
                }
                return;
            }
        }
    }
}

void EventHandler::Init(IComponentList* components, IPlayerPool* playerPool)
{
    players     = playerPool;
    actors      = components->queryComponent<IActorsComponent>();
    objects     = components->queryComponent<IObjectsComponent>();
    pickups     = components->queryComponent<IPickupsComponent>();
    checkpoints = components->queryComponent<ICheckpointsComponent>();
    classes     = components->queryComponent<IClassesComponent>();
}

EventHandler::~EventHandler()
{
    if (players)
    {
        players->getPlayerConnectDispatcher().removeEventHandler(PlayerEvents::Get());
        players->getPlayerSpawnDispatcher().removeEventHandler(PlayerEvents::Get());
        players->getPlayerShotDispatcher().removeEventHandler(PlayerEvents::Get());
    }

    if (actors)
    {
        actors->getEventDispatcher().removeEventHandler(ActorEvents::Get());
    }

    if (objects)
    {
        objects->getEventDispatcher().removeEventHandler(ObjectEvents::Get());
    }

    if (pickups)
    {
        pickups->getEventDispatcher().removeEventHandler(PickupEvents::Get());
    }

    if (classes)
    {
        classes->getEventDispatcher().removeEventHandler(ClassEvents::Get());
    }

    if (checkpoints)
    {
        checkpoints->getEventDispatcher().removeEventHandler(CheckpointEvents::Get());
    }
}

void EventHandler::addEvents() const
{
    if (players)
    {
        players->getPlayerConnectDispatcher().addEventHandler(PlayerEvents::Get());
        players->getPlayerSpawnDispatcher().addEventHandler(PlayerEvents::Get());
        players->getPlayerShotDispatcher().addEventHandler(PlayerEvents::Get());
    }

    if (actors)
    {
        actors->getEventDispatcher().addEventHandler(ActorEvents::Get());
    }

    if (objects)
    {
        objects->getEventDispatcher().addEventHandler(ObjectEvents::Get());
    }

    if (pickups)
    {
        pickups->getEventDispatcher().addEventHandler(PickupEvents::Get());
    }

    if (classes)
    {
        classes->getEventDispatcher().addEventHandler(ClassEvents::Get());
    }

    if (checkpoints)
    {
        checkpoints->getEventDispatcher().addEventHandler(CheckpointEvents::Get());
    }
}

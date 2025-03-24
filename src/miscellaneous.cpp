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

#include "miscellaneous.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::miscellaneous
{
    std::optional<float> GetDistanceToItem(const Eigen::Vector3f& position, StreamerItemType itemType, int id, int dimensions)
    {
        float           distance = 0;
        Eigen::Vector3f itemPos  = Eigen::Vector3f::Zero();
        bool            success  = false;
        switch (itemType)
        {
        case STREAMER_TYPE_OBJECT: {
            std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
            if (o != core->getData()->objects.end())
            {
                if (o->second->attach)
                {
                    itemPos = o->second->attach->position;
                }
                else
                {
                    itemPos = o->second->position;
                }
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_PICKUP: {
            std::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
            if (p != core->getData()->pickups.end())
            {
                itemPos = p->second->position;
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_CP: {
            std::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
            if (c != core->getData()->checkpoints.end())
            {
                itemPos = c->second->position;
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_RACE_CP: {
            std::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
            if (r != core->getData()->raceCheckpoints.end())
            {
                itemPos = r->second->position;
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_MAP_ICON: {
            std::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
            if (m != core->getData()->mapIcons.end())
            {
                itemPos = m->second->position;
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_3D_TEXT_LABEL: {
            std::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
            if (t != core->getData()->textLabels.end())
            {
                if (t->second->attach)
                {
                    itemPos = t->second->attach->position;
                }
                else
                {
                    itemPos = t->second->position;
                }
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_AREA: {
            std::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
            if (a != core->getData()->areas.end())
            {
                std::variant<Polygon2d, Box2d, Box3d, Eigen::Vector2f, Eigen::Vector3f> areaPosition;
                if (a->second->attach)
                {
                    areaPosition = a->second->attach->position;
                }
                else
                {
                    areaPosition = a->second->position;
                }
                switch (a->second->type)
                {
                case STREAMER_AREA_TYPE_CIRCLE:
                case STREAMER_AREA_TYPE_CYLINDER: {
                    float distance = 0.0f;
                    distance       = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(position[0], position[1]), std::get<Eigen::Vector2f>(areaPosition)));
                    return true;
                }
                case STREAMER_AREA_TYPE_SPHERE: {
                    itemPos = std::get<Eigen::Vector3f>(areaPosition);
                    success = true;
                    break;
                }
                case STREAMER_AREA_TYPE_RECTANGLE: {
                    Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(std::get<Box2d>(areaPosition));
                    distance                 = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(position[0], position[1]), centroid));
                    return true;
                }
                case STREAMER_AREA_TYPE_CUBOID: {
                    itemPos = boost::geometry::return_centroid<Eigen::Vector3f>(std::get<Box3d>(areaPosition));
                    success = true;
                    break;
                }
                case STREAMER_AREA_TYPE_POLYGON: {
                    Eigen::Vector2f centroid = boost::geometry::return_centroid<Eigen::Vector2f>(std::get<Polygon2d>(areaPosition));
                    distance                 = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(position[0], position[1]), centroid));
                    return true;
                }
                }
            }
            if (success)
            {
                break;
            }
            return std::nullopt;
        }
        case STREAMER_TYPE_ACTOR: {
            std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
            if (a != core->getData()->actors.end())
            {
                itemPos = a->second->position;
                break;
            }
            return std::nullopt;
        }
        default: {
            Utility::logError("Streamer_GetDistanceToItem: Invalid type specified.");
            return std::nullopt;
        }
        }
        switch (dimensions)
        {
        case 2: {
            distance = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(position[0], position[1]), Eigen::Vector2f(itemPos[0], itemPos[1])));
            break;
        }
        case 3: {
            distance = static_cast<float>(boost::geometry::distance(Eigen::Vector3f(position[0], position[1], position[2]), itemPos));
            break;
        }
        default: {
            Utility::logError("Streamer_GetDistanceToItem: Invalid number of dimensions specified (outside range of 2-3).");
            return std::nullopt;
        }
        }
        return distance;
    }

    bool ToggleItem(int playerId, StreamerItemType itemType, int id, bool toggle)
    {
        switch (itemType)
        {
        case STREAMER_TYPE_OBJECT: {
            std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
            if (o != core->getData()->objects.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(o->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(o->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_PICKUP: {
            std::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
            if (p != core->getData()->pickups.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(p->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(p->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_CP: {
            std::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
            if (c != core->getData()->checkpoints.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(c->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(c->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_RACE_CP: {
            std::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
            if (r != core->getData()->raceCheckpoints.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(r->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(r->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_MAP_ICON: {
            std::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
            if (m != core->getData()->mapIcons.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(m->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(m->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_3D_TEXT_LABEL: {
            std::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
            if (t != core->getData()->textLabels.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(t->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(t->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_AREA: {
            std::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
            if (a != core->getData()->areas.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(a->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(a->second->players, playerId);
                }
            }
            break;
        }
        case STREAMER_TYPE_ACTOR: {
            std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
            if (a != core->getData()->actors.end())
            {
                if (!toggle)
                {
                    return Utility::removeFromContainer(a->second->players, playerId);
                }
                else
                {
                    return Utility::addToContainer(a->second->players, playerId);
                }
            }
            break;
        }
        default: {
            Utility::logError("Streamer_ToggleItem: Invalid type specified.");
            return false;
        }
        }
        return true;
    }

    bool IsToggleItem(int playerId, StreamerItemType itemType, int id)
    {
        switch (itemType)
        {
        case STREAMER_TYPE_OBJECT: {
            std::unordered_map<int, Item::SharedObject>::iterator o = core->getData()->objects.find(id);
            if (o != core->getData()->objects.end())
            {
                return Utility::isInContainer(o->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_PICKUP: {
            std::unordered_map<int, Item::SharedPickup>::iterator p = core->getData()->pickups.find(id);
            if (p != core->getData()->pickups.end())
            {
                return Utility::isInContainer(p->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_CP: {
            std::unordered_map<int, Item::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(id);
            if (c != core->getData()->checkpoints.end())
            {
                return Utility::isInContainer(c->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_RACE_CP: {
            std::unordered_map<int, Item::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(id);
            if (r != core->getData()->raceCheckpoints.end())
            {
                return Utility::isInContainer(r->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_MAP_ICON: {
            std::unordered_map<int, Item::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(id);
            if (m != core->getData()->mapIcons.end())
            {
                return Utility::isInContainer(m->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_3D_TEXT_LABEL: {
            std::unordered_map<int, Item::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
            if (t != core->getData()->textLabels.end())
            {
                return Utility::isInContainer(t->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_AREA: {
            std::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(id);
            if (a != core->getData()->areas.end())
            {
                return Utility::isInContainer(a->second->players, playerId);
            }
            break;
        }
        case STREAMER_TYPE_ACTOR: {
            std::unordered_map<int, Item::SharedActor>::iterator a = core->getData()->actors.find(id);
            if (a != core->getData()->actors.end())
            {
                return Utility::isInContainer(a->second->players, playerId);
            }
            break;
        }
        default: {
            Utility::logError("Streamer_IsToggleItem: Invalid type specified.");
            return false;
        }
        }
        return true;
    }
} // namespace streamer::miscellaneous

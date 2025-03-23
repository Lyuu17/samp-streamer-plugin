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

#include "text-labels.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::areas
{
    Identifier Area::identifier;

    Area::Area()
        : references(0) {}
    Area::Attach::Attach()
        : references(0) {}

    int Area::getID() const
    {
        return areaId;
    }

    Item::SharedArea CreateDynamicCircle(std::optional<AMX*> amx, const Eigen::Vector2f& position, float size, int worldId, int interiorId, int playerId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx            = amx;
        area->areaId         = areaId;
        area->type           = STREAMER_AREA_TYPE_CIRCLE;
        area->position       = position;
        area->comparableSize = size * size;
        area->size           = size;
        Utility::addToContainer(area->worlds, worldId);
        Utility::addToContainer(area->interiors, interiorId);
        Utility::addToContainer(area->players, playerId);
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicCylinder(std::optional<AMX*> amx, const Eigen::Vector2f& position, const Eigen::Vector2f& height, float size, int worldId, int interiorId, int playerId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx            = amx;
        area->areaId         = areaId;
        area->type           = STREAMER_AREA_TYPE_CYLINDER;
        area->position       = position;
        area->height         = height;
        area->comparableSize = size * size;
        area->size           = size;
        Utility::addToContainer(area->worlds, worldId);
        Utility::addToContainer(area->interiors, interiorId);
        Utility::addToContainer(area->players, playerId);
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicSphere(std::optional<AMX*> amx, const Eigen::Vector3f& position, float size, int worldId, int interiorId, int playerId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx            = amx;
        area->areaId         = areaId;
        area->spectateMode   = true;
        area->type           = STREAMER_AREA_TYPE_SPHERE;
        area->position       = position;
        area->comparableSize = size * size;
        area->size           = size;
        Utility::addToContainer(area->worlds, worldId);
        Utility::addToContainer(area->interiors, interiorId);
        Utility::addToContainer(area->players, playerId);
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicRectangle(std::optional<AMX*> amx, const Eigen::Vector2f& minposition, const Eigen::Vector2f& maxposition, int worldId, int interiorId, int playerId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx          = amx;
        area->areaId       = areaId;
        area->spectateMode = true;
        area->type         = STREAMER_AREA_TYPE_RECTANGLE;
        area->position     = Box2d(minposition, maxposition);
        boost::geometry::correct(std::get<Box2d>(area->position));
        area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(std::get<Box2d>(area->position).min_corner(), std::get<Box2d>(area->position).max_corner()));
        area->size           = static_cast<float>(boost::geometry::distance(std::get<Box2d>(area->position).min_corner(), std::get<Box2d>(area->position).max_corner()));
        Utility::addToContainer(area->worlds, worldId);
        Utility::addToContainer(area->interiors, interiorId);
        Utility::addToContainer(area->players, playerId);
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicCuboid(std::optional<AMX*> amx, const Eigen::Vector3f& minposition, const Eigen::Vector3f& maxposition, int worldId, int interiorId, int playerId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx          = amx;
        area->areaId       = areaId;
        area->spectateMode = true;
        area->type         = STREAMER_AREA_TYPE_CUBOID;
        area->position     = Box3d(minposition, maxposition);
        boost::geometry::correct(std::get<Box3d>(area->position));
        area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(std::get<Box3d>(area->position).min_corner()[0], std::get<Box3d>(area->position).min_corner()[1]), Eigen::Vector2f(std::get<Box3d>(area->position).max_corner()[0], std::get<Box3d>(area->position).max_corner()[1])));
        area->size           = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(std::get<Box3d>(area->position).min_corner()[0], std::get<Box3d>(area->position).min_corner()[1]), Eigen::Vector2f(std::get<Box3d>(area->position).max_corner()[0], std::get<Box3d>(area->position).max_corner()[1])));
        Utility::addToContainer(area->worlds, worldId);
        Utility::addToContainer(area->interiors, interiorId);
        Utility::addToContainer(area->players, playerId);
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicPolygon(std::optional<AMX*> amx, const std::vector<Eigen::Vector2f>& points, const Eigen::Vector2f& height, int worldId, int interiorId, int playerId, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        if (points.size() < 6)
        {
            Utility::logError("CreateDynamicPolygon: Number of points must be bigger or equal to 6.");
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx          = amx;
        area->areaId       = areaId;
        area->spectateMode = true;
        area->type         = STREAMER_AREA_TYPE_POLYGON;

        auto& polygon = std::get<Polygon2d>(area->position);

        boost::geometry::assign_points(polygon, points);
        boost::geometry::correct(polygon);

        area->height         = height;
        Box2d box            = boost::geometry::return_envelope<Box2d>(std::get<Polygon2d>(area->position));
        area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
        area->size           = static_cast<float>(boost::geometry::distance(box.min_corner(), box.max_corner()));
        Utility::addToContainer(area->worlds, worldId);
        Utility::addToContainer(area->interiors, interiorId);
        Utility::addToContainer(area->players, playerId);
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicCircleEx(std::optional<AMX*> amx, const Eigen::Vector2f& position, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx            = amx;
        area->areaId         = areaId;
        area->spectateMode   = true;
        area->type           = STREAMER_AREA_TYPE_CIRCLE;
        area->position       = position;
        area->comparableSize = size * size;
        area->size           = size;
        area->worlds.insert(worlds.begin(), worlds.end());
        area->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            area->players.set(player);
        }
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicCylinderEx(std::optional<AMX*> amx, const Eigen::Vector2f& position, const Eigen::Vector2f& height, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx            = amx;
        area->areaId         = areaId;
        area->spectateMode   = true;
        area->type           = STREAMER_AREA_TYPE_CYLINDER;
        area->position       = position;
        area->height         = height;
        area->comparableSize = size * size;
        area->size           = size;
        area->worlds.insert(worlds.begin(), worlds.end());
        area->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            area->players.set(player);
        }
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicSphereEx(std::optional<AMX*> amx, const Eigen::Vector3f& position, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx            = amx;
        area->areaId         = areaId;
        area->spectateMode   = true;
        area->type           = STREAMER_AREA_TYPE_SPHERE;
        area->position       = position;
        area->comparableSize = size * size;
        area->size           = size;
        area->worlds.insert(worlds.begin(), worlds.end());
        area->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            area->players.set(player);
        }
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicRectangleEx(std::optional<AMX*> amx, const Eigen::Vector2f& minposition, const Eigen::Vector2f& maxposition, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx          = amx;
        area->areaId       = areaId;
        area->spectateMode = true;
        area->type         = STREAMER_AREA_TYPE_RECTANGLE;
        area->position     = Box2d(minposition, maxposition);
        boost::geometry::correct(std::get<Box2d>(area->position));
        area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(std::get<Box2d>(area->position).min_corner(), std::get<Box2d>(area->position).max_corner()));
        area->size           = static_cast<float>(boost::geometry::distance(std::get<Box2d>(area->position).min_corner(), std::get<Box2d>(area->position).max_corner()));
        area->worlds.insert(worlds.begin(), worlds.end());
        area->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            area->players.set(player);
        }
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicCuboidEx(std::optional<AMX*> amx, const Eigen::Vector3f& minposition, const Eigen::Vector3f& maxposition, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx          = amx;
        area->areaId       = areaId;
        area->spectateMode = true;
        area->type         = STREAMER_AREA_TYPE_CUBOID;
        area->position     = Box3d(minposition, maxposition);
        boost::geometry::correct(std::get<Box3d>(area->position));
        area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(Eigen::Vector2f(std::get<Box3d>(area->position).min_corner()[0], std::get<Box3d>(area->position).min_corner()[1]), Eigen::Vector2f(std::get<Box3d>(area->position).max_corner()[0], std::get<Box3d>(area->position).max_corner()[1])));
        area->size           = static_cast<float>(boost::geometry::distance(Eigen::Vector2f(std::get<Box3d>(area->position).min_corner()[0], std::get<Box3d>(area->position).min_corner()[1]), Eigen::Vector2f(std::get<Box3d>(area->position).max_corner()[0], std::get<Box3d>(area->position).max_corner()[1])));
        area->worlds.insert(worlds.begin(), worlds.end());
        area->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            area->players.set(player);
        }
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea CreateDynamicPolygonEx(std::optional<AMX*> amx, const std::vector<Eigen::Vector2f>& points, const Eigen::Vector2f& height, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority)
    {
        if (core->getData()->getGlobalMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
        {
            return nullptr;
        }
        if (points.size() < 6)
        {
            Utility::logError("CreateDynamicPolygonEx: Number of points must be bigger or equal to 6.");
            return nullptr;
        }
        int              areaId = Area::identifier.get();
        Item::SharedArea area(new Area);
        area->amx          = amx;
        area->areaId       = areaId;
        area->spectateMode = true;
        area->type         = STREAMER_AREA_TYPE_POLYGON;
        auto& polygon      = std::get<Polygon2d>(area->position);

        boost::geometry::assign_points(polygon, points);
        boost::geometry::correct(polygon);

        area->height         = height;
        Box2d box            = boost::geometry::return_envelope<Box2d>(std::get<Polygon2d>(area->position));
        area->comparableSize = static_cast<float>(boost::geometry::comparable_distance(box.min_corner(), box.max_corner()));
        area->size           = static_cast<float>(boost::geometry::distance(box.min_corner(), box.max_corner()));
        area->worlds.insert(worlds.begin(), worlds.end());
        area->interiors.insert(interiors.begin(), interiors.end());
        for (int player : players)
        {
            area->players.set(player);
        }
        area->priority = priority;
        core->getGrid()->addArea(area);
        core->getData()->areas.insert(std::make_pair(areaId, area));
        return area;
    }

    Item::SharedArea GetDynamicArea(int areaId)
    {
        std::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaId);
        if (a != core->getData()->areas.end())
        {
            return a->second;
        }
        return nullptr;
    }

    bool DestroyDynamicArea(int areaId)
    {
        Utility::executeFinalAreaCallbacks(areaId);
        std::unordered_map<int, Item::SharedArea>::iterator a = core->getData()->areas.find(areaId);
        if (a != core->getData()->areas.end())
        {
            Utility::destroyArea(a);
            return true;
        }
        return false;
    }
} // namespace streamer::areas

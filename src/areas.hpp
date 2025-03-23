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

namespace streamer::areas
{
    struct Area : public IArea
    {
        Area();

        virtual int getID() const override;

        std::optional<AMX*> amx;
        int                 areaId;
        SharedCell          cell;
        float               comparableSize;
        Eigen::Vector2f     height;
        int                 priority;
        int                 references;
        float               size;
        bool                spectateMode;
        int                 type;

        std::variant<Polygon2d, Box2d, Box3d, Eigen::Vector2f, Eigen::Vector3f> position;

        struct Attach
        {
            Attach();

            Eigen::Vector2f                                                         height;
            std::tuple<int, int, int>                                               object;
            int                                                                     player;
            std::variant<Polygon2d, Box2d, Box3d, Eigen::Vector2f, Eigen::Vector3f> position;
            Eigen::Vector3f                                                         positionOffset;
            int                                                                     references;
            int                                                                     vehicle;

            EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        };

        std::shared_ptr<Attach> attach;

        std::unordered_set<int>                   areas;
        std::vector<int>                          extras;
        std::unordered_map<int, std::vector<int>> extraExtras;
        std::unordered_set<int>                   interiors;
        std::bitset<PLAYER_POOL_SIZE>             players;
        std::unordered_set<int>                   worlds;

        static Identifier identifier;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    Item::SharedArea CreateDynamicCircle(std::optional<AMX*> amx, const Eigen::Vector2f& position, float size, int worldId, int interiorId, int playerId, int priority);
    Item::SharedArea CreateDynamicCylinder(std::optional<AMX*> amx, const Eigen::Vector2f& position, const Eigen::Vector2f& height, float size, int worldId, int interiorId, int playerId, int priority);
    Item::SharedArea CreateDynamicSphere(std::optional<AMX*> amx, const Eigen::Vector3f& position, float size, int worldId, int interiorId, int playerId, int priority);
    Item::SharedArea CreateDynamicRectangle(std::optional<AMX*> amx, const Eigen::Vector2f& minposition, const Eigen::Vector2f& maxposition, int worldId, int interiorId, int playerId, int priority);
    Item::SharedArea CreateDynamicCuboid(std::optional<AMX*> amx, const Eigen::Vector3f& minposition, const Eigen::Vector3f& maxposition, int worldId, int interiorId, int playerId, int priority);
    Item::SharedArea CreateDynamicPolygon(std::optional<AMX*> amx, const std::vector<Eigen::Vector2f>& points, const Eigen::Vector2f& height, int worldId, int interiorId, int playerId, int priority);

    Item::SharedArea CreateDynamicCircleEx(std::optional<AMX*> amx, const Eigen::Vector2f& position, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority);
    Item::SharedArea CreateDynamicCylinderEx(std::optional<AMX*> amx, const Eigen::Vector2f& position, const Eigen::Vector2f& height, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority);
    Item::SharedArea CreateDynamicSphereEx(std::optional<AMX*> amx, const Eigen::Vector3f& position, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority);
    Item::SharedArea CreateDynamicRectangleEx(std::optional<AMX*> amx, const Eigen::Vector2f& minposition, const Eigen::Vector2f& maxposition, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority);
    Item::SharedArea CreateDynamicCuboidEx(std::optional<AMX*> amx, const Eigen::Vector3f& minposition, const Eigen::Vector3f& maxposition, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority);
    Item::SharedArea CreateDynamicPolygonEx(std::optional<AMX*> amx, const std::vector<Eigen::Vector2f>& points, const Eigen::Vector2f& height, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority);

    Item::SharedArea GetDynamicArea(int areaId);
    bool             DestroyDynamicArea(int areaId);
} // namespace streamer::areas

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

#include "../main.h"

#include "../natives.h"
#include "../core.h"
#include "../utility.h"
#include "../objects.hpp"
#include "../map-icons.hpp"

cell AMX_NATIVE_CALL Natives::CreateDynamicObjectEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(18);

    int modelId = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };
    Eigen::Vector3f rotation { amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]) };

    float streamDistance = amx_ctof(params[8]);
    float drawDistance   = amx_ctof(params[9]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[10], params[15], worlds);
    Utility::convertArrayToContainer(amx, params[11], params[16], interiors);
    Utility::convertArrayToContainer(amx, params[12], params[17], players);
    Utility::convertArrayToContainer(amx, params[13], params[18], areas);

    int priority = static_cast<int>(params[14]);

    auto object = streamer::objects::CreateDynamicObjectEx(amx, modelId, position, rotation, streamDistance, drawDistance, worlds, interiors, players, areas, priority);
    if (!object) return INVALID_STREAMER_ID;
    return static_cast<cell>(object->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicPickupEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(15);

    int modelId = static_cast<int>(params[1]);
    int type    = static_cast<int>(params[2]);

    Eigen::Vector3f position { amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]) };

    float streamDistance = amx_ctof(params[6]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[7], params[12], worlds);
    Utility::convertArrayToContainer(amx, params[8], params[13], interiors);
    Utility::convertArrayToContainer(amx, params[9], params[14], players);
    Utility::convertArrayToContainer(amx, params[10], params[15], areas);

    int priority = static_cast<int>(params[11]);

    auto pickup = streamer::pickups::CreateDynamicPickupEx(amx, modelId, type, position, streamDistance, worlds, interiors, players, areas, priority);
    if (pickup == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(pickup->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCPEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(14);

    Eigen::Vector3f position { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };

    float size = amx_ctof(params[4]);
    float streamDistance = amx_ctof(params[5]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[6], params[11], worlds);
    Utility::convertArrayToContainer(amx, params[7], params[12], interiors);
    Utility::convertArrayToContainer(amx, params[8], params[13], players);
    Utility::convertArrayToContainer(amx, params[9], params[14], areas);

    int priority = static_cast<int>(params[10]);

    auto checkpoint = streamer::checkpoints::CreateDynamicCheckpointEx(amx, position, size, streamDistance, worlds, interiors, players, areas, priority);
    if (checkpoint == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(checkpoint->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicRaceCPEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(18);

    int type = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };
    Eigen::Vector3f next { amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]) };

    float size           = amx_ctof(params[8]);
    float streamDistance = amx_ctof(params[9]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[10], params[15], worlds);
    Utility::convertArrayToContainer(amx, params[11], params[16], interiors);
    Utility::convertArrayToContainer(amx, params[12], params[17], players);
    Utility::convertArrayToContainer(amx, params[13], params[18], areas);

    int priority = static_cast<int>(params[14]);

    auto raceCheckpoint = streamer::racecheckpoints::CreateDynamicRaceCheckpointEx(amx, type, position, next, size, streamDistance, worlds, interiors, players, areas, priority);
    if (raceCheckpoint == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(raceCheckpoint->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicMapIconEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(16);
    Eigen::Vector3f position { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };

    int   type           = static_cast<int>(params[4]);
    int   color          = static_cast<int>(params[5]);
    int   style          = static_cast<int>(params[6]);
    float streamDistance = amx_ctof(params[7]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[8], params[13], worlds);
    Utility::convertArrayToContainer(amx, params[9], params[14], interiors);
    Utility::convertArrayToContainer(amx, params[10], params[15], players);
    Utility::convertArrayToContainer(amx, params[11], params[16], areas);

    int priority = static_cast<int>(params[12]);

    auto mapIcon = streamer::mapicons::CreateDynamicMapIconEx(amx, position, type, color, style, streamDistance, worlds, interiors, players, areas, priority);
    if (mapIcon == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(mapIcon->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamic3DTextLabelEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(19);

    auto text = Utility::convertNativeStringToString(amx, params[1]);

    int color = static_cast<int>(params[2]);

    Eigen::Vector3f position { amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]) };

    float drawDistance      = amx_ctof(params[6]);
    int   attachedPlayerId  = static_cast<int>(params[7]);
    int   attachedVehicleId = static_cast<int>(params[8]);
    bool  testLOS           = static_cast<int>(params[9]) != 0;
    int   worldId           = static_cast<int>(params[10]);
    float streamDistance    = amx_ctof(params[11]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[11], params[16], worlds);
    Utility::convertArrayToContainer(amx, params[12], params[17], interiors);
    Utility::convertArrayToContainer(amx, params[13], params[18], players);
    Utility::convertArrayToContainer(amx, params[14], params[19], areas);

    int priority = static_cast<int>(params[15]);

    auto textlabel = streamer::textlabels::CreateDynamicTextLabelEx(amx, text, color, position, drawDistance, attachedPlayerId, attachedVehicleId, testLOS, streamDistance, worlds, interiors, players, areas, priority);
    if (textlabel == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(textlabel->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCircleEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(10);

    Eigen::Vector2f position { amx_ctof(params[1]), amx_ctof(params[2]) };

    float size = amx_ctof(params[3]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[4], params[8], worlds);
    Utility::convertArrayToContainer(amx, params[5], params[9], interiors);
    Utility::convertArrayToContainer(amx, params[6], params[10], players);

    int priority = static_cast<int>(params[7]);

    auto area = streamer::areas::CreateDynamicCircleEx(amx, position, size, worlds, interiors, players, priority);
    if (area == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(area->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCylinderEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(12);
    Eigen::Vector2f position { amx_ctof(params[1]), amx_ctof(params[2]) };
    Eigen::Vector2f height { amx_ctof(params[3]), amx_ctof(params[4]) };
    float           size = amx_ctof(params[5]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[6], params[10], worlds);
    Utility::convertArrayToContainer(amx, params[7], params[11], interiors);
    Utility::convertArrayToContainer(amx, params[8], params[12], players);

    int priority = static_cast<int>(params[9]);

    auto area = streamer::areas::CreateDynamicCylinderEx(amx, position, height, size, worlds, interiors, players, priority);
    if (area == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(area->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicSphereEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(11);

    Eigen::Vector3f position { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };
    float           size = amx_ctof(params[4]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[5], params[9], worlds);
    Utility::convertArrayToContainer(amx, params[6], params[10], interiors);
    Utility::convertArrayToContainer(amx, params[7], params[11], players);

    int priority = static_cast<int>(params[8]);

    auto area = streamer::areas::CreateDynamicSphereEx(amx, position, size, worlds, interiors, players, priority);
    if (area == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(area->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicRectangleEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(11);
    Eigen::Vector2f minposition { amx_ctof(params[1]), amx_ctof(params[2]) };
    Eigen::Vector2f maxposition { amx_ctof(params[3]), amx_ctof(params[4]) };

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[5], params[9], worlds);
    Utility::convertArrayToContainer(amx, params[6], params[10], interiors);
    Utility::convertArrayToContainer(amx, params[7], params[11], players);

    int priority = static_cast<int>(params[8]);

    auto area = streamer::areas::CreateDynamicRectangleEx(amx, minposition, maxposition, worlds, interiors, players, priority);
    if (area == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(area->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicCuboidEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(13);

    Eigen::Vector3f minposition { amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]) };
    Eigen::Vector3f maxposition { amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]) };

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[7], params[11], worlds);
    Utility::convertArrayToContainer(amx, params[8], params[12], interiors);
    Utility::convertArrayToContainer(amx, params[9], params[13], players);

    int priority = static_cast<int>(params[10]);

    auto area = streamer::areas::CreateDynamicCuboidEx(amx, minposition, maxposition, worlds, interiors, players, priority);
    if (area == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(area->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicPolygonEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(11);

    int pointsSize = static_cast<int>(params[4]);
    if (pointsSize < 6 || pointsSize % 2)
    {
        Utility::logError("CreateDynamicPolygon: Number of points must be divisible by 2 and bigger or equal to 6.");
        return INVALID_STREAMER_ID;
    }

    std::vector<Eigen::Vector2f> points;
    cell*                        array = NULL;
    amx_GetAddr(amx, params[1], &array);
    for (std::size_t i = 0; i < static_cast<std::size_t>(pointsSize); i += 2)
        points.push_back(Eigen::Vector2f(amx_ctof(array[i]), amx_ctof(array[i + 1])));

    Eigen::Vector2f height { amx_ctof(params[2]), amx_ctof(params[3]) };

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[5], params[9], worlds);
    Utility::convertArrayToContainer(amx, params[6], params[10], interiors);
    Utility::convertArrayToContainer(amx, params[7], params[11], players);

    int priority = static_cast<int>(params[8]);

    auto area = streamer::areas::CreateDynamicPolygonEx(amx, points, height, worlds, interiors, players, priority);
    if (area == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(area->getID());
}

cell AMX_NATIVE_CALL Natives::CreateDynamicActorEx(AMX* amx, cell* params)
{
    CHECK_PARAMS(17);

    int modelId = static_cast<int>(params[1]);

    Eigen::Vector3f position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };

    float rotation       = amx_ctof(params[5]);
    bool  invulnerable   = static_cast<int>(params[6]) != 0;
    float health         = amx_ctof(params[7]);
    float streamDistance = amx_ctof(params[8]);

    std::unordered_set<int> worlds;
    std::unordered_set<int> interiors;
    std::unordered_set<int> players;
    std::unordered_set<int> areas;
    Utility::convertArrayToContainer(amx, params[9], params[14], worlds);
    Utility::convertArrayToContainer(amx, params[10], params[15], interiors);
    Utility::convertArrayToContainer(amx, params[11], params[16], players);
    Utility::convertArrayToContainer(amx, params[12], params[17], areas);

    int priority = static_cast<int>(params[13]);

    auto actor = streamer::actors::CreateDynamicActorEx(amx, modelId, position, rotation, invulnerable, health, streamDistance, worlds, interiors, players, areas, priority);
    if (actor == nullptr) return INVALID_STREAMER_ID;
    return static_cast<cell>(actor->getID());
}

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

#include "updates.hpp"

#include "main.h"
#include "natives.h"
#include "core.h"
#include "utility.h"

namespace streamer::updates
{
    bool Update(int playerId, StreamerItemType type)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerId);
        if (p != core->getData()->players.end())
        {
            p->second.interiorId = ompgdk::GetPlayerInterior(p->first);
            p->second.worldId    = ompgdk::GetPlayerVirtualWorld(p->first);
            ompgdk::GetPlayerPos(p->first, &p->second.position[0], &p->second.position[1], &p->second.position[2]);
            core->getStreamer()->startManualUpdate(p->second, type);
            return true;
        }
        return false;
    }

    bool UpdateEx(int playerId, const Eigen::Vector3f& position, std::optional<int> worldId, std::optional<int> interiorId, StreamerItemType type, std::optional<int> compensatedTime, bool freezePlayer)
    {
        std::unordered_map<int, Player>::iterator p = core->getData()->players.find(playerId);
        if (p != core->getData()->players.end())
        {
            p->second.position   = position;
            p->second.worldId    = worldId.value_or(ompgdk::GetPlayerVirtualWorld(p->first));
            p->second.interiorId = interiorId.value_or(ompgdk::GetPlayerInterior(p->first));
            if (compensatedTime.has_value())
            {
                ompgdk::SetPlayerPos(p->first, p->second.position[0], p->second.position[1], p->second.position[2]);
                if (freezePlayer)
                {
                    ompgdk::TogglePlayerControllable(p->first, false);
                }
                p->second.delayedUpdate       = true;
                p->second.delayedUpdateType   = type;
                p->second.delayedUpdateTime   = std::chrono::steady_clock::now() + std::chrono::milliseconds(compensatedTime.value());
                p->second.delayedUpdateFreeze = freezePlayer;
            }
            core->getStreamer()->startManualUpdate(p->second, type);
            return true;
        }
        return false;
    }
} // namespace streamer::updates

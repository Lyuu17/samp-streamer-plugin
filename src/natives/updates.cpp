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

#include "../updates.hpp"

cell AMX_NATIVE_CALL Natives::Streamer_ProcessActiveItems(AMX *amx, cell *params)
{
	core->getStreamer()->processActiveItems();
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_ToggleIdleUpdate(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		p->second.updateWhenIdle = static_cast<int>(params[2]) != 0;
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_IsToggleIdleUpdate(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		return static_cast<cell>(p->second.updateWhenIdle != 0);
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_ToggleCameraUpdate(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		p->second.updateUsingCameraPosition = static_cast<int>(params[2]) != 0;
		return 1;
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_IsToggleCameraUpdate(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		return static_cast<cell>(p->second.updateUsingCameraPosition != 0);
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_ToggleItemUpdate(AMX *amx, cell *params)
{
	CHECK_PARAMS(3);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		if (static_cast<int>(params[2]) >= 0 && static_cast<int>(params[2]) < STREAMER_MAX_TYPES)
		{
			p->second.enabledItems.set(static_cast<size_t>(params[2]), params[3] != 0);
			return 1;
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_IsToggleItemUpdate(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
	std::unordered_map<int, Player>::iterator p = core->getData()->players.find(static_cast<int>(params[1]));
	if (p != core->getData()->players.end())
	{
		if (static_cast<int>(params[2]) >= 0 && static_cast<int>(params[2]) < STREAMER_MAX_TYPES)
		{
			return static_cast<cell>(p->second.enabledItems.test(params[2]) != 0);
		}
	}
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_GetLastUpdateTime(AMX *amx, cell *params)
{
	CHECK_PARAMS(1);
	Utility::storeFloatInNative(amx, params[1], core->getStreamer()->getLastUpdateTime());
	return 1;
}

cell AMX_NATIVE_CALL Natives::Streamer_Update(AMX *amx, cell *params)
{
	CHECK_PARAMS(2);
    if (streamer::updates::Update(static_cast<int>(params[1]), static_cast<StreamerItemType>(params[2]))) return 1;
	return 0;
}

cell AMX_NATIVE_CALL Natives::Streamer_UpdateEx(AMX *amx, cell *params)
{
	CHECK_PARAMS(9);

    int                playerId = static_cast<int>(params[1]);
    Eigen::Vector3f    position { amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]) };
    std::optional<int> worldId           = static_cast<int>(params[5]) >= 0 ? std::optional<int>(static_cast<int>(params[5])) : std::nullopt;
    std::optional<int> interiorId        = static_cast<int>(params[6]) >= 0 ? std::optional<int>(static_cast<int>(params[6])) : std::nullopt;
    int                type              = static_cast<int>(params[7]);
    std::optional<int> compensatedTime   = static_cast<int>(params[8]) >= 0 ? std::optional<int>(static_cast<int>(params[8])) : std::nullopt;
    bool               freezePlayer      = static_cast<int>(params[9]) != 0;
    if (streamer::updates::UpdateEx(playerId, position, worldId, interiorId, (StreamerItemType)type, compensatedTime, freezePlayer)) return 1;
	return 0;
}

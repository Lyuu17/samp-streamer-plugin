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

namespace streamer::updates
{
    bool Update(int playerId, StreamerItemType type);
    bool UpdateEx(int playerId, const Eigen::Vector3f& position, std::optional<int> worldId, std::optional<int> interiorId, StreamerItemType type, std::optional<int> compensatedTime, bool freezePlayer);
} // namespace streamer::updates

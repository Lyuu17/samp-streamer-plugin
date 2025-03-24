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

namespace streamer::miscellaneous
{
    std::optional<float> GetDistanceToItem(const Eigen::Vector3f& position, StreamerItemType itemType, int id, int dimensions = 3);
    bool                 ToggleItem(int playerId, StreamerItemType itemType, int id, bool toggle);
    bool                 IsToggleItem(int playerId, StreamerItemType itemType, int id);

} // namespace streamer::miscellaneous

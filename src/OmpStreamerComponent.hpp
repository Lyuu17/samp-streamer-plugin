
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
#include "events.hpp"

#include "streamer/StreamerComponent.hpp"

#include <Server/Components/Pawn/pawn.hpp>
#include <Impl/events_impl.hpp>

struct OmpStreamerComponent : public IOmpStreamerComponent,
                              public CoreEventHandler,
                              public PawnEventHandler,
                              public SingleNetworkOutEventHandler
{
    inline static OmpStreamerComponent* instance = nullptr;

    ICore*          omp_core      = nullptr;
    IPlayerPool*    players       = nullptr;
    IPawnComponent* pawnComponent = nullptr;
    EventHandler    streamerEventHandler;

    Impl::DefaultEventDispatcher<streamer::StreamerEventHandler> eventDispatcher;

    StringView      componentName() const override;
    SemanticVersion componentVersion() const override;
    void            onLoad(ICore* c) override;
    void            onInit(IComponentList* components) override;
    void            onReady() override;
    void            onTick(Microseconds elapsed, TimePoint now) override;
    void            onAmxLoad(IPawnScript& script) override;
    void            onAmxUnload(IPawnScript& script) override;
    void            onFree(IComponent* component) override;
    void            reset() override;
    void            free() override;
    bool            onSend(IPlayer* peer, NetworkBitStream& bs) override;
    ~OmpStreamerComponent() override;

    IEventDispatcher<streamer::StreamerEventHandler>& getEventDispatcher() override;

    // IOmpStreamerComponent virtuals
    std::shared_ptr<streamer::IActor>          getDynamicActor(int actorId) override;
    std::shared_ptr<streamer::IActor>          createDynamicActor(int modelId, const Vector3& position, float rotation, bool invulnerable, float health, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority) override;
    std::shared_ptr<streamer::IActor>          createDynamicActorEx(int modelId, const Vector3& position, float rotation, bool invulnerable, float health, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicActor(int actorId) override;
    bool                                       isDynamicActorStreamedIn(int actorId, int playerId) override;
    std::shared_ptr<streamer::IActor>          getPlayerTargetDynamicActor(int playerId) override;
    std::shared_ptr<streamer::IActor>          getPlayerCameraTargetDynActor(int playerId) override;
    std::shared_ptr<streamer::IArea>           createDynamicCircle(const Vector2& position, float size, int worldId, int interiorId, int playerId, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicCylinder(const Vector2& position, const Vector2& height, float size, int worldId, int interiorId, int playerId, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicSphere(const Vector3& position, float size, int worldId, int interiorId, int playerId, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicRectangle(const Vector2& minposition, const Vector2& maxposition, int worldId, int interiorId, int playerId, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicCuboid(const Vector3& minposition, const Vector3& maxposition, int worldId, int interiorId, int playerId, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicPolygon(const std::vector<Vector2>& points, const Vector2& height, int worldId, int interiorId, int playerId, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicCircleEx(const Vector2& position, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicCylinderEx(const Vector2& position, const Vector2& height, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicSphereEx(const Vector3& position, float size, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicRectangleEx(const Vector2& minposition, const Vector2& maxposition, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicCuboidEx(const Vector3& minposition, const Vector3& maxposition, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority) override;
    std::shared_ptr<streamer::IArea>           createDynamicPolygonEx(const std::vector<Vector2>& points, const Vector2& height, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, int priority) override;
    std::shared_ptr<streamer::IArea>           getDynamicArea(int areaId) override;
    bool                                       destroyDynamicArea(int areaId) override;
    std::shared_ptr<streamer::ICheckpoint>     getDynamicCheckpoint(int checkpointId) override;
    std::shared_ptr<streamer::ICheckpoint>     createDynamicCheckpoint(const Vector3& position, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority) override;
    std::shared_ptr<streamer::ICheckpoint>     createDynamicCheckpointEx(const Vector3& position, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicCheckpoint(int checkpointId) override;
    std::shared_ptr<streamer::IMapIcon>        getDynamicMapIcon(int mapIconId) override;
    std::shared_ptr<streamer::IMapIcon>        createDynamicMapIcon(int modelId, const Vector3& position, int type, int color, int worldId, int interiorId, int playerId, float streamDistance, MapIconStyle style, int areaId, int priority) override;
    std::shared_ptr<streamer::IMapIcon>        createDynamicMapIconEx(int modelId, const Vector3& position, int type, int color, int style, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicMapIcon(int mapIconId) override;
    std::shared_ptr<streamer::IObject>         getDynamicObject(int objectId) override;
    std::shared_ptr<streamer::IObject>         createDynamicObject(int modelId, const Vector3& position, const Vector3& rotation, int worldId, int interiorId, int playerId, float streamDistance, float drawDistance, int areaId, int priority) override;
    std::shared_ptr<streamer::IObject>         createDynamicObjectEx(int modelId, const Vector3& position, const Vector3& rotation, float streamDistance, float drawDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicObject(int objectId) override;
    std::shared_ptr<streamer::IObject>         getPlayerCameraTargetDynObject(int playerId) override;
    std::shared_ptr<streamer::IPickup>         getDynamicPickup(int pickupId) override;
    std::shared_ptr<streamer::IPickup>         createDynamicPickup(int modelId, int type, const Vector3& position, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority) override;
    std::shared_ptr<streamer::IPickup>         createDynamicPickupEx(int modelId, int type, const Vector3& position, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicPickup(int pickupId) override;
    std::shared_ptr<streamer::IRaceCheckpoint> getDynamicRaceCheckpoint(int raceCheckpointId) override;
    std::shared_ptr<streamer::IRaceCheckpoint> createDynamicRaceCheckpoint(int type, const Vector3& position, const Vector3& next, float size, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority) override;
    std::shared_ptr<streamer::IRaceCheckpoint> createDynamicRaceCheckpointEx(int type, const Vector3& position, const Vector3& next, float size, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicRaceCheckpoint(int raceCheckpointId) override;
    std::shared_ptr<streamer::ITextLabel>      getDynamicTextLabel(int textlabelId) override;
    std::shared_ptr<streamer::ITextLabel>      createDynamicTextLabel(const std::string& text, int color, const Vector3& position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, int worldId, int interiorId, int playerId, float streamDistance, int areaId, int priority) override;
    std::shared_ptr<streamer::ITextLabel>      createDynamicTextLabelEx(const std::string& text, int color, const Vector3& position, float drawDistance, int attachedPlayerId, int attachedVehicleId, bool testLOS, float streamDistance, const std::unordered_set<int>& worlds, const std::unordered_set<int>& interiors, const std::unordered_set<int>& players, const std::unordered_set<int>& areas, int priority) override;
    bool                                       destroyDynamicTextLabel(int textlabelId) override;
    std::optional<float>                       getDistanceToItem(const Vector3& position, StreamerItemType itemType, int id, int dimensions) override;
    bool                                       toggleItem(int playerId, StreamerItemType itemType, int id, bool toggle) override;
    bool                                       isToggleItem(int playerId, StreamerItemType itemType, int id) override;
    bool                                       update(int playerId, StreamerItemType itemType) override;
    bool                                       updateEx(int playerId, const Vector3& position, std::optional<int> worldId, std::optional<int> interiorId, StreamerItemType itemType, std::optional<int> compensatedTime, bool freezePlayer) override;
};

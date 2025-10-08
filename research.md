## Networking Layer Plan

### Goals
- **Authoritative server**: Prevent cheating, ensure consistent world state.
- **Low-latency gameplay**: Prioritize real-time inputs and state updates.
- **Scalable sessions**: Support singleplayer, LAN, and internet multiplayer.
- **Engine-first design**: Clean API integrated with ECS and Systems.

### Architecture
- **Topology**: Client-Server (authoritative server). Optional listen-server for quick demos.
- **Transport**: UDP-based with reliability layer (e.g., ENet-style channels) or QUIC. TCP only for control/metadata if needed.
- **Message Framing**: Binary messages with header (protocol version, msg type, seq, timestamp).
- **Serialization**: Protobuf or FlatBuffers. Keep hot-path messages minimal and fixed-size where possible.
- **Threading**: Dedicated networking thread per process; lock-free queues to/from main thread.
- **Platforms**: Abstract `INetworkTransport` with implementations for desktop; allow stubs/mocks for tests.

### Core Concepts
- **Sessions/Rooms**: Server hosts `Session` objects; clients join via `sessionId`.
- **Connections**: State machine (Connecting, Handshaking, Active, Disconnecting). Heartbeats and timeouts.
- **Channels/QoS**:
  - Reliable-ordered: joins, RPCs, inventory changes, entity spawns/despawns.
  - Unreliable: input samples, state deltas, snapshots.
  - Reliable-unordered: asset manifests, chunked transfers.
- **Tick Model**: Server fixed tick (e.g., 60Hz). Messages carry `serverTick` for reconciliation.

### ECS Integration
- **NetEntityId**: Stable network id separate from local entity handles.
- **Components**: Opt-in `Replicated` tag; optional `Prediction` marker.
- **Replication**: Server builds per-client interest-filtered snapshots/deltas of component subsets.
- **Authority**: Server writes authoritative state; client writes predicted components locally.

### Client-Side Techniques
- **Client Prediction**: Predict local player movement; buffer inputs with sequence numbers.
- **Server Reconciliation**: On authoritative state, rewind to last acked tick, reapply pending inputs.
- **Interpolation/Extrapolation**: Interpolate remote entities using buffered snapshots; moderate extrapolation for short gaps.

### Server-Side Techniques
- **Lag Compensation**: For hitscan/server validation, rewind to client `fireTime` within bounds.
- **Interest Management**: Spatial partitioning (grid/quadtree) and relevance rules to reduce bandwidth.
- **Rate Limiting**: Per-connection budgets for bytes/tick and messages/tick.

### Reliability, Ordering, and Flow Control
- **Sequence/Ack**: Per-channel sequence numbers; ack bitfields for fast retransmit.
- **Fragmentation/MTU**: Detect MTU, fragment large reliable messages, reassemble with timeouts.
- **Clock Sync**: Lightweight NTP-style RTT/offset estimation; expose `getNetworkTime()`.

### Protocol and Message Schema (Illustrative)
- `Hello`, `Challenge`, `Auth`, `JoinSession`, `LeaveSession`
- `InputSample{seq, dt, axes/buttons}`
- `StateDelta{serverTick, entities:[id, changedComponents...]}`
- `Snapshot{serverTick, entities:[id, fullState...]}`
- `RPC{target, methodId, payload}`
- `Pong/Ping`, `Disconnect{reason}`

### Security and Integrity
- **Authentication**: Token-based (opaque session token). Optional guest mode.
- **Cheat Mitigation**: Authoritative checks, sanity validation on inputs, max speeds/teleport caps.
- **Encryption**: DTLS or QUIC if available; otherwise optional libsodium box per-connection.
- **DoS Protections**: Stateless cookie during handshake, IP rate limiting, proof-of-work toggle for public servers.

### API Surface (Engine)
- `NetworkSystem` (engine system): lifecycle, tick integration, dispatch.
- `INetworkTransport`: send/receive datagrams, time, connection events.
- `NetServer`, `NetClient`: high-level roles, session management.
- `Replicator`: builds deltas/snapshots from ECS, interest filtering.
- `PredictionManager`: client reconciliation and smoothing.
- `RpcRegistry`: register callable functions with ids; route calls.
- `Serializer`: encode/decode with chosen schema.

### Configuration
- Protocol version, tick rate, snapshot rate, send/recv budgets, interpolation delay.
- Interest rules, reliability timeouts, max entities per packet.

### Observability
- Counters: RTT, packet loss, goodput, resend rate, snapshot sizes, interest cull ratios.
- Logs: connection lifecycle, drops, fragmentation, unusual input patterns.
- Debug UI overlay: network graphs; server console commands.

### Testing Strategy
- Unit: serializer roundtrips, ack/seq logic, fragmentation.
- Deterministic sims: lockstep harness with scripted RTT/loss/jitter profiles.
- Soak tests: bots moving/shooting; measure stability and memory.
- Replay: record input and snapshots, deterministic replays for repros.

### Migration/Compatibility
- Schema versioning in headers; support minor backward compatibility window.
- Feature flags for experimental channels or message types.

### Performance Targets (initial)
- < 80 bytes avg per entity delta per snapshot for typical scenes.
- 60Hz server tick; 20–30Hz snapshot send rate; < 120ms playable RTT.

### Roadmap
1) Prototype transport + message framing (loopback, no encryption).
2) Add reliable/unreliable channels, seq/ack, heartbeats, time sync.
3) Basic client prediction + reconciliation for character movement.
4) Snapshot/delta replication with interest management.
5) RPC system and server commands.
6) Security hardening (auth, encryption), rate limiting.
7) Observability and tools (overlay, counters, captures).
8) QUIC transport option and listen-server mode.

### Open Questions
- QUIC vs ENet-style UDP for first implementation?
- Protobuf vs FlatBuffers vs custom bitpacking for hot paths?
- Minimum supported platforms and NAT traversal requirements?



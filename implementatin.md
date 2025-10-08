## Networking Implementation Plan (WinSock from scratch)

### Scope and Constraints
• Transport: UDP via WinSock2 from scratch; custom reliability/ordering.
• OS: Windows 10+ desktop. IPv4 first, IPv6 later.
• No external net libs: Build our own framing, acks, retransmit, fragmentation.

### Milestones
1) Bootstrap WinSock and minimal loopback echo (UDP).
2) Non-blocking sockets + basic send/recv API + graceful shutdown.
3) Connection model (pseudo-connection over UDP): handshakes, heartbeats, timeouts.
4) Channels/QoS: unreliable, reliable-ordered, reliable-unordered.
5) Sequencing, ack bitfields, retransmission, congestion/back-pressure.
6) Message framing, MTU discovery, fragmentation/reassembly.
7) Time sync (RTT/offset), tick stamping, clock smoothing.
8) Client prediction & server reconciliation hooks (movement only).
9) Replication scaffolding (snapshot/delta) and interest filtering.
10) Observability: counters, logs, debug overlay; soak tests.
11) Performance pass; optional IOCP migration.

### Threading Model (phase 1)
• One dedicated networking thread per process.
• Lock-free MPSC queues between main thread and net thread:
  – toNetworkQueue: game→net messages (outgoing).
  – fromNetworkQueue: net→game events (incoming).
• Non-blocking sockets with WSAPoll initially; IOCP in phase 2.

### WinSock Setup
• Call WSAStartup(MAKEWORD(2, 2), &wsaData) on init; WSACleanup() on shutdown.
• Create UDP socket: socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP).
• Set non-blocking: u_long mode = 1; ioctlsocket(sock, FIONBIO, &mode).
• Set buffer sizes via setsockopt (SO_RCVBUF/SO_SNDBUF), enable SIO_UDP_CONNRESET workaround.
• Bind for server; for client, bind ephemeral and remember server endpoint.

### Optional IO Model (phase 2)
• IOCP: CreateIoCompletionPort, OVERLAPPED, WSARecvFrom/WSASendTo posting.
• Pool of IO worker threads sized to cores.
• Maintain per-connection endpoint state keyed by (addr, port).

### Protocol Framing
• Fixed header (little-endian):
  – u16 protocolVersion
  – u8 messageType
  – u8 channelId
  – u32 seq
  – u32 ack (last received on this channel)
  – u32 ackBits (bitmap of previous 32)
  – u32 serverTick
  – u32 timestamp (client local send time)
• Payload: one or more frames (TLV or simple concatenation with length fields).
• Keep total payload under discovered MTU minus IP/UDP headers.

### Channels and Reliability
• Channel 0: unreliable (inputs, state deltas)
• Channel 1: reliable-ordered (spawns, inventory, RPC)
• Channel 2: reliable-unordered (assets, control)
• Per-channel sequence numbers, ack/ackBits logic.
• Retransmit queue for reliable channels with per-packet send time and retry policy.

### Connection State Machine (UDP pseudo-connection)
• States: Idle → SendingHello → AwaitingChallenge → Active → Disconnecting.
• Heartbeat: send keepalive every N ticks; drop on timeout.
• Stateless cookie in handshake to mitigate spoofing.

### MTU, Fragmentation, and Reassembly
• Start with conservative payload (e.g., 1200 bytes).
• Probe up to discovered path MTU with DF toggles if possible.
• Fragment large reliable payloads; reassemble with timeouts and max in-flight limits.

### Time Sync
• Ping/Pong with timestamp reflection; compute RTT and clock offset.
• Expose getNetworkTime() with smoothed offset.

### Public Engine API (C++)
• class INetworkTransport
  – bool initialize(const TransportConfig&)
  – void shutdown()
  – bool send(const Endpoint&, const ByteSpan&, ChannelId)
  – bool pollReceived(Packet&, int maxPackets)
• class NetClient
  – bool connect(const Endpoint&, const ClientConfig&)
  – void disconnect(DisconnectReason)
  – void enqueueInput(const InputSample&)
• class NetServer
  – bool listen(const Endpoint&, const ServerConfig&)
  – void broadcast(const Message&, ChannelId)
  – void sendTo(ClientId, const Message&, ChannelId)
• class Replicator
  – buildSnapshot(deltaMode); applySnapshot(...)
• class PredictionManager
  – pushLocalInput(...); reconcile(authoritativeState, ackSeq)

### Data Structures
• Endpoint{ sockaddr_storage addr; int len; }
• Packet{ Header hdr; std::vector<uint8_t> payload; }
• SendQueue{ per-channel ring buffers, retransmit heap by deadline }
• RecvState{ per-channel expectedSeq, reassembly maps }
• Lock-free queues: reusable ring buffers with sequence cursors.

### Serialization
• Start with custom binary writer/reader (fixed endianness, aligned primitives, varints for lengths).
• Later: bit-packing for hot paths.

### Integration with Engine
• NetworkSystem tick order: input capture → enqueue → net tick → update prediction → renderer.
• ECS: NetEntityId mapping; Replicated tag; component serializers registered at startup.

### Testing
• Unit tests: header encode/decode, ack/seq logic, retransmit policy, fragmentation.
• Loopback tests: client/server in-process with artificial RTT/loss.
• Soak: bots move/shoot for hours; monitor leaks and counters.

### Diagnostics
• Counters: sent/received packets, loss, RTT, resend rate, bandwidth.
• Logs: connection lifecycle, handshake, timeouts, unusual packet sizes.
• Debug UI overlay hooks for graphs.

### Security
• Handshake cookie; rate limiting per IP; sanity checks on payload sizes and frequencies.
• Optional XOR/AEAD later; validate state transitions server-side.

### Migration Path to IOCP (optional)
• Replace WSAPoll loop with IOCP.
• Per-socket binding to completion port; pre-post WSARecvFrom buffers.
• Lock-free freelists for OVERLAPPED structures and buffers.

### Deliverables per Milestone
• M1: NetClient/NetServer create sockets, send/recv echo over loopback.
• M2: Non-blocking, clean shutdown, config for buffers and timeouts.
• M3: Handshake, heartbeats, disconnect reasons; counters for RTT/loss.
• M4: Reliable channels with acks; retransmit and ordering tests.
• M5: Fragmentation, MTU probe; big message tests.
• M6: Time sync; expose getNetworkTime(); tick stamps in headers.
• M7: Prediction/reconciliation demo controlling a character.
• M8: Snapshot/delta production and application; interest filtering stub.
• M9: Debug overlay, logs, soak tests.
• M10: IOCP backend (optional) parity with WSAPoll.

### Risks & Mitigations
• Head-of-line blocking in reliable channel → separate channels, careful batching.
• Timer granularity and sleep jitter → use QueryPerformanceCounter for timing.
• Buffer thrash → ring buffers, object pools, contiguous storage.



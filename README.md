# idk_game

The game runs an authoritative server for both singleplayer and multiplayer.
The graphical client sends input and renders snapshots; it does not simulate
player physics. The default model demo and compiled-level mode both use this
path, built on the engine's `RemoteRxTxer` networking layer.

## Build and launch

Build from the sibling `idk-engine` checkout inside `steamrt4`:

```bash
./scripts/env.sh
./scripts/build.sh --cmakeopts -DIDK_BUILD_EDITOR_TESTS=ON
```

With the default debug preset, executables and content install into
`idk-engine/build-debug/idkge/`. Keep `game_server`, `content/`, and `engine.cfg`
beside `game`. A custom `IDK_APP_NAME` also renames the server to `<name>_server`.

```bash
cd build-debug/idkge

# Singleplayer: starts a private, separate server process over loopback.
./game
./game --level content/level/playground.idklvl

# Dedicated multiplayer server, in another terminal or on another machine.
./game_server --port 28080 --level content/level/playground.idklvl

# Each client has the same compiled levels installed locally.
./game --connect 127.0.0.1 --port 28080 --level content/level/playground.idklvl
```

Replace `127.0.0.1` with the server hostname for remote multiplayer. Omit
`--level` on both server and clients for the replicated model demo. `game --server`
also enters the dedicated loop without constructing `Engine` or initializing
video/audio. `game_server` additionally has no renderer or Steamworks link
dependency. Its default binding accepts connections on all interfaces; use
`--bind 127.0.0.1` for an explicitly local dedicated server.

Normal singleplayer starts its child before engine/SDL initialization. It waits
for a readiness pipe and connects to the port actually selected by that child.
The private server tries ports starting at 28080, allowing multiple local games;
`--port N` requests that exact port and fails if occupied. It never silently
joins an existing server. The parent stops and reaps the child on normal exit
or startup failure; Linux parent-death signalling also stops it if the parent
crashes or is killed. Quitting a remote client leaves the dedicated server running.

## Controls and levels

For levels, click to capture the mouse; use WASD to move, Shift to run, Space to
jump, Tab to release the mouse, R to restart your player, and Escape to close.
The demo accepts WASD movement without mouse capture and keeps model rotation
on the server. Unfocused clients send neutral movement. Inputs are sent at up
to 60 Hz, with jump/restart taps retained until the next send.

The server uses the existing Jolt `Player` and `PlayerController`, including
collision, acceleration, jumping and fall respawn. Remote players appear as
blue boxes. All players share the current level: when any player enters its
exit, the server transitions everyone and respawns them at the new entry. An
empty destination marks completion for everyone; R resets your player and
clears the completion marker. Respawns allocate new entity generations, so
clients never interpolate through a teleport or confuse a reused slot with an
old player. Late joiners receive the current level, players and completion state.

Exit destinations remain compiled paths relative to installed `content`, such
as `level/finish.idklvl`. `--level-root /path/to/content` changes that root; pass
it on the server and each client when using external levels. Absolute initial
level paths are supported, as required by mapedit's **Build & Play**. The
launcher resolves paths before the engine changes working directory.

Startup walks the linked level chain (at most 16 unique levels, including
cycles), validates it, and hashes file content plus the game protocol version.
Missing destinations or paths escaping the level root fail startup. Clients
with different content are rejected. Paths are never accepted from network
packets: replicated scene resource IDs resolve through each process's local
catalog. Changing level files during a session requires a restart. A failed
server transition retains the active scene; leave and re-enter the exit to retry.
The 64-bit content fingerprint is a compatibility check, not authentication.

## Timing and automation

`--config path.cfg` selects timing settings, otherwise `engine.cfg` beside the
executable is used. Server simulation uses `fixed_hz` and `physics_hz`; client
input polling/rendering uses its own configuration. F5 reloads the client and
forwards changed simulation rates to its private server. For a separate
server, send `SIGHUP` to request a reload. Invalid server reloads retain current
settings. Remote clients cannot change server timing.

`--frames N` exits a graphical client after N rendered frames **after joining
and receiving its player**, so it cannot pass a load test while still connecting.
It works for both the demo and levels. `--ticks N` limits dedicated server ticks.
Connection rejection, timeout, rendering failure, invalid config and missing
content produce nonzero exits. A graphical client exits if its server is lost;
relaunch to reconnect.

The engine still limits sessions to 8 clients and 16 replicated entities. This
game uses at most 10 (8 players, one scene, one completion marker). Movement is
interpolated from authoritative snapshots without prediction, so latency is
visible. Input is latest-state UDP: brief button actions can still be lost in
transit, and this is not a reliable transaction/RPC system. Player-vs-player
collision, authentication, encryption and NAT traversal are not implemented.
Models, terrain and GPU handles stay local; only stable resource IDs and state
cross the wire. App-owned session/catalog storage is bounded; Jolt and SDL_net
retain their existing internal allocations.

## Checks

```bash
ctest --test-dir build-debug/cmake --output-on-failure
```

`idk_game_network_tests` covers two-player authoritative movement, jump edges,
respawn generations, shared level transitions, completion, late joining,
disconnect cleanup, content mismatch/change detection and demo movement.
`idk_game_process_tests` runs the actual server and two offscreen graphical
clients for both modes, checks occupied-port failures, and verifies private
server cleanup after successful play and renderer initialization failure.
`idk_game_local_demo` is a short offscreen startup/render/shutdown check. Tests
need local UDP access and OpenGL 4.6 through the offscreen SDL driver.

See [engine networking](../idk-engine/code/network/README.md) and
[engine timing](../idk-engine/code/engine/README.md#timing-configuration) for the
underlying APIs and settings.

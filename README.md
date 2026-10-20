# idk_game

Build and install from the sibling `IDKGameEngine` checkout inside `steamrt4`
with `./scripts/build.sh`.
Running `./build-debug/game` opens the existing model demo.

Edit the `[timing]` section of `build-debug/engine.cfg` to set `fixed_hz`,
`input_hz`, `physics_hz`, `render_hz`, and `vsync`. Press **F5** to reload
settings while running; edits are not applied automatically. No rebuild is
needed, and builds preserve your configuration edits. Use `--config path.cfg`
to select another file. Invalid reloads keep the current settings.
See [engine timing configuration](../IDKGameEngine/code/engine/README.md#timing-configuration)
for defaults, supported ranges, and the C++ API.

To play a compiled level:

```bash
./build-debug/game --level build-debug/content/level/playground.idklvl
```

Click to capture the mouse; use WASD to move, Shift to run, Space to jump, Tab
to release the mouse, R to restart, and Escape to close. The game starts at the
entry marker. Entering the orange exit volume loads its configured next level
in the same process; an empty destination reports completion. The player and
engine services persist, with the player reset at the new entry. A failed
transition keeps the current level running; leave and re-enter the exit to retry.

Exit destinations are compiled paths relative to installed `content`, such as
`level/finish.idklvl`. Use `--level-root /path/to/levels` with `--level` to change
this root for external levels. The installed playground leads to the finish
example. Build destination artifacts before playing.

`mapedit` launches this same mode through **Build & Play**. `mapc` compiles
source `.idkmap` files into runtime `.idklvl` artifacts. See the engine's
[level pipeline documentation](../IDKGameEngine/code/level/README.md) for full usage.

For an automated graphics/load check, add `--frames 10`. This flag requires
`--level`; malformed levels and initialization failures return a nonzero code.

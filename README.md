# idk_game

Build and install from the workspace root inside `steamrt4` with `./build.sh`.
Running `./build-debug/game` opens the existing model demo.

To play a compiled level:

```bash
./build-debug/game --level build-debug/content/level/playground.idklvl
```

Click to capture the mouse; use WASD to move, Shift to run, Space to jump, Tab
to release the mouse, R to restart, and Escape to close. The game starts at the
entry marker and reports when the player reaches the orange exit volume.

`mapedit` launches this same mode through **Build & Play**. `mapc` compiles
source `.idkmap` files into runtime `.idklvl` artifacts. See the engine's
[level pipeline documentation](../idk_engine/level/README.md) for full usage.

For an automated graphics/load check, add `--frames 10`. This flag requires
`--level`; malformed levels and initialization failures return a nonzero code.

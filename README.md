# VEX Pushback — Team 86X Robot Code

Competition code for **VEX team 86X**'s robot in the **Pushback** season, written in C++ on the [PROS](https://pros.cs.purdue.edu/) kernel. This is an archived past-season repository — my current-season code as captain of 8632X lives in [RCFOverride2027](https://github.com/Telfair8632/RCFOverride2027).

## What's in here

| Path | Contents |
|---|---|
| `src/auto/` | 5 competition autonomous routines — red/blue left/right and programming skills |
| `src/motorcontrol/` | X-drive `chassis`, `roller` scoring control, and the shared `kernel` |
| `src/display/` | On-screen logger used for driver-side debugging |
| `include/haws/` | The personal code framework I carried across VEX seasons: hardware config, motor-control modules, logging |

Drivetrain motion builds on [LemLib](https://github.com/LemLib/LemLib) for odometry and path following, integrated with the team's own motor-control layer.

## Building

Requires the [PROS CLI](https://pros.cs.purdue.edu/v5/getting-started/):

```bash
pros make      # build
pros upload    # flash to the brain
```

## License

MIT — see [LICENSE](LICENSE). © 2026 Team 86X members.

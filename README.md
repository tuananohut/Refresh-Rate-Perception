# Refresh Rate Perception

![sim.gif](sim.gif)

This is a small simulation to explore refresh rate perception.

The main question is: can the human eye perceive differences beyond the visible frame rate?

This program renders at a low real frame rate (e.g., 30 FPS) while simulating physics at a much higher internal update rate (e.g., 480 Hz). The physics steps are visualized to show how motion is calculated in between frames.

The goal is not scientific accuracy, but to experiment with frame pacing, temporal resolution, and visual perception.

## Key Features

- Fixed Timestep Physics: Physics runs at a high frequency (Target 480Hz) decoupled from the rendering framerate.
- Sub-stepping & Iterative Solver: Uses multiple solver iterations per frame to handle collisions and stacking stability, replacing complex CCD with a high-frequency discrete approach.
- Particle Collision: Basic circle-to-circle collision resolution.
- Screen Bounds: Particles bounce off screen edges with velocity checks to prevent sticking.
- UI & Controls: - `F`: Toggle UI (shows current/max particle count and physics FPS).
  - `Left Mouse`: Spawn particles.

## Implementation Details

The simulation uses an accumulator approach for time management. Even if the display only updates 30 times a second, the physics engine calculates positions and collisions 480 times a second (or more), ensuring deterministic and stable physical interactions regardless of the visual framerate.

## Based on

- Tsoding's "subframes" project: [GitHub](https://github.com/tsoding/subframes)

## Acknowledgements

- [Subframes video by Tsoding](https://youtu.be/pnhJ5SDZpGU?si=fZ6equkOxoF3EgFu)
- [Freya Holmer on refresh rate perception](https://x.com/FreyaHolmer/status/1718979996125925494)
- [Raphlinus on compositor issues](https://raphlinus.github.io/ui/graphics/2020/09/13/compositor-is-evil.html)
- [Raphlinus on swapchain frame pacing](https://raphlinus.github.io/ui/graphics/gpu/2021/10/22/swapchain-frame-pacing.html)
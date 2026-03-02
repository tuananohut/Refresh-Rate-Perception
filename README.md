# Refresh Rate Perception

## Subframe Visualization & Temporal Resolution Exploration

![sim.gif](sim.gif)

This is a small simulation to explore refresh rate perception.

The main question is: can the human eye perceive differences beyond the visible frame rate?

This program renders at a low real frame rate (e.g., 30 FPS) while simulating physics at a much higher internal update rate (e.g., 480 Hz). The physics steps are visualized to show how motion is calculated in between frames.

This is an experimental visualization tool — not a scientific study and not an implementation of real frame pacing at the presentation layer. This project does not modify swapchain behavior, presentation timing, or OS-level frame pacing.

## What This Project Demonstrates

- The difference between simulation frequency and render frequency
- Fixed timestep physics using high-frequency substepping
- Visualizing subframes within a single display frame
- How temporal density affects perceived smoothness

## Technical Overview

### Fixed Timestep Physics

Physics runs at a constant frequency:

Target Physics Rate: 480 Hz

This is implemented using a fixed timestep loop (substepping), independent of rendering frequency.

Even if rendering occurs at 30 FPS, physics continues updating at 480 discrete steps per second.

## Key Features

- Fixed Timestep Physics: Physics runs at a high frequency (Target 480Hz) decoupled from the rendering framerate.
- Particle Collision: Basic circle-to-circle collision resolution.
- Screen Bounds: Particles bounce off screen edges with velocity checks to prevent sticking.
- UI & Controls:
  - `F`: Toggle UI (shows current/max particle count and physics FPS).
  - `Left Mouse`: Spawn particles.


## Based on

- Tsoding's "subframes" project: [GitHub](https://github.com/tsoding/subframes)

## Acknowledgements

- [Subframes video by Tsoding](https://youtu.be/pnhJ5SDZpGU?si=fZ6equkOxoF3EgFu)
- [Freya Holmer on refresh rate perception](https://x.com/FreyaHolmer/status/1718979996125925494)
- [Raphlinus on compositor issues](https://raphlinus.github.io/ui/graphics/2020/09/13/compositor-is-evil.html)
- [Raphlinus on swapchain frame pacing](https://raphlinus.github.io/ui/graphics/gpu/2021/10/22/swapchain-frame-pacing.html)
- [Don't use malloc](https://youtu.be/S7TPgGCZdeU?si=Ewy5U-5hGOpNlE6K)
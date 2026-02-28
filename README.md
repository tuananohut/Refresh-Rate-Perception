# Refresh Rate Perception – Extended Version

This is a small simulation to explore refresh rate perception.

The main question is: can the human eye perceive differences beyond the visible frame rate?

This program renders at a low real frame rate while simulating movement at a much higher internal update rate (subframes). The subframes are visualized to show how motion is distributed over time.

The goal is not scientific accuracy, but to experiment with frame pacing, temporal resolution, and visual perception.

## This version adds:

- Particle collision with Continuous Collision Detection (CCD) to prevent overlap and tunneling
- Automatic removal of particles leaving the screen
- On-screen UI toggle with F key, showing current and maximum particle counts
- Particle count limitation 

Goal remains to explore motion perception, frame pacing, and visual subframes.

## Based on

- Tsoding's "subframes" project: [GitHub](https://github.com/tsoding/subframes)

## Acknowledgements

- [Subframes video by Tsoding](https://youtu.be/pnhJ5SDZpGU?si=fZ6equkOxoF3EgFu)
- [Freya Holmer on refresh rate perception](https://x.com/FreyaHolmer/status/1718979996125925494)
- [Raphlinus on compositor issues](https://raphlinus.github.io/ui/graphics/2020/09/13/compositor-is-evil.html)
- [Raphlinus on swapchain frame pacing](https://raphlinus.github.io/ui/graphics/gpu/2021/10/22/swapchain-frame-pacing.html)
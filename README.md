# Dark Souls-Style Death Overlay

A lightweight Geode mod that shows a full-screen fade-in "YOU DIED" overlay with sound when the player dies, then seamlessly continues the normal reset.

## Features

- Full-screen semi-transparent black overlay (correct origin and sizing)
- "YOU DIED" label using GD bitmap font, red color, smooth fade-in
- Optional death sound effect (add your own OGG/WAV)
- Clean tear-down before calling the original reset (camera stays default)

## Installation

1. Place your sound at `resources/audio/you_died.ogg` (or update the path in code)
2. Build & install:
	- `geode build`
3. Launch Geometry Dash with Geode.

## Usage

- Play any level. When you die, the overlay fades in and the death sound plays; after ~1.2s the game resets like normal.

## Implementation Notes

- Hooks `PlayLayer::resetLevel()` to display overlay and audio, then calls the original function.
- Overlay constructed with `CCLayerColor` from bottom-left (0,0) to cover `winSize`.
- Label is centered and faded in; both nodes are removed before continuing reset.
- No camera or player icon manipulation; default behavior preserved.

# Geode Mod Development Instructions

## Project Overview
This is a Geometry Dash mod built with the Geode SDK that adds a Dark Souls-style death overlay. The mod hooks into the game's death/reset mechanics using Geode's class modification system.

## Architecture & Key Concepts

### Geode's `$modify` Hook System
- Use `class $modify(CustomName, OriginalClass)` to hook GD classes without manual trampolines
- Always call the original function: `PlayLayer::resetLevel()` calls base implementation
- Access mod-specific state via `m_fields->fieldName` (defined in nested `Fields` struct)
- Example: `DeathOverlayPlayLayer` hooks `PlayLayer::resetLevel()` to intercept death events

### Game State Access
- Player death: Check `m_player1->m_isDead` (NOT `m_isDead` on PlayLayer)
- Practice mode: Use `m_isPracticeMode` to skip overlay in practice
- Window size: `CCDirector::sharedDirector()->getWinSize()` for responsive positioning

### Cocos2d-x UI Patterns
- Create overlays: `CCLayerColor::create({0, 0, 0, 0}, width, height)` with anchor at `{0.f, 0.f}`
- Labels: Use GD fonts like `"bigFont.fnt"` with `CCLabelBMFont::create()`
- Z-ordering: Add to scene with high z-index (e.g., 1000) to render above gameplay
- Actions/animations: Chain with `CCSequence::create(CCFadeTo, CCDelayTime, CCCallFunc, nullptr)`

### Resource Handling
- Audio files: Reference with `_spr` macro: `"resources/audio/file.ogg"_spr`
- Declare in `mod.json` under `"resources": { "files": [...] }`
- Audio playback: `FMODAudioEngine::sharedEngine()->playEffect(path)`

## Build & Development Workflow

```bash
geode build              # Builds and auto-installs to GD mods folder
```

- CMake configured via `CMakeLists.txt` (requires `GEODE_SDK` env var)
- Output: `.geode` package in `build/` directory
- Source: All C++ files in `src/` are auto-included
- Bindings: Auto-generated from `build/_deps/bindings-src/bindings/{version}/GeometryDash.bro`

## Project-Specific Patterns

### Hook Timing Critical
- Hook `resetLevel()` (not `destroyPlayer()`) to show overlay AFTER death animation completes
- Use boolean flag (`isShowingDeathOverlay`) to prevent recursion when calling original
- Clean up UI elements before calling original function to avoid memory leaks

### Memory Management
- Always remove Cocos2d nodes: `node->removeFromParent()` before nulling pointers
- Actions auto-cleanup, but store node pointers in Fields to manually clean overlays

### Finding GD APIs
- Search bindings: `build/_deps/bindings-src/bindings/{GD_VERSION}/GeometryDash.bro`
- Use grep to find member variables (e.g., `m_player1`, `m_isPracticeMode`)
- Official docs: https://docs.geode-sdk.org/tutorials/modify

## Common Gotchas
- Camera modifications not needed for overlays - adding to scene with high z-index overlays everything
- `m_isDead` doesn't exist on PlayLayer - check `m_player1->m_isDead` instead
- Overlay dimensions: Set anchor to `{0.f, 0.f}` and position to `{0.f, 0.f}` for full-screen coverage
- Action sequences require `nullptr` terminator in `CCSequence::create()`

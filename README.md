# Chicken Flock

Chicken Flock (also known as Cock Flock) is a SDL3 (initially SDL2) minigame developed by
Colibri Studios as a first-year game-development project.

## Controls

- **Space, Enter, or left click:** advance through menus
- **P:** pause
- **M:** mute or unmute audio
- **T:** open the store from the menu
- **Q:** leave the store or return to the menu from pause
- **WASD** or **arrow keys:** move
- **F1:** toggle God Mode during gameplay and show red hitbox outlines
- **F2:** toggle Hard Mode
- **F3:** show credits
- **F4 from the menu or Graphics Room:** open or leave the Graphics Room
  (menu entry requires F1 debug mode)
- **Tab in the Graphics Room:** switch between the image gallery and audio
  player; **Space** previews the selected audio file
- **Left/Right:** browse one resource at a time; **Page Up/Page Down** jump
  ten resources, and **Home/End** jump to the first or last
- **Click the arrows or Back 10/Next 10 buttons:** navigate the selected
  resource
- **/** or the search field: filter images by name; type to search and press
  Enter to finish

The troubleshooting room recursively catalogs image and audio files below
`assets/`, including nested folders. The catalog and the five hand-written
curator notes in `assets/gallery/descriptions.json` are read lazily on a
background thread the first time the room is opened. Images and audio are
loaded only when selected for preview.

## Project layout

```text
assets/
  images/                 Game sprites and UI
  audio/music/            Music loaded by the game
  audio/sfx/              Sound effects loaded by the game
  audio/source/           Additional source audio, not loaded at runtime
  gallery/                Lazy-loaded image descriptions for the gallery
src/
  core/                   Shared definitions, game state, and asset paths
    game/scenes/          Scene implementations, factories, and scene-only contexts
  entities/               Player, enemies, pickups, and world objects
  rendering/              Sprites, buttons, camera, and shapes
  resources/              Image and audio resource store
                          Headers live beside their matching sources
CMakeLists.txt            Build definition
CMakePresets.json         Shared CLI and IDE configurations
vcpkg.json                Pinned SDL3 dependency manifest
```

The build stages runtime assets beside the executable. Asset lookup is based on
the executable path, so launching from a terminal, CLion, or another working
directory uses the same files.

Rupees, unlocked chicken types, and gameplay count are stored in a versioned
save file named `save.dat` under SDL's per-user preferences directory. The
game requests SDL's path with organization name `Colibri Studios` and
application name `Chicken Flock`. On Windows, the default file is:

```text
%APPDATA%\Colibri Studios\Chicken Flock\save.dat
```

`%APPDATA%` expands to the current Windows user's roaming application-data
directory (typically `C:\Users\<user>\AppData\Roaming`). SDL chooses the
corresponding per-user preferences location on other platforms. Updates are
coalesced and written on a background thread; pending data is drained when the
game shuts down. Rupees collected during a run are added to the saved balance
when the run ends in either victory or defeat.

## Requirements

- CMake 4.2 or newer
- Visual Studio 2026 or Visual Studio 2026 Build Tools with the **Desktop
  development with C++** workload
- Git and vcpkg

SDL3, SDL3_image, and SDL3_mixer are restored automatically from the pinned
vcpkg manifest when CMake configures the project. SDL3_mixer is built with
Vorbis, FLAC, MP3, Opus, and module-format support for the game and gallery.
The first configure needs network access.

### Install vcpkg on Windows

Run these once in PowerShell, choosing a stable location for vcpkg:

```powershell
winget install Kitware.CMake
New-Item -ItemType Directory -Force "$env:USERPROFILE\dev" | Out-Null
git clone https://github.com/microsoft/vcpkg $env:USERPROFILE\dev\vcpkg
& "$env:USERPROFILE\dev\vcpkg\bootstrap-vcpkg.bat"
$env:VCPKG_ROOT = "$env:USERPROFILE\dev\vcpkg"
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", $env:VCPKG_ROOT, "User")
```

Set `VCPKG_ROOT` as a persistent user environment variable if you want to use
the project in new terminals or IDE sessions without setting it again.

## Build and run from the CLI

From the repository root, configure and build the Debug preset:

```powershell
cmake --preset windows-debug
cmake --build --preset debug
.\build\windows-debug-vs18\Debug\ChickenFlock.exe
```

For an optimized build:

```powershell
cmake --preset windows-release
cmake --build --preset release
.\build\windows-release-vs18\Release\ChickenFlock.exe
```

The Windows presets use the Visual Studio 2026 generator and MSVC. The generated
executable and staged `assets` directory are under the corresponding
`build\windows-*-vs18\<Configuration>` directory. To remove local build output,
delete the `build` directory.

## Build and run in CLion

1. Install CMake 4.2 or newer and configure a Visual Studio 2026 C++ toolchain
   with the Desktop development with C++ workload.
2. Make `VCPKG_ROOT` available to CLion (restart it after setting a persistent
   environment variable).
3. Open this repository's root directory. CLion reads `CMakePresets.json`;
   enable and select **Windows x64 Debug** or **Windows x64 Release** as the
   CMake profile, then choose Debug or Release in the build configuration
   selector. If SDL headers remain unresolved after changing profiles, reload
   the CMake project so the vcpkg include directory is indexed.
4. Let CMake configure and restore the vcpkg dependencies. Build the
   `ChickenFlock` target, then run or debug that target.

If using IntelliJ IDEA rather than CLion, install and enable its C++ and CMake
support first; availability depends on the IntelliJ edition and installed
plugins. Then open the repository root and use the same CMake preset and
`VCPKG_ROOT` setup. CLion is the recommended JetBrains IDE for this project.

## Dependency and build management

- CMake is the only project/build-system definition; the Visual Studio
  `.sln`/`.vcxproj` project and checked-in SDL binaries have been retired.
- CMake automatically discovers `.cpp` and `.h` files recursively under `src/`,
  and adds each header's directory to the include path. New files and thematic
  subfolders do not require editing the build definition.
- `vcpkg.json` pins the registry baseline and declares all third-party
  dependencies. Avoid adding downloaded libraries directly to the source tree.
- `CMakePresets.json` keeps CLI and IDE configure/build settings aligned.
- Build trees, package-manager output, and per-user IDE files are ignored by
  Git. Game art and audio remain versioned.
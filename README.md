# Chicken Flock

Chicken Flock (also known as Cock Flock) is a small SDL2 game developed by
Colibri Studios as a first-year game-development project.

## Controls

- **Space, Enter, or left click:** advance through menus
- **P:** pause
- **M:** mute or unmute audio
- **T:** open the store from the menu
- **Q:** leave the store or return to the menu from pause
- **WASD** or **arrow keys:** move
- **F1:** toggle God Mode
- **F2:** toggle Hard Mode
- **F3:** show credits
- **F4:** graphics room (not implemented)

## Project layout

```text
assets/
  images/                 Game sprites and UI
  audio/music/            Music loaded by the game
  audio/sfx/              Sound effects loaded by the game
  audio/source/           Additional source audio, not loaded at runtime
src/
  app/                    Game loop and entry point
  core/                   Shared definitions and asset paths
  entities/               Player, enemies, pickups, and world objects
  rendering/              Sprites, buttons, camera, and shapes
  resources/              Image and audio resource store
                          Headers live beside their matching sources
CMakeLists.txt            Build definition
CMakePresets.json         Shared CLI and IDE configurations
vcpkg.json                Pinned SDL2 dependency manifest
```

The build stages runtime assets beside the executable. Asset lookup is based on
the executable path, so launching from a terminal, CLion, or another working
directory uses the same files.

## Requirements

- CMake 3.25 or newer
- Visual Studio 2026 or Visual Studio 2026 Build Tools with the **Desktop
  development with C++** workload
- Git and vcpkg

SDL2, SDL2_image, and SDL2_mixer are restored automatically from the pinned
vcpkg manifest when CMake configures the project. The first configure needs
network access.

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

From the repository root, configure with the Visual Studio generator and build
the Debug configuration:

```powershell
cmake --preset windows-msvc
cmake --build --preset debug
.\build\windows-msvc\Debug\ChickenFlock.exe
```

For an optimized build:

```powershell
cmake --build --preset release
.\build\windows-msvc\Release\ChickenFlock.exe
```

The Visual Studio generator selects the MSVC compiler and doesn't require Ninja
or a preconfigured Developer PowerShell. The generated executable and staged
`assets` directory are in the matching configuration under
`build\windows-msvc`. To remove local build output, delete the `build`
directory.

## Build and run in CLion

1. Install CLion and configure a Visual Studio 2022 C++ toolchain with the
   Desktop development with C++ workload.
2. Make `VCPKG_ROOT` available to CLion (restart it after setting a persistent
   environment variable).
3. Open this repository's root directory. CLion reads `CMakePresets.json`;
   select **Windows x64 (Visual Studio 2026)** as the CMake profile, then
   choose Debug or Release in the build configuration selector.
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

## Future SDL3 migration guide (planning notes)

The current game remains on SDL2; these are suggested steps for a separate,
future SDL3 effort, not changes included in the present build:

1. Create a migration branch and record a working SDL2 build and playthrough.
2. Upgrade the SDL2, SDL2_image, and SDL2_mixer manifest entries together only
   after confirming compatible SDL3 package versions are available in vcpkg.
3. Port initialization, window/rendering, input/event handling, and timing to
   SDL3. Compile and run after each area rather than mixing API changes into
   one large rewrite.
4. Migrate image and audio integration to SDL3_image and SDL3_mixer, checking
   format support, initialization, and error handling against the new APIs.
5. Verify every scene, control, music track, sound effect, and asset on each
   supported platform; update this guide and the dependency manifest only
   after the SDL3 version is reproducible from a clean configure.

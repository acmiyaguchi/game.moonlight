# game.moonlight

A Moonlight client using the Kodi Game API

# Building stand-alone (development)

Stand-alone builds are closer to "normal" software builds. The build system looks for its dependencies, by default with `/usr` and `/usr/local` prefixes.

To provide these dependencies yourself in a local working directory (`$HOME/kodi`), build Kodi with an installation prefix

```shell
./configure --prefix=$HOME/kodi
make
make install
```

Clone kodi-platform and create a CMake build directory

```shell
git clone https://github.com/xbmc/kodi-platform.git
cd kodi-platform
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=$HOME/kodi \
      ..
make
make install
```

The platform library was split from kodi-platform. Do the same as above for the new platform library:

```
git clone https://github.com/Pulse-Eight/platform.git
...
```

With these dependencies in place, the add-on can be built. Point CMake to the add-on's build system instead of `$HOME/workspace/xbmc/project/cmake/addons`

```shell
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=$HOME/workspace/xbmc/addons \
      -DCMAKE_PREFIX_PATH=$HOME/kodi \
      -DPACKAGE_ZIP=1 \
      ..
```
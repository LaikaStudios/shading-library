# Production Shading Library

[![](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE-APACHE)
[![](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE-MIT)

The Production Shading Library refers to [Laika's](https://www.laika.com) entire collection of VFX production shading capabilities. This repository will contain those portions of Laika's Production Shading Library that have been released as open source.

This release contains a set of shading nodes based on [Pixar's](https://www.pixar.com) [RenderMan](https://renderman.pixar.com/product) that provide:

- [Material Layering System](https://github.com/LaikaStudios/shading-library/wiki/dev.Material-Layering-System)
- [Annotate displayfilter](https://github.com/LaikaStudios/shading-library/wiki/dev.Annotate)

If you make use of this repsitory, I strongly urge you to [subscribe to the discussion group](https://groups.google.com/group/laikastudios-shading-library). Please use this group to provide feedback on any bugs you may find or to discuss any feature requests.

# Requirements
* [Pixar's](https://www.pixar.com/renderman) [RenderMan 23](https://rmanwiki.pixar.com/display/REN23/RenderMan) </br></br>
* [Foundry's](https://www.foundry.com) [Katana](https://www.foundry.com/products/katana)
* [Pixar's](https://www.pixar.com/renderman) [RenderMan 23 for Katana 3.2 or 3.5](https://rmanwiki.pixar.com/display/RFK23/RenderMan+23+for+Katana) </br>~and/or~</br>
* ~[Autodesk's](https://www.autodesk.com) [Maya](https://www.autodesk.com/products/maya/overview)~
* ~[Pixar's](https://www.pixar.com/renderman) [RenderMan 23 for Maya 2017/2018/2019](https://rmanwiki.pixar.com/display/RFM23/RenderMan+23+for+Maya)~ </br></br>
* A c++14 compatible compiler (clang++ is specified in the [ris/Makefile](https://github.com/LaikaStudios/shading-library/blob/dev/ris/Makefile))

# Documentation
Documentation is on the [wiki page](https://github.com/LaikaStudios/shading-library/wiki/dev.Home).

# Building Instructions
The Production Shading Library at Laika is developed in a [Linux](https://en.wikipedia.org/wiki/Linux) environment. Specifically, [Fedora](https://getfedora.org). This initial release does not contain any facilities for building in other operating system environments. That said, as long as the necessary `c++` compatible compiler and `make` and `rsync` commands are available, it should be trivial to build the shading-library code on other platforms.

To build the contents, execute

    make clean; make

in your local checkout directory of the shading-library. This will put the built contents into a `build` sub-directory.

# Environment
Assuming you checked out this git repository to `~/shading-library` and have built it, katana's execution environment should have the following environment variables set. Make any alterations necessary for your specific set of software.

Using `bash` shell:

    # Shading-library installation location.
    export SHADING_LIBRARY="${HOME}/shading-library"

    # Pixar installation location.
    export PIXAR_ROOT="/opt/pixar"

    # RenderMan version.
    export RMAN_MAJOR="23"
    export RMAN_MINOR="2"
    export RMAN_MICRO=""
    export RMAN_VERSION="${RMAN_MAJOR}.${RMAN_MINOR}${RMAN_MICRO}"

    # Katana version.
    export KATANA_MAJOR="3"
    export KATANA_MINOR="2"
    export KATANA_MICRO="v4"
    export KATANA_SHORT_VERSION="${KATANA_MAJOR}.${KATANA_MINOR}"

    # RenderMan and shading-library software locations.
    export RMANTREE="${PIXAR_ROOT}/RenderManProServer-${RMAN_VERSION}"
    export RFK_LOCATION="${PIXAR_ROOT}/RenderManForKatana-${RMAN_VERSION}-katana${KATANA_SHORT_VERSION}"
    export RMAN_RIXPLUGINPATH="${SHADING_LIBRARY}/build:${RMANTREE}/lib/plugins"
    export RMAN_SHADERPATH="${SHADING_LIBRARY}/build:${RMANTREE}/lib/plugins:${RMANTREE}/lib/shaders"

    # Katana resource locations.
    export KATANA_RESOURCES="${RFK_LOCATION}/plugins/Resources/PRMan${RMAN_MAJOR}:${SHADING_LIBRARY}/katana"

# License
Licensed under either of

 * Apache License, Version 2.0, ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.

## Contribution
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be dual licensed as above, without any
additional terms or conditions.

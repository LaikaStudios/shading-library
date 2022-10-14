# Production Shading Library

[![](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE-APACHE)
[![](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE-MIT)

The Production Shading Library refers to [Laika's](https://www.laika.com) entire collection of VFX production shading capabilities.

This release contains the final contents of Laika's Production Shading Library based on [Pixar's](https://www.pixar.com) [RenderMan 20](https://renderman.pixar.com/resources/RenderMan_20/home.html).
It represents the culmination of this toolset as it evolved and matured over the course of producing 
[ParaNorman](https://laika.com/our-films/paranorman), 
[The Boxtrolls](https://laika.com/our-films/boxtrolls),
and [Kubo and the Two Strings](https://laika.com/our-films/kubo).

Please use the Discussions tab to provide any feedback or to initiate any other dialog about this repository.

# Requirements
* [Pixar's](https://www.pixar.com) [RenderMan 20](https://renderman.pixar.com/resources/RenderMan_20/home.html)
* [Pixar's](https://www.pixar.com) [RenderMan 20 for Maya](https://renderman.pixar.com/resources/RenderMan_20/RMS_101.html)
* [Autodesk's](https://www.autodesk.com) [Maya](https://www.autodesk.com/products/maya/overview) [2015](https://knowledge.autodesk.com/support/maya/downloads/caas/downloads/content/autodesk-maya-2015-service-pack-6.html), [2016](https://knowledge.autodesk.com/support/maya/downloads/caas/downloads/content/maya-2016-service-pack-6.html), or [2017](https://help.autodesk.com/view/MAYAUL/2017/ENU/)

*Note*: Maya is not strictly required, as the RenderMan for Maya shader authoring tool [Slim](https://renderman.pixar.com/resources/RenderMan_20/slimGettingStarted.html) can be used by itself to create and manage shaders.

# Documentation
Documentation is on the [wiki page](https://github.com/LaikaStudios/shading-library/wiki/prman_20.Home).

# Environment
Assuming you cloned this git repository to `/opt/laika/shading-library`, you should have the following environment variables set. Make any alterations as needed for your specific operating system or application versions.

For addition information, you can take a look at the [RenderMan 20 Environment Variable](https://renderman.pixar.com/resources/RenderMan_20/env_vars.html) documentation.


Using `bash` shell:

    # Laika shading-library installation location.
    export LAIKA_ROOT="/opt/laika"
    export SHADING_LIBRARY="${LAIKA_ROOT}/shading-library"

    # Pixar installation location.
    export PIXAR_ROOT="/opt/pixar"

    # RenderMan version.
    export RMAN_MAJOR="20"
    export RMAN_MINOR="12"
    export RMAN_MICRO=""
    export RMAN_VERSION="${RMAN_MAJOR}.${RMAN_MINOR}${RMAN_MICRO}"

    # Maya version.
    export MAYA_VERSION="2017"

    # RenderMan Environment.
    export RMANTREE="${PIXAR_ROOT}/RenderManProServer-${RMAN_VERSION}"
    export RMSTREE="${PIXAR_ROOT}/RenderManStudio-${RMAN_VERSION}-maya${MAYA_VERSION}"
    export RMAN_SHADERPATH="${RMSTREE}/lib/plugins:${RMSTREE}/lib/shaders:${RMANTREE}/lib/shaders:${RMANTREE}/lib/rsl/shaders"
    export RMS_SCRIPT_PATHS="${SHADING_LIBRARY}"

    # Maya Environment.
    export MAYA_PLUG_IN_PATH="${RMSTREE}/plug-ins"
    export MAYA_SCRIPT_PATH="${SHADING_LIBRARY}/command/maya:${RMSTREE}/scripts"
    export XBMLANGPATH="${RMSTREE}/icons/%B"

    # Add bin directories to PATH.
    export PATH="${PATH}:${RMANTREE}/bin:${RMSTREE}/bin"

# License
Licensed under either of

 * Apache License, Version 2.0, ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.

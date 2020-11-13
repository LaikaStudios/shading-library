# Production Shading Library

[![](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE-APACHE)
[![](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE-MIT)

The Production Shading Library refers to [Laika's](https://www.laika.com) entire collection of VFX production shading capabilities.

This release contains the final contents of Laika's Production Shading Library based on [Pixar's](https://www.pixar.com) [RenderMan 20](https://renderman.pixar.com/resources/RenderMan_20/home.html).
It represents the culmination of this toolset as it evolved and matured over the course of producing 
[ParaNorman](https://laika.com/our-films/paranorman), 
[The Boxtrolls](https://laika.com/our-films/boxtrolls),
and [Kubo and the Two Strings](https://laika.com/our-films/kubo).

If you make use of this repository, I strongly urge you to [subscribe to the discussion group](https://groups.google.com/group/laikastudios-shading-library). Please use this group to provide feedback on any bugs you may find or to discuss any feature requests.

# Requirements
* [Pixar's](https://www.pixar.com) [RenderMan 20](https://renderman.pixar.com/resources/RenderMan_20/home.html)
* [Pixar's](https://www.pixar.com) [RenderMan 20 for Maya](https://renderman.pixar.com/resources/RenderMan_20/RMS_101.html)
* [Autodesk's](https://www.autodesk.com) [Maya](https://www.autodesk.com/products/maya/overview) [2015](https://knowledge.autodesk.com/support/maya/downloads/caas/downloads/content/autodesk-maya-2015-service-pack-6.html), [2016](https://knowledge.autodesk.com/support/maya/downloads/caas/downloads/content/maya-2016-service-pack-6.html), [2017](https://help.autodesk.com/view/MAYAUL/2017/ENU/)

*Note*: Maya is not strictly required, as the RenderMan for Maya shader authoring tool [Slim](https://renderman.pixar.com/resources/RenderMan_20/slimGettingStarted.html) can be used by itself to create and manage shaders.

# Documentation
Documentation is on the [wiki page](https://github.com/LaikaStudios/shading-library/wiki/prman_20.Home).

# Environment
Assuming you cloned this git repository to `/opt/laika/shading-library`, you should have the following environment variables set. Make any alterations as needed for your specific situation.

Using `bash` shell:

    # Shading-library installation location.
    export SHADING_ROOT="/opt/laika/shading-library"

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
    export RMSTREE="${PIXAR_ROOT}/RenderManForMaya-${RMAN_VERSION}-maya${MAYA_VERSION}"
    export RMAN_SHADERPATH="${RMSTREE}/lib/plugins:${RMSTREE}/lib/shaders:${RMANTREE}/lib/plugins:${RMANTREE}/lib/shaders"

    # Maya Environment.
    export MAYA_PLUG_IN_PATH="${RMSTREE}/plug-ins"
    export MAYA_SCRIPT_PATH="${SHADING_ROOT}/command/maya:${RMSTREE}/scripts"
    export XBMLANGPATH="${RMSTREE}/icons/%B"

For reference, you can take a look at the [RenderMan 20 Environment Variable](https://renderman.pixar.com/resources/RenderMan_20/env_vars.html) documentation.

# License
Licensed under either of

 * Apache License, Version 2.0, ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
 * MIT license ([LICENSE-MIT](LICENSE-MIT) or http://opensource.org/licenses/MIT)

at your option.

## Contribution
Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be dual licensed as above, without any
additional terms or conditions.

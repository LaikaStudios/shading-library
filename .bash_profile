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

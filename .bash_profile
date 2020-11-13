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

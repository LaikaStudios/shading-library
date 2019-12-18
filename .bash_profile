# Shading-library installation location.
export SHADING_LIBRARY="${HOME}/shading-library"

# Pixar installation location.
export PIXAR_ROOT="/opt/pixar"

# RenderMan version.
export RMAN_MAJOR="23"
export RMAN_MINOR="0"
export RMAN_MICRO=""
export RMAN_VERSION="${RMAN_MAJOR}.${RMAN_MINOR}${RMAN_MICRO}"

# Katana version.
export KATANA_MAJOR="3"
export KATANA_MINOR="2"
export KATANA_MICRO="v3"
export KATANA_SHORT_VERSION="${KATANA_MAJOR}.${KATANA_MINOR}"

# RenderMan and shading-library software locations.
export RMANTREE="${PIXAR_ROOT}/RenderManProServer-${RMAN_VERSION}"
export RFK_LOCATION="${PIXAR_ROOT}/RenderManForKatana-${RMAN_VERSION}-katana${KATANA_SHORT_VERSION}"
export RMAN_SHADERPATH="${SHADING_LIBRARY}/build:${RMANTREE}/lib/plugins:${RMANTREE}/lib/shaders"
export RMAN_RIXPLUGINPATH="${SHADING_LIBRARY}/build:${RMANTREE}/lib/plugins"

# Katana resource locations.
export KATANA_RESOURCES="${RFK_LOCATION}/plugins/Resources/PRMan${RMAN_MAJOR}:${SHADING_LIBRARY}/katana"
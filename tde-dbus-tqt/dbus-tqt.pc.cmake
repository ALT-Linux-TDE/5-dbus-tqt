prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@PC_EXEC_PREFIX@
libdir=@PC_LIB_DIR@
includedir=@PC_INCLUDE_DIR@

Name: @PROJECT_NAME@
Description: D-BUS TQt bindings
Version: @VERSION@
Requires: dbus-1
Libs: -L${libdir} @PC_LIBRARIES@
Cflags: -I${includedir} -I${includedir}/dbus-1.0

set(_rpi_root_dir ${_rpi_install_prefix})
set(_rpi_module_paths ${_rpi_install_prefix})

set(_rpi_at @)
set(_rpi_module_location_template ${_rpi_install_prefix}/${_rpi_at}module${_rpi_at}/${_rpi_at}module${_rpi_at}Config.cmake)
unset(_rpi_at)

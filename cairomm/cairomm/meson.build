cairomm_sources = [
    'context.cc',
    'context_surface_quartz.cc',
    'context_surface_win32.cc',
    'context_surface_xlib.cc',
    'device.cc',
    'exception.cc',
    'fontface.cc',
    'fontoptions.cc',
    'matrix.cc',
    'path.cc',
    'pattern.cc',
    'private.cc',
    'quartz_font.cc',
    'quartz_surface.cc',
    'region.cc',
    'scaledfont.cc',
    'script.cc',
    'script_surface.cc',
    'surface.cc',
    'win32_font.cc',
    'win32_surface.cc',
    'xlib_surface.cc',
]

cairomm_headers = [
    'cairomm.h',
    'context.h',
    'context_private.h',
    'device.h',
    'enums.h',
    'exception.h',
    'fontface.h',
    'fontoptions.h',
    'matrix.h',
    'path.h',
    'pattern.h',
    'private.h',
    'quartz_font.h',
    'quartz_surface.h',
    'refptr.h',
    'region.h',
    'scaledfont.h',
    'script.h',
    'script_surface.h',
    'surface.h',
    'types.h',
    'win32_font.h',
    'win32_surface.h',
    'xlib_surface.h',
]

install_headers(cairomm_headers, subdir: cairomm_api_path)

# Features header
cairomm_version_conf = configuration_data()
cairomm_version_conf.set('CAIROMM_MAJOR_VERSION', cairomm_major_version)
cairomm_version_conf.set('CAIROMM_MINOR_VERSION', cairomm_minor_version)
cairomm_version_conf.set('CAIROMM_MICRO_VERSION', cairomm_micro_version)
cairomm_version_conf.set('CAIROMM_BINARY_AGE', cairomm_binary_age)
cairomm_version_conf.set('CAIROMM_INTERFACE_AGE', cairomm_interface_age)

cairomm_cflags = [
  '/D CAIROMM_BUILD',
  '/D _WINDLL',
  '/D _MBCS',
]

def_file = meson.project_name() + '.def'
cairomm_ldflags = [ '/_MK_DEF:' + def_file, ]

# Our link command that create the def files
my_link = find_program('link_gendef.exe')
# We copy this in the build dir
cpy = find_program('link_gendef-cpy.py')
run_command(cpy, my_link.path(), meson.build_root())

cairomm_inc = include_directories('.')
system_inc = include_directories( join_paths(cairomm_includedir, 'glib-2.0') )
win = import('windows')

libcairomm = shared_library('cairomm-@0@'.format(cairomm_api_version),
                        win.compile_resources('cairomm.rc'),
                        sources: cairomm_sources,
                        version: cairomm_libversion,
                        install: true,
                        dependencies: [
                            cairo_dep,
                            sigc_dep,
                        ],
                        include_directories: [ root_inc, cairomm_inc, system_inc, ],
                        cpp_args: common_cflags + cairomm_cflags,
                        link_args: common_ldflags + cairomm_ldflags)

libcairomm_dep = declare_dependency(link_with: libcairomm,
                                    include_directories: cairomm_inc,
                                    dependencies: [
                                        cairo_dep,
                                        sigc_dep,
                                    ],
                                )

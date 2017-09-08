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

if false
configure_file(input: 'atkversion.h.in',
               output: 'atkversion.h',
               configuration: cairomm_version_conf,
               install: true,
               install_dir: join_paths(cairomm_includedir, cairomm_api_path))
# Marshallers
cairomm_marshals = gnome.genmarshal('atkmarshal',
                                sources: 'atkmarshal.list',
                                prefix: 'cairomm_marshal')
cairomm_marshal_h = cairomm_marshals[1]
endif

if false
# Enumerations for GType
cairomm_enums = gnome.mkenums('atk-enum-types',
                          sources: cairomm_headers,
                          c_template: 'atk-enum-types.c.template',
                          h_template: 'atk-enum-types.h.template',
                          install_dir: join_paths(cairomm_includedir, cairomm_api_path),
                          install_header: true)
cairomm_enum_h = cairomm_enums[1]
endif

cairomm_cflags = [
  '/D CAIROMM_BUILD',
  '/D _WINDLL',
  '/D _MBCS',
  '/FImsvc_recommended_pragmas.h',
]

def_file = join_paths(meson.current_build_dir(), 'cairomm.def')
cairomm_ldflags = [ '/DEF:' + def_file, ]

cairomm_inc = include_directories('.')
system_inc = include_directories( join_paths(cairomm_includedir, 'glib-2.0') )
win = import('windows')

message('Cur build:' + meson.current_build_dir())
libcairomm = shared_library('cairomm-@0@'.format(cairomm_api_version),
                        win.compile_resources('cairomm.rc'),
                        sources: cairomm_sources,
                        version: cairomm_libversion,
                        install: true,
                        link_depends : [ 'cairomm.def', ],
                        dependencies: [ cairo_dep,
                                        sigc_dep,
                        ],
                        include_directories: [ root_inc, cairomm_inc, system_inc, ],
                        cpp_args: common_cflags + cairomm_cflags,
                        link_args: common_ldflags + cairomm_ldflags)

# build the object list for generating the def files
obj = []
foreach s: cairomm_sources
    ta = join_paths('cairomm-1.0@sha', s + '.obj')
    message(ta)
    obj += join_paths(ta)
endforeach
gdef = custom_target('auto_def',
                     input : obj,
                     output : 'cairomm.def',
                     command : [ join_paths(cairomm_prefix, 'bin', 'gendef.exe'), '@OUTPUT@', 'cairomm-1.0-1.dll', '@INPUT@', ],
##                     build_always : true,
)
##def_dep = declare_dependency(sources : def_file)

libcairomm_dep = declare_dependency(link_with: libcairomm,
                                    include_directories: cairomm_inc,
                                    dependencies: [ cairo_dep,
                                                sigc_dep,
                                    ],
                                )

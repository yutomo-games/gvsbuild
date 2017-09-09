project('cairomm', 'cpp',
        version: '1.15.4',
        license: 'LGPLv2.1+',
        default_options: [
          'buildtype=debug',
          'warning_level=1',
        ],
        meson_version : '>= 0.42')

cc = meson.get_compiler('cpp')

version = meson.project_version().split('.')
cairomm_major_version = version[0].to_int()
cairomm_minor_version = version[1].to_int()
cairomm_micro_version = version[2].to_int()

cairomm_interface_age = 1
cairomm_binary_age = 10000 * cairomm_major_version + 100 * cairomm_minor_version + 10 + cairomm_micro_version

cairomm_api_version = '1.0'
cairomm_api_path = 'cairomm-@0@'.format(cairomm_api_version)
cairomm_api_path = 'cairomm'

cairomm_prefix = get_option('prefix')
cairomm_libdir = join_paths(cairomm_prefix, get_option('libdir'))
cairomm_sysconfdir = join_paths(cairomm_prefix, get_option('sysconfdir'))
cairomm_includedir = join_paths(cairomm_prefix, get_option('includedir'))
cairomm_datadir = join_paths(cairomm_prefix, get_option('datadir'))
cairomm_libexecdir = join_paths(cairomm_prefix, get_option('libexecdir'))

cairomm_conf = configuration_data()

cairomm_conf.set_quoted('VERSION', meson.project_version())
cairomm_conf.set('CAIROMM_MAJOR_VERSION', cairomm_major_version)
cairomm_conf.set('CAIROMM_MINOR_VERSION', cairomm_minor_version)
cairomm_conf.set('CAIROMM_MICRO_VERSION', cairomm_micro_version)
cairomm_conf.set('CAIROMM_EXCEPTION_ENABLED', 1)
cairomm_conf.set_quoted('GETTEXT_PACKAGE', 'cairomm10')

# Maintain version scheme with libtool
cairomm_soversion = 1
cairomm_libversion = '@0@.@1@.@2@'.format(cairomm_soversion, (cairomm_binary_age - cairomm_interface_age), cairomm_interface_age)

##add_project_arguments([ '-DG_DISABLE_SINGLE_INCLUDES', '-DATK_DISABLE_SINGLE_INCLUDES' ], language: 'cpp')

# Compiler and linker flags
common_ldflags = []

# Compiler flags
if cc.get_id() == 'msvc'
  # Make MSVC more pedantic, this is a recommended pragma list
  # from _Win32_Programming_ by Rector and Newcomer.  Taken from
  # glib's msvc_recommended_pragmas.h--please see that file for
  # the meaning of the warning codes used here
  test_cflags = [
    '-we4002', # too many actual parameters for macro
    '-we4003', # not enough actual parameters for macro
    '-w14010', # single-line comment contains line-continuation character
    '-we4013', # 'function' undefined; assuming extern returning int
    '-w14016', # no function return type; using int as default
    '-we4020', # too many actual parameters
    '-we4021', # too few actual parameters
    '-we4027', # function declared without formal parameter list
    '-we4029', # declared formal parameter list different from definition
    '-we4033', # 'function' must return a value
    '-we4035', # 'function' : no return value
    '-we4045', # array bounds overflow
    '-we4047', # different levels of indirection
    '-we4049', # terminating line number emission
    '-we4053', # an expression of type void was used as an operand
    '-we4071', # no function prototype given
    '-we4819', # the file contains a character that cannot be represented in the current code page
  ]
else
  test_cflags = []
endif

common_cflags = []
foreach cflag: test_cflags
  if cc.has_argument(cflag)
    common_cflags += [ cflag ]
  endif
endforeach

# Dependencies
sigc_req_version = '>= 2.10.0'
sigc_dep = dependency('sigc++', version: sigc_req_version)

cairo_req_version = '>= 1.12.0'
cairo_dep = dependency('cairo', version: cairo_req_version)

# Compat variables for pkgconfig
pkgconf = configuration_data()
pkgconf.set('prefix', cairomm_prefix)
pkgconf.set('exec_prefix', cairomm_prefix)
pkgconf.set('libdir', cairomm_libdir)
pkgconf.set('includedir', cairomm_includedir)
pkgconf.set('VERSION', meson.project_version())
pkgconf.set('ATK_API_VERSION', cairomm_api_version)
pkgconf.set('srcdir', '.')

pc_files = [
    'cairomm-1.16.pc',
    'cairomm-ft-1.16.pc',
    'cairomm-pdf-1.16.pc',
    'cairomm-png-1.16.pc',
    'cairomm-ps-1.16.pc',
    'cairomm-svg-1.16.pc',
    'cairomm-win32-1.16.pc',
    'cairomm-win32-font-1.16.pc',
]

foreach pkg: pc_files
  configure_file(input: join_paths('data', pkg + '.in'),
                 output: pkg,
                 configuration: pkgconf,
                 install: true,
                 install_dir: join_paths(cairomm_libdir, 'pkgconfig'))
endforeach

gnome = import('gnome')

# Internal configuration header
configure_file(output: 'cairommconfig.h', configuration: cairomm_conf)

root_inc = include_directories('.')

subdir('cairomm')
subdir('examples\text')
# subdir('tests')


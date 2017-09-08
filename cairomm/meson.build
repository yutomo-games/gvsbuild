project('cairomm', 'cpp',
        version: '1.15.4',
        license: 'LGPLv2.1+',
        default_options: [
          'buildtype=debug',
          'warning_level=1',
#          'c_std=c99',
        ],
        meson_version : '>= 0.40.1')

##cc = meson.get_compiler('c')
##host_system = host_machine.system()

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
common_cflags = []
common_ldflags = []

test_cflags = []

if false
        # Symbol visibility
        if get_option('default_library') != 'static'
          if host_system == 'windows'
            cairomm_conf.set('DLL_EXPORT', true)
            cairomm_conf.set('_ATK_EXTERN', '__declspec(dllexport) extern')
            if cc.get_id() != 'msvc'
              test_cflags += ['-fvisibility=hidden']
            endif
          else
            cairomm_conf.set('_ATK_EXTERN', '__attribute__((visibility("default"))) extern')
            test_cflags += ['-fvisibility=hidden']
          endif
        endif

        # Check all compiler flags
        foreach cflag: test_cflags
          if cc.has_argument(cflag)
            common_cflags += [ cflag ]
          endif
        endforeach

        # Linker flags
        if host_machine.system() == 'linux'
          foreach ldflag: [ '-Wl,-Bsymbolic', '-Wl,-z,relro', '-Wl,-z,now', ]
            if cc.has_argument(ldflag)
              common_ldflags += [ ldflag ]
            endif
          endforeach
        endif

        # Maintain compatibility with autotools on macOS
        if host_machine.system() == 'darwin'
          common_ldflags += [ '-compatibility_version=1', '-current_version=1.0', ]
        endif

        # Functions
        checked_funcs = [
          'bind_textdomain_codeset',
        ]

        foreach f: checked_funcs
          if cc.has_function(f)
            cairomm_conf.set('HAVE_' + f.underscorify().to_upper(), 1)
          endif
        endforeach
endif

# Dependencies
## gobject_req_version = '>= 2.31.2'
## gobject_dep = dependency('gobject-2.0', version: gobject_req_version)

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


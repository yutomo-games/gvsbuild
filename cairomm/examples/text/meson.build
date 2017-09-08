text_sources = [
    'text-rotate',
    'toy-text',
    'user-font',
]

foreach t: text_sources
  c_example = executable(t,
                         sources: t + '.cc',
                         dependencies: libcairomm_dep,
                         include_directories: root_inc,
                         link_args: common_ldflags,
                         cpp_args: common_cflags)
endforeach

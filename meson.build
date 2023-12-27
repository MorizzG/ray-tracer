project('ray-tracer', 'cpp', default_options : [
    'warning_level=3',
    'werror=true',
    'cpp_std=c++23',
    'b_lto=true',
    'b_lto_mode=thin',
])

add_project_arguments(
    '-Wconversion',

    language : 'cpp'
)

sources = []
subdir('src')

inc = include_directories('src')

executable('ray-tracer', sources, include_directories : inc)

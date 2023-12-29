project('ray-tracer', 'cpp', default_options : [
    'warning_level=3',
    'werror=true',
    'cpp_std=c++23',
    'b_lto=true',
    # 'b_lto_mode=thin',
])

add_project_arguments(
    '-Wconversion',

    language : 'cpp'
)


philox_proj = subproject('philox', default_options : ['warning_level=0', 'werror=false'])
philox_dep = philox_proj.get_variable('philox_dep')


sources = []
subdir('src')

inc = include_directories('src')

executable('ray-tracer', sources, include_directories : inc, dependencies : [philox_dep])

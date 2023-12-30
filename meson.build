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

if get_option('native')
    add_global_arguments(
        '-march=native',

        language : 'c'
    )

    add_global_arguments(
        '-march=native',

        language : 'cpp'
    )
endif


rand_proj = subproject('rand', default_options : ['warning_level=0', 'werror=false'])
rand_dep = rand_proj.get_variable('rand_dep')


sources = []
subdir('src')

inc = include_directories('src')

executable('ray-tracer', sources, include_directories : inc, dependencies : [rand_dep])

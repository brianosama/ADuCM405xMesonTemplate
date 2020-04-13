project('ADuCM405xMesonTemplate', 'c',
            default_options : [ 'buildtype=debug',
                                ])

# initialize global variables

srcs          = []
incdirs       = []
c_args        = []
link_args     = []
link_deps     = []

# import project subdirectories

feature_list = meson.get_external_property('feature_list')
dfp_subdir = meson.get_external_property('dfpack')

subdir(dfp_subdir)

# detect cpu

cpu = host_machine.cpu() == 'cortex-m0+' ? 'cortex-m0plus' : host_machine.cpu()

# detect cpu architecture

arch       = (cpu == 'cortex-m0') or (cpu == 'cortex-m0+') or (cpu == 'cortex-m1') ? 'v6-m'  : ''
arch      += (cpu == 'cortex-m3')                                                  ? 'v7-m'  : ''
arch      += (cpu == 'cortex-m4') or (cpu == 'cortex-m7')                          ? 'v7e-m' : ''

# get c source files

srcs    += files('main.c')

# get c include files

incdirs += include_directories('.')
incdirs += include_directories('/usr/lib/arm-none-eabi/include/')

# setup c compiler arguments
    
c_args  += '-mcpu=@0@'.format(cpu)
c_args  += meson.get_external_property('c_args')

# setup c linker properties

link_args += meson.get_external_property('c_link_args')

foreach linkfile : mcu_linksct
    link_args += ['-Wl,-T,@0@/@1@/@2@'.format(meson.current_source_dir(), dfp_subdir, linkfile)]
endforeach

link_args += '-Wl,-Map,output.map'

# setup linker dependencies

link_deps +=  meson.get_compiler('c').find_library('/usr/lib/gcc/arm-none-eabi/6.3.1/thumb/@0@/libgcc'.format(arch))
link_deps +=  meson.get_compiler('c').find_library('/usr/lib/arm-none-eabi/lib/libc_nano')
link_deps +=  meson.get_compiler('c').find_library('/usr/lib/arm-none-eabi/lib/libnosys')
link_deps +=  meson.get_compiler('c').find_library('/usr/lib/arm-none-eabi/lib/libm')

main = executable(
            'main',
            [srcs] ,
            name_suffix         : 'elf',
            c_args              : [c_args],
            link_args           : [link_args, '-Wl,--gc-sections'],
            dependencies        : link_deps,
            include_directories : [incdirs] )

# import binary objects

objcopy  = '@0@'.format(find_program('objcopy').path())
objdump  = '@0@'.format(find_program('objdump').path())
size     = '@0@'.format(find_program('size').path())
gdb      = '@0@'.format(find_program('gdb').path())
terminal = '@0@'.format(find_program('terminal',required:false).path())
openocd  = '@0@'.format(find_program('openocd', required:false).path())

# create custom target : create binary, hex dump, size and disassemble dump
mainbin = custom_target(
                        'main.bin',
    output           : ['main.bin'],
    # name_suffix      : 'bin',
    build_by_default : true,
    command          : [objcopy, '-O', 'binary', 'main.elf', 'main.bin'],
    depends          : [main])

mainhex = custom_target(
                            'main.hex',
        output           : ['main.hex'],
        build_by_default : true,
        command          : [objcopy, '-O', 'ihex', 'main.elf', 'main.hex'],
        depends          : [main])

mainsize = custom_target(
                          'size',
        capture          : true,
        output           : ['main.size'],
        build_by_default : true,
        command          : [size, '--format=berkeley', 'main.elf'],
        depends          : [main])

maindump = custom_target(
                          'dump',
        capture          : true,
        output           : 'main.dump',
        build_by_default : false,
        command          : [objdump, '-triple=@0@-none-eabi'.format(arch), '-disassemble-all', '-S', '-t', 'main.elf'],
        depends          : [main])

run_target('gdb',
        command : [terminal, '-e', 'gdb-multiarch', '-q', '@0@/main.elf'.format(meson.current_build_dir())])





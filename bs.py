import os
import sys
import build_tools as bt

target_name = 'PhoenixOS - Bootloader'

as_compiler = 'nasm'
asm_flags = '-felf32'

cc_compiler = 'clang'
c_flags = '-ffreestanding -nostdlib -m32 -fno-pic'

ld = 'ld'
linker_file = 'linker.ld'
ld_flags = '-nostdlib -melf_i386 -s'

def compile_asm(file_to_compile: str, flags: str, filename: str):
    bt.run_cmd(f'{as_compiler} {flags} {file_to_compile} -o {filename}.o')
    
def compile_c(file_to_compile: str, flags: str, filename: str):
    bt.run_cmd(f'{cc_compiler} {flags} -c {file_to_compile} -o {filename}.o')

def link_obj(root_dir: str, build_dir: str):
    bt.trace('linking object files...')
    os.chdir(root_dir)
    if not os.path.exists('iso_root'):
        os.makedirs('iso_root')
    object_files = bt.find_files(build_dir, '.o')
    obj_files = ''
    for obj in object_files:
        obj_files += f'{obj} '
    bt.run_cmd(f'{ld} {ld_flags} -T{root_dir}/{linker_file} {obj_files} -o {root_dir}/stage2.bin')
    bt.info('finished linking obj files!')

def build(root_dir: str, src_dir: str, build_dir: str):
    bt.trace(f'building {target_name}')
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    bt.compile_files(build_dir, src_dir, f'-i{src_dir} {asm_flags}', '.asm', [f'{src_dir}/stage2.asm'], compile_asm)
    bt.compile_files_ext(build_dir, src_dir, c_flags, '.c', compile_c)
    link_obj(root_dir, build_dir)
    bt.run_cmd(f'nasm -i{build_dir} -i{src_dir} {src_dir}/stage1.asm -o {build_dir}/stage1.bin')

if __name__ == "__main__":
    root_dir = os.getcwd() # get current working directory
    src_dir = f'{root_dir}/src'
    build_dir = f'{root_dir}/build'
    
    for arg_ in enumerate(sys.argv):
        arg = arg_[1]

        if arg == 'clean':
            bt.clean(root_dir, [build_dir])
        elif arg == 'build':
            build(root_dir, src_dir, build_dir)
        elif arg == 'qemu':
            bt.run_cmd(f'qemu-system-x86_64 -fda ${build_dir}/stage1.bin')
        elif arg == 'bs.py': pass
        else: bt.error("Unrecognized argument")
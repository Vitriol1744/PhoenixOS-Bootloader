import os
import sys
import build_tools as bt

target_name = 'PhoenixOS - Bootloader'

as_compiler = 'nasm'
asm_flags = '-felf32'

cc_compiler = 'clang'
c_flags = '-ffreestanding -nostdlib -fno-pic -fno-stack-protector -fshort-wchar -mno-red-zone -masm=intel -mno-sse -mno-sse2 -mno-mmx -mno-80387'

ld = 'ld'
linker_file = 'linker.ld'
ld_flags = '-nostdlib -no-pie -melf_i386 -s'

use_uefi = False

def compile_asm(input_file: str, flags: str, output_filename: str):
    bt.run_cmd(f'{as_compiler} {flags} {input_file} -o {output_filename}.o')
    
def compile_c(input_file: str, flags: str, output_filename: str):
    bt.run_cmd(f'{cc_compiler} {flags} -c -o {output_filename}.o {input_file}')

def link_obj(root_dir: str, build_dir: str):
    bt.trace('linking object files...')
    os.chdir(root_dir)

    object_files = bt.find_files(build_dir, '.o')
    obj_files = ''
    for obj in object_files:
        obj_files += f'{obj} '

    if use_uefi:
        bt.run_cmd(f'lld-link -subsystem:efi_application -nodefaultlib -dll -entry:efi_main {obj_files} -out:{build_dir}/BOOTX64.EFI')
        return
    bt.run_cmd(f'{ld} {ld_flags} -T{root_dir}/{linker_file} {obj_files} -o {root_dir}/stage2.bin')
    bt.info('finished linking obj files!')

def build(root_dir: str, src_dir: str, build_dir: str):
    bt.trace(f'building {target_name}')
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    c_internal_flags = c_flags

    if use_uefi:
        c_internal_flags = f'-DPH_USE_UEFI -I{root_dir}/gnu-efi/inc -I{root_dir}/gnu-efi/inc/x86_64 -I{root_dir}/gnu-efi/inc/protocol {c_flags} -target x86_64-pc-win32-coff'
        bt.compile_files_ext(build_dir, f'{root_dir}/gnu-efi/lib', c_internal_flags, '.c', compile_c)
    else:
        c_internal_flags += ' -m32'
        bt.compile_files(build_dir, src_dir, f'-i{src_dir} {asm_flags}', '.asm', [f'stage2.asm', 'disk.asm'], compile_asm)
    bt.compile_files_ext(build_dir, src_dir, c_internal_flags, '.c', compile_c)
    #bt.compile_files(build_dir, src_dir, f'-i{src_dir} {asm_flags}', '.asm', [f'assets.asm'], compile_asm)

    link_obj(root_dir, build_dir)
    if use_uefi:
        bt.run_cmd(f'{root_dir}/buildimg.sh')
        return
    bt.run_cmd(f'nasm -i{build_dir} -i{src_dir} {src_dir}/stage1.asm -o {build_dir}/stage1.bin')

def run(root_dir: str, build_dir: str):
    os.chdir(root_dir)
    if use_uefi:
        bt.run_cmd(f'qemu-system-x86_64 -drive file={root_dir}/fat.img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="{root_dir}/OVMF/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="{root_dir}/OVMF/OVMF_VARS-pure-efi.fd" -net none')
        pass
    else:
        bt.run_cmd(f'qemu-system-x86_64 -hdd {build_dir}/stage1.bin')

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
            run(root_dir, build_dir)
        elif arg == 'all':
            bt.clean(root_dir, [build_dir])
            build(root_dir, src_dir, build_dir)
            run(root_dir, build_dir)
        elif arg == 'bs.py': pass
        else: bt.error("Unrecognized argument")
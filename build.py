#!/usr/bin/python3
"""
TODO
- Simplify commands for basic presets.
- Improve stderr output when building as it currently hides it.  Make it clear if it fails.
    - Print out directory structure on completion
- Separate options like llvm and linker to functions.  Replace executable paths with path builder functions for llvm.

"""

import argparse
import platform
import subprocess
import multiprocessing
import os
from datetime import datetime

# https://docs.godotengine.org/en/stable/contributing/development/compiling/compiling_for_linuxbsd.html#using-system-libraries-for-faster-development
# - WARNING : When using system libraries, the resulting library is not portable across Linux distributions anymore.
#           Do not use this approach for creating binaries you intend to distribute to others, unless you're creating a package for a Linux distribution.
dynamic_link_libs="builtin_embree=no builtin_enet=no builtin_freetype=no builtin_graphite=no builtin_harfbuzz=no builtin_libogg=no builtin_libpng=no builtin_libtheora=no builtin_libvorbis=no builtin_libwebp=no builtin_mbedtls=no builtin_miniupnpc=no builtin_pcre2=no builtin_zlib=no builtin_zstd=no"

custom_modules_path=os.path.abspath("../cyborg_survivors_game/engine_custom_modules" )
custom_game_path= os.path.abspath("../cyborg_survivors_game/game" )
tracy_build_add='tracy_enable=yes CCFLAGS=\"-fno-omit-frame-pointer -fno-inline -ggdb3\"'
build_options={
    'debug':f"dev_build=yes compiledb=yes verbose=yes warnings=all tests=yes  lto=none  use_llvm=yes linker=mold {dynamic_link_libs}",
    'production':f"production=yes lto=none use_llvm=yes linker=mold debug_symbols=yes compiledb=yes {dynamic_link_libs}",
    'release_production':f"production=yes lto=full use_llvm=yes linker=mold debug_symbols=no compiledb=no "
}

template_options={
    'debug':"target=template_debug",
    'production':"target=template_release "
}

arch="x86_64"

def run_command_in_new_terminal(command):
    system_platform = platform.system()
    print(" Running Command")
    print(command)
    print("", flush=True)
    subprocess.run(command, shell=True,check=True)


def parser_init():
    parser = argparse.ArgumentParser(description="Quick Build System.")
    parser.add_argument("--os",dest="os", choices=["linuxbsd", "windows"], required=False, help="Specify the operating system type.")
    
    valid_build_options = ["binary", "templates", "release"]
    parser.add_argument(
        "--build",
        dest="build",
        choices=valid_build_options,
        default=valid_build_options[0],
        help="What to build."
    )

    valid_mode_options = ["debug", "production", "release_production"]
    parser.add_argument(
        "--mode",
        dest="mode",
        choices=valid_mode_options,
        default=valid_mode_options[0],
        help="What build settings to use."
    )

    parser.add_argument(
        "--tracy",
        dest="tracy",
        action="store_true",
        help="Build with tracy integration."
    )

    parser.add_argument("--release_dir",dest="release_dir", required=False, help="Output release dir.  Required when using 'release'.")
    args = parser.parse_args()

    os_type = platform.system()
    # Allow user to not specify machine to build on.
    if not args.os:
        if os_type == "Linux": args.os = "linuxbsd"
        elif os_type == "Windows": args.os = "windows"
        else:
            parser.error("Unknown operating system: {}".format(os_type))

    if "release" in args.build and not args.release_dir:
        parser.error("--release_dir required in release build.")
    return args


def main():
    os_type = platform.system()
    num_threads =  max(multiprocessing.cpu_count() - 2, 1)
    print(f"Numthreads={num_threads}")
    # Get user input
    args = parser_init()
    

    # Build binary - always build.
    extra_debug_options = ""
    if "debug" in args.mode:
        if os_type == "Windows": extra_debug_options += "vsproj=yes"
    print("Building binary..")
    command = f"scons platform={args.os} -j {num_threads} {extra_debug_options} {build_options[args.mode]} { tracy_build_add if args.tracy else ''} compiledb=yes custom_modules={custom_modules_path} "
    try:
        if os.path.isfile("./compile_commands.json"):
            print("Deleting compile_commands.json for clion inspector updating.")
            os.remove("./compile_commands.json")
        run_command_in_new_terminal(command)
    except subprocess.CalledProcessError as e:
        print("Error while building:")
        raise e


    # Build templates
    extra_debug_options = ""
    if any([mode in args.build for mode in ["templates", "release"] ] ):
        if "debug" in args.mode:
            if os_type == "Windows": extra_debug_options += "vsproj=yes"
        print("Building templates..")
        command = f"scons platform={args.os} -j {num_threads} {extra_debug_options} {build_options[args.mode]} { tracy_build_add if args.tracy else ''} {template_options[args.mode]} custom_modules={custom_modules_path} "
        try:
            run_command_in_new_terminal(command)
        except subprocess.CalledProcessError as e:
            print("Error while building:")
            raise e

    # Package game
    extra_debug_options = ""
    if any([mode in args.build for mode in ["release"] ] ):
        
        try:
            # Run the command and capture output
            execute_bin = f"./bin/godot.{args.os}.editor{'.dev' if 'debug' in args.mode else ''}.{arch}.llvm{'.exe' if 'windows' in args.os else ''}"
            execute_bin = os.path.abspath(execute_bin)
            release_type = '--export-release' if 'production' in args.mode else '--export-debug'

            if "linux" in args.os : godot_export = f"linux_{arch}"
            elif "windows" in args.os : godot_export = f"windows_{arch}"

            formatted_datetime = datetime.now().strftime("%Y-%m-%d.%H.%M.%S")
            build_dir = os.path.join(args.release_dir, formatted_datetime + "." + args.mode)
            build_dir = os.path.abspath(build_dir)
            os.makedirs(build_dir, exist_ok=True)
            build_dir = os.path.join(build_dir, "game")
            
            command = f"{execute_bin} --path {custom_game_path} {release_type} {godot_export} {build_dir} --verbose --debug --display-driver 'headless' "
            print("Packaging game..")
            run_command_in_new_terminal(command)
            print(f"Game saved to [ {build_dir} ]")
        except subprocess.CalledProcessError as e:
            # Handle errors, if any
            print("Error while building:")
            raise e
    
    print("-- COMPLETE --")

if __name__ == "__main__":
    main()

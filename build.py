#!/usr/bin/python3


import argparse
import platform
import subprocess
import multiprocessing
import os
from datetime import datetime

custom_modules_path=os.path.abspath("../cyborg_survivors_game/engine_custom_modules" )
custom_game_path= os.path.abspath("../cyborg_survivors_game/game" )

build_options={
    'debug':"dev_build=yes compiledb=yes verbose=yes werror=no tests=yes warnings=no lto=none",
    #'debug':"dev_mode=yes dev_build=yes",
    'production':"production=yes lto=full tests=yes"
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

    valid_mode_options = ["debug", "production"]
    parser.add_argument(
        "--mode",
        dest="mode",
        choices=valid_mode_options,
        default=valid_mode_options[0],
        help="What build settings to use."
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
    command = f"scons platform={args.os} -j {num_threads} {extra_debug_options} {build_options[args.mode]} compiledb=yes custom_modules={custom_modules_path}"
    try:
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
        command = f"scons platform={args.os} -j {num_threads} {extra_debug_options} {build_options[args.mode]} {template_options[args.mode]} custom_modules={custom_modules_path}"
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
            execute_bin = f"./bin/godot.{args.os}.editor{'.dev' if 'debug' in args.mode else ''}.{arch}{'.exe' if 'windows' in args.os else ''}"
            execute_bin = os.path.abspath(execute_bin)
            release_type = '--export-release' if 'production' in args.mode else '--export-debug'

            if "linux" in args.os : godot_export = f"linux_{arch}"
            elif "windows" in args.os : godot_export = f"windows_{arch}"

            formatted_datetime = datetime.now().strftime("%Y-%m-%d.%H.%M.%S")
            build_dir = os.path.join(args.release_dir, formatted_datetime + "." + args.mode)
            build_dir = os.path.abspath(build_dir)
            os.makedirs(build_dir, exist_ok=True)
            build_dir = os.path.join(build_dir, "game")
            
            command = f"{execute_bin} --path {custom_game_path} {release_type} {godot_export} {build_dir} --verbose --debug --display-driver 'headless'"
            print("Packaging game..")
            result = subprocess.run(command, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            print(f"Game saved to [ {build_dir} ]")
        except subprocess.CalledProcessError as e:
            # Handle errors, if any
            print("Error while building:")
            raise e
    
    print("-- COMPLETE --")

if __name__ == "__main__":
    main()

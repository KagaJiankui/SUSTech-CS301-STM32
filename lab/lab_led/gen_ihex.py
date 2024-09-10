import subprocess

Import("env")


def gen_compiledb(source, target, env):
    if "compiledb" not in COMMAND_LINE_TARGETS:  # avoids infinite recursion
        subprocess.run(["pio", "run", "-t", "compiledb"])


env.AddPostAction("buildprog", gen_compiledb)

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(
        " ".join(
            [
                "$OBJCOPY",
                "-O",
                "ihex",
                "-R",
                ".eeprom",
                "$BUILD_DIR/${PROGNAME}.elf",
                "$BUILD_DIR/${PROGNAME}.hex",
            ]
        ),
        "Building $BUILD_DIR/${PROGNAME}.hex",
    ),
)
env.AddPostAction(
    "$BUILD_DIR/compile_commands.json",
    env.VerboseAction(
        " ".join(
            [
                "cp",
                "$BUILD_DIR/compile_commands.json",
                "$PROJECT_DIR/compile_commands.json",
            ]
        ),
        "Copying $BUILD_DIR/compile_commands.json to $PROJECT_DIR/compile_commands.json",
    ),
)

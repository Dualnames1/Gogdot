#!/usr/bin/env python

env = SConscript(
    "godot-cpp/SConstruct",
    variant_dir="build",
    duplicate=False,
    exports={
        "api_version": "4.6"
    }
)

env.Append(CPPPATH=["src","godot-cpp/include","godot-cpp/gen/include","include"])
env.Append(CPPDEFINES=["GOG_GALAXY"])

sources = ["src/gog_galaxy.cpp","src/register_types.cpp"]


if env["platform"] == "windows":

    env.Append(CXXFLAGS=["/EHsc"])
    env.Append(LIBPATH=["libraries/win64"])

    env.Append(LIBS=["Galaxy64"])

    debug_library = env.SharedLibrary(target="output/gog_galaxy.windows.template_debug.x86_64.dll",source=sources)
    release_env = env.Clone()
    release_library = release_env.SharedLibrary(target="output/gog_galaxy.windows.template_release.x86_64.dll",source=sources)

    Default([debug_library,release_library])



elif env["platform"] == "macos":

    env.Append(LIBPATH=["libraries/osx"])

    env.Append(LIBS=["Galaxy"])

    debug_env = env.Clone()
    debug_env["SHLIBPREFIX"] = ""

    debug_library = debug_env.SharedLibrary(target="output/gog_galaxy.macos.template_debug.universal.dylib",source=sources)

    release_env = env.Clone()
    release_env["SHLIBPREFIX"] = ""

    release_library = release_env.SharedLibrary(target="output/gog_galaxy.macos.template_release.universal.dylib",source=sources)

    Default([debug_library,release_library])
    
elif env["platform"] == "linux":

    debug_env = env.Clone()
    debug_env["SHLIBPREFIX"] = ""
    debug_library = debug_env.SharedLibrary(target="output/gog_galaxy.linux.template_debug.x86_64.so",source=sources)

    release_env = env.Clone()
    release_env["SHLIBPREFIX"] = ""
    release_library = release_env.SharedLibrary(target="output/gog_galaxy.linux.template_release.x86_64.so",source=sources)

    Default([debug_library,release_library])
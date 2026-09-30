Gog Galaxy integration as a GDExtension for Godot.
Works on 4.6 [probably prior too, haven't tested]


How to install:
1) Download the framework from https://devportal.gog.com/galaxy/components/sdk (YOU NEED TO BE A GOG DEVELOPER)
2) Copy "Galaxy64.dll" and "libGalaxy.dylib" inside the following directory

-gog_galaxy\bin

3) Copy gog_galaxy[the folder] in your project root/addons/ directory
4) Copy gog_galaxy[the folder] in demo/addons [optional if you want the demo to work, you also need to edit the script so that client id and client secret are set, and other code too]



To build it on your own:
1) Download the framework from https://devportal.gog.com/galaxy/components/sdk
2) Copy the folder Include\galaxy into include\galaxy
3) Copy "Galaxy64.dll", "Galaxy64.lib", and "Galaxy64.pdb" to libraries\win64
4) Copy "Galaxy.dll", "Galaxy.lib", and "Galaxy.pdb" to libraries\win32
5) Copy "libGalaxy.dylib" to libraries\osx [that's for MAC, so skip on win]
6) Run "scons" via Powershell(windows) or Terminal(OSX) on the directory you downloaded/cloned the repo files.

The libraries will be found afterwards in output.

The plugin was specifically created to allow GOG integration for Nighthawks!

Games currently utilizing it:
- Nighthawks: https://www.gog.com/en/game/nighthawks
  Platforms: Windows · Linux · macOS

#include "register_types.h"
#include "gog_galaxy.h"

using namespace godot;

void initialize_gog_galaxy(ModuleInitializationLevel level)
{
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
    ClassDB::register_class<GOGGalaxy>();
}

void uninitialize_gog_galaxy(ModuleInitializationLevel level)
{
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}

extern "C"
{
	GDExtensionBool GDE_EXPORT gog_galaxy_library_init(GDExtensionInterfaceGetProcAddress get_proc_address,GDExtensionClassLibraryPtr library,GDExtensionInitialization *initialization)
	{
		GDExtensionBinding::InitObject init_obj(get_proc_address,library,initialization);
		init_obj.register_initializer(initialize_gog_galaxy);
		init_obj.register_terminator(uninitialize_gog_galaxy);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
		return init_obj.init();
	}

}
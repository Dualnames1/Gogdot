#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>

#include <galaxy/GalaxyApi.h>

namespace godot {

class GOGGalaxy : public RefCounted {
    GDCLASS(GOGGalaxy, RefCounted);

private:
    static GOGGalaxy *singleton;

    bool initialized = false;

protected:
    static void _bind_methods();

public:
    GOGGalaxy();
    ~GOGGalaxy();

    static GOGGalaxy *get_singleton();

    bool initialize(String p_client_id, String p_client_secret);
    void shutdown();
    bool is_available() const;
    bool is_initialized() const;
    bool is_signed_in() const;
    bool is_logged_in() const;
    bool sign_in();
    void sign_out();
    void process();
    bool SetAchievement(String achievement_id, bool state);
	bool GetAchievement(String achievement_id);
};

}
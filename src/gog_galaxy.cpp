#include "gog_galaxy.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GOGGalaxy *GOGGalaxy::singleton = nullptr;

GOGGalaxy::GOGGalaxy() 
{
    singleton = this;
}

GOGGalaxy::~GOGGalaxy() 
{
    shutdown();
    if (singleton == this) singleton = nullptr;    
}

GOGGalaxy *GOGGalaxy::get_singleton()
{
    return singleton;
}

void GOGGalaxy::_bind_methods() 
{
    ClassDB::bind_method(D_METHOD("initialize", "client_id", "client_secret"),&GOGGalaxy::initialize);
    ClassDB::bind_method(D_METHOD("shutdown"),&GOGGalaxy::shutdown);
    ClassDB::bind_method(D_METHOD("is_available"),&GOGGalaxy::is_available);
    ClassDB::bind_method(D_METHOD("is_initialized"),&GOGGalaxy::is_initialized);
    ClassDB::bind_method(D_METHOD("is_logged_in"),&GOGGalaxy::is_logged_in);
    ClassDB::bind_method(D_METHOD("process"),&GOGGalaxy::process);
    ClassDB::bind_method(D_METHOD("SetAchievement", "achievement_id","state"),&GOGGalaxy::SetAchievement);
	ClassDB::bind_method(D_METHOD("GetAchievement", "achievement_id"),&GOGGalaxy::GetAchievement);
}

bool GOGGalaxy::initialize(String p_client_id, String p_client_secret) 
{
	#if defined(_WIN32) || defined(__APPLE__)

    if (initialized) return true;
    

    if (p_client_id.is_empty() || p_client_secret.is_empty()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: Client ID or Client Secret is empty.");
        return false;
    }

    CharString client_ = p_client_id.utf8();
	CharString clientsecret_ = p_client_secret.utf8();
	
	galaxy::api::InitOptions options(client_.get_data(),clientsecret_.get_data());
	
	galaxy::api::Init(options);
	galaxy::api::User()->SignInGalaxy();
	initialized = true;
	
	UtilityFunctions::print("GOG Galaxy initialized successfully.");
	
	return true;
	
	#else

    initialized = false;
    UtilityFunctions::print("GOG Galaxy is not available on this platform.");

    return false;

	#endif
}

void GOGGalaxy::shutdown() {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized)return;
	galaxy::api::Shutdown();
	UtilityFunctions::print("GOG Galaxy shut down.");
    
#endif
	initialized = false;
}

bool GOGGalaxy::is_available() const {
#if defined(_WIN32) || defined(__APPLE__)
    return true;
#else
    return false;
#endif
}

bool GOGGalaxy::is_initialized() const {
    return initialized;
}

bool GOGGalaxy::is_logged_in() const {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) return false;
	return galaxy::api::User()->IsLoggedOn();    

#else
    return false;
#endif
}


bool requestUSA=false;

void GOGGalaxy::process() {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) return;
    

    galaxy::api::ProcessData();
	bool islogged=galaxy::api::User()->IsLoggedOn();
	bool issignedin=galaxy::api::User()->SignedIn();
	if (islogged && issignedin && !requestUSA)
	{
		galaxy::api::Stats()->RequestUserStatsAndAchievements();
		requestUSA=true;
	}

#endif
}

bool GOGGalaxy::SetAchievement(String achievement_id, bool state) {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) 
	{
        UtilityFunctions::printerr("GOG Galaxy: cannot unlock achievement because Galaxy is not initialized.");
		return false;
    }

    if (achievement_id.is_empty()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: achievement ID is empty.");
        return false;
    }

    if (!is_logged_in()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: cannot unlock achievement because the user is not logged in.");
        return false;
    }
	
	CharString achievement_ = achievement_id.utf8();
	
	if (state)
	{
        galaxy::api::Stats()->SetAchievement(achievement_.get_data());
	}
	else
	{
		galaxy::api::Stats()->ClearAchievement(achievement_.get_data());
	}
	galaxy::api::Stats()->StoreStatsAndAchievements();
	UtilityFunctions::print(String("GOG Galaxy: achievement submitted: ")+ achievement_id);
	
	return true;	
#else
    return false;
#endif
}

bool GOGGalaxy::GetAchievement(String achievement_id) {
    #if defined(_WIN32) || defined(__APPLE__)
	if (!initialized || !galaxy::api::User()->IsLoggedOn()) return false;
	
    CharString achievement_ = achievement_id.utf8();
    bool unlocked = false;
    uint32_t unlock_time = 0;

    galaxy::api::Stats()->GetAchievement(achievement_.get_data(),unlocked,unlock_time);

    return unlocked;
	
	#else
		return false;
	#endif
}
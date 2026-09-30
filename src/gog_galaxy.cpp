#include "gog_galaxy.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GOGGalaxy *GOGGalaxy::singleton = nullptr;
bool requestData=false;
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
    ClassDB::bind_method(D_METHOD("Initialize", "client_id", "client_secret"),&GOGGalaxy::Initialize);
    ClassDB::bind_method(D_METHOD("shutdown"),&GOGGalaxy::shutdown);
    ClassDB::bind_method(D_METHOD("is_available"),&GOGGalaxy::is_available);
    ClassDB::bind_method(D_METHOD("is_initialized"),&GOGGalaxy::is_initialized);
    ClassDB::bind_method(D_METHOD("is_logged_in"),&GOGGalaxy::is_logged_in);
    ClassDB::bind_method(D_METHOD("Update"),&GOGGalaxy::Update);
    ClassDB::bind_method(D_METHOD("RequestStatsAndAchievements"),&GOGGalaxy::RequestStatsAndAchievements);
    ClassDB::bind_method(D_METHOD("SetAchievement", "achievement_id","state"),&GOGGalaxy::SetAchievement);
	ClassDB::bind_method(D_METHOD("GetAchievement", "achievement_id"),&GOGGalaxy::GetAchievement);
	ClassDB::bind_method(D_METHOD("SetStatInt", "stat_id","value"),&GOGGalaxy::SetStatInt);
	ClassDB::bind_method(D_METHOD("GetStatInt", "stat_id"),&GOGGalaxy::GetStatInt);
	ClassDB::bind_method(D_METHOD("SetStatFloat", "stat_id","value"),&GOGGalaxy::SetStatFloat);
	ClassDB::bind_method(D_METHOD("GetStatFloat", "stat_id"),&GOGGalaxy::GetStatFloat);
}

bool GOGGalaxy::Initialize(String client_id, String client_secret) 
{
	#if defined(_WIN32) || defined(__APPLE__)

    if (initialized) return true;
    

    if (client_id.is_empty() || client_secret.is_empty()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: Client ID or Client Secret is empty.");
        return false;
    }

    CharString client_ = client_id.utf8();
	CharString clientsecret_ = client_secret.utf8();
	
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
	requestData=false;
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


void GOGGalaxy::Update() {//this basically needs to run to process Data on gog side. It requests data the first time it runs.
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) return;
    
    galaxy::api::ProcessData();
	bool islogged=galaxy::api::User()->IsLoggedOn();
	if (islogged && !requestData)
	{
		galaxy::api::Stats()->RequestUserStatsAndAchievements();
		requestData=true;
	}

#endif
}

void GOGGalaxy::RequestStatsAndAchievements() {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) return;
    
	bool islogged=galaxy::api::User()->IsLoggedOn();
	if (islogged)
	{
		galaxy::api::Stats()->RequestUserStatsAndAchievements();
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


bool GOGGalaxy::SetStatInt(String stat_id, int value) {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) 
	{
        UtilityFunctions::printerr("GOG Galaxy: cannot set stat cause GOG Galaxy is not initialized.");
		return false;
    }

    if (stat_id.is_empty()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: stat ID is empty.");
        return false;
    }

    if (!is_logged_in()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: cannot set stat because the user is not logged in.");
        return false;
    }
	
	CharString stat_ = stat_id.utf8();
	
	galaxy::api::Stats()->SetStatInt(stat_.get_data(),value);
	galaxy::api::Stats()->StoreStatsAndAchievements();
	UtilityFunctions::print(String("GOG Galaxy: stat set: ")+ stat_id, value);
	
	return true;	
#else
    return false;
#endif
}

int GOGGalaxy::GetStatInt(String stat_id) {
    #if defined(_WIN32) || defined(__APPLE__)
	if (!initialized || !galaxy::api::User()->IsLoggedOn()) return -1;
	if (stat_id.is_empty()) return -1;
    CharString stat_ = stat_id.utf8();

    return galaxy::api::Stats()->GetStatInt(stat_.get_data());
	
	#else
		return -1;
	#endif
}

bool GOGGalaxy::SetStatFloat(String stat_id, float value) {
#if defined(_WIN32) || defined(__APPLE__)

    if (!initialized) 
	{
        UtilityFunctions::printerr("GOG Galaxy: cannot set stat cause GOG Galaxy is not initialized.");
		return false;
    }

    if (stat_id.is_empty()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: stat ID is empty.");
        return false;
    }

    if (!is_logged_in()) 
	{
        UtilityFunctions::printerr("GOG Galaxy: cannot set stat because the user is not logged in.");
        return false;
    }
	
	CharString stat_ = stat_id.utf8();
	
	galaxy::api::Stats()->SetStatFloat(stat_.get_data(),value);
	galaxy::api::Stats()->StoreStatsAndAchievements();
	UtilityFunctions::print(String("GOG Galaxy: stat set: ")+ stat_id, value);
	
	return true;	
#else
    return false;
#endif
}

float GOGGalaxy::GetStatFloat(String stat_id) {
    #if defined(_WIN32) || defined(__APPLE__)
	if (!initialized || !galaxy::api::User()->IsLoggedOn()) return -1.0f;
	if (stat_id.is_empty()) return -1.0f;
    CharString stat_ = stat_id.utf8();

    return galaxy::api::Stats()->GetStatFloat(stat_.get_data());
	
	#else
		return -1;
	#endif
}

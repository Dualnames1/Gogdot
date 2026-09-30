extends Node

var galaxy

@onready var status_label = $StatusLabel

func _ready():
	status_label.text = "GOG Galaxy: starting..."

	galaxy = GOGGalaxy.new()

	if galaxy == null:
		status_label.text = "ERROR: Could not create GOGGalaxy"
		return

	status_label.text = "GOGGalaxy created\nInitializing..."
	var clientid="CLIENT ID GOES HERE"
	var clientsecret="CLIENT SECRET"

	var success = galaxy.initialize(clientid, clientsecret)

	if success:
		status_label.text = "Galaxy initialized!\nWaiting for authentication..."
	else:
		status_label.text = "Galaxy initialization FAILED"


func _process(_delta):
	if galaxy == null:
		status_label.text = "ERROR: Galaxy is NULL"
		return
	
	galaxy.process()

	if galaxy.is_logged_in():
		status_label.text = "GOG Galaxy\n\n✓ LOGGED IN \nAchievement HEAR ME:"+str(galaxy.GetAchievement("HEAR_ME"))+"\nAchievement HANG OF IT:"+str(galaxy.GetAchievement("HANG_OF_IT"));
		
	else:
		status_label.text = "GOG Galaxy\n\nWaiting for authentication..."

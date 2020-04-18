extends Control

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var myHpVal
var myHpBar
var myGoldCounter
var mySwordIcon
var myGliderIcon
var myFamiliarBellIcon
var myRoundNumber

# Called when the node enters the scene tree for the first time.
func _ready():
	if(get_node("/root/Network").get("singlePlayerMode")):
		get_node("GUI/HBoxContainer/Rows/Items/RoundNumber").show()
	myHpVal = get_node("GUI/HBoxContainer/Rows/HBoxContainer/LifeBar/Count/Background/Label")
	myHpBar = get_node("GUI/HBoxContainer/Rows/HBoxContainer/LifeBar/Guage")
	myGoldCounter = get_node("GUI/HBoxContainer/Rows/Items/HBoxContainer/GoldCounter/Background/Number")
	mySwordIcon = get_node("GUI/HBoxContainer/Rows/Items/HBoxContainer/SwordItem")
	myGliderIcon = get_node("GUI/HBoxContainer/Rows/Items/HBoxContainer/GliderItem")
	myFamiliarBellIcon = get_node("GUI/HBoxContainer/Rows/Items/HBoxContainer/FamiliarBellItem")
	myRoundNumber = get_node("GUI/HBoxContainer/Rows/Items/RoundNumber/Background/Label")
	
func _on_hp_changed(newHp, maxHp):
	var mNewHp = float(newHp)
	var mMaxHp = float(maxHp)
	var percentHp = mNewHp / mMaxHp
	percentHp *= 100
	var intPercentHp = int(percentHp)
	myHpVal.set_text(String(newHp))
	myHpBar.value = intPercentHp
	
func _on_gold_count_changed(newGold):
	myGoldCounter.set_text(String(newGold))
	
func _on_item_collected(itemType):
	if(itemType == 0):
		myGliderIcon.show()
	elif(itemType == 1):
		mySwordIcon.show()
	elif(itemType == 2):
		myFamiliarBellIcon.show()
		
func set_round_num(newRound):
	myRoundNumber.set_text("Round " + String(newRound))

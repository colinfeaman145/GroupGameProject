Press F2 to access testing room


Take inspiration from 
- Risk of Rain 2
- Isaac
- Enter the gungeon
- Ravenswatch



# How to add items

## Create a new class derived from ItemEffect

1. Create a new class and put the .cpp file into the Items folder

2. Override the 4 functions from ItemEffect 
```c++
<ItemId_?>
class ItemId_? : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) override;
	void OnRemove(Attackable* owner, int stacks) override;
	void OnModifyStats(StatSheet& stats, int stacks)  override;
	void OnEvent(EventType type, EventContext ctx, int stacks) override;
};
```
(The name of the class should represent the items ID; the IDs are used in the code to refer to a specific item so please dont change existing ones.)

3. Implement the 4 classes in it corresponding .cpp file.

Do you want to make the item just change stats or does it trigger on specific events? 
Does the item only have effects on pickup or deletion?
Depending on you item idea implement the function you need.

Example stat changing item:
```c++
#include "StatSheet.hpp"


void ItemId_1::OnPickup(Attackable* owner, int stacks) {
}

void ItemId_1::OnRemove(Attackable* owner, int stacks) {
}

// adds base health per stack
void ItemId_1::OnModifyStats(StatSheet& stats, int stacks) {
	auto baseHealth = data["params"]["baseHealth"].get<float>();
	auto increasePerStack = data["params"]["increasePerStack"].get<float>();
	stats.bonusHealth = ItemEffect::GetLinearStackingItemValue(baseHealth, increasePerStack, stacks);
}

void ItemId_1::OnEvent(EventType type, EventContext ctx, int stacks) {

}
```


Example event based item:

```c++
void InvincibleModule::OnPickup(Attackable* owner, int stacks) {
}

void InvincibleModule::OnRemove(Attackable* owner, int stacks) {
}

void InvincibleModule::OnModifyStats(StatSheet& stats, int stacks) {
}

// makes the owner invincible for a short amout of time (configurable in json file)
// prevents the system to apply damage to the player every frame
void InvincibleModule::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type != EventType::OnGettigHit) return;
	ctx.target->ApplyStatusEffect(StatusEffectType::Invincible, data["params"]["duration"], ctx.target);
}
```
For event based items, you have to check what event is called. If you don't, the item will trigger at any event called.



## Add item in GameSetupItemRegistry.cpp file

After implementing it, you add it to the Item Registry.
It contains all items in the game and lives in the GameContext.hpp. From there any item definition can be reveived.

Add a additional entry like this:
```c++
	context.ir->RegisterItem({
		.id = 6,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_6>(data["6"]),
		.data = data["6"]
	});
```

Change the ID to match you item and !SWAP OUT THE TEMPLATE TYPE! (the datatype in <>).


## Add item to items.json file

Last add a new entry into the items.json file.
All values other than the ones in the params should be the same.
In the params you can put in your own data key-values.
These values can be accessed in each item implementation using its data member variable. (data["params"]["*key*"])
```json
"1": {
    "id": 1,
    "name": "Gun",
    "description": "A simple gun you can shoot with",
    "spritePath": "..\\..\\assets\\sprites\\Items\\item1.png",
    "params": {
      "pierceCount": 1,
      "ttl": 5,
      "bulletSpeed": 5000
    }
```
 


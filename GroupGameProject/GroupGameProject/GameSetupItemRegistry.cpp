#include "Game.hpp"

#include <fstream>
#include "ItemRegistry.hpp"
#include "json.hpp"
#include "GameContext.hpp"

#include "Item.hpp"
#include "Items.hpp"


using json = nlohmann::json;

void Game::SetupItemRegistry(const std::string& filepath) {

	ifstream file(filepath);
	if (!file.is_open()) {
		cerr << "Failed to open items file: " << filepath << endl;
		return;
	}
	json data = json::parse(file);

	context.ir->RegisterItem({
		.id = 1,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_1>(data["1"]),
		.data = data["1"]
		});
	context.ir->RegisterItem({
		.id = 3,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_3>(data["3"]),
		.data = data["3"]
		});
	context.ir->RegisterItem({
		.id = 4,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_4>(data["4"]),
		.data = data["4"]
		});
	context.ir->RegisterItem({
		.id = 5,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_5>(data["5"]),
		.data = data["5"]
		});
	context.ir->RegisterItem({
		.id = 6,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_6>(data["6"]),
		.data = data["6"]
		});
	context.ir->RegisterItem({
		.id = 7,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_7>(data["7"]),
		.data = data["7"]
		});
	context.ir->RegisterItem({
		.id = 8,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_8>(data["8"]),
		.data = data["8"]
		});
	context.ir->RegisterItem({
		.id = 9,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_9>(data["9"]),
		.data = data["9"]
		});
	context.ir->RegisterItem({
		.id = 10,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_10>(data["10"]),
		.data = data["10"]
		});
	context.ir->RegisterItem({
		.id = 11,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_11>(data["11"]),
		.data = data["11"]
		});
	context.ir->RegisterItem({
		.id = 12,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_12>(data["12"]),
		.data = data["12"]
		});
	context.ir->RegisterItem({
		.id = 13,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_13>(data["13"]),
		.data = data["13"]
		});
	context.ir->RegisterItem({
		.id = 14,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_14>(data["14"]),
		.data = data["14"]
		});
	context.ir->RegisterItem({
		.id = 15,
		.tier = ItemTier::Legendary,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_15>(data["15"]),
		.data = data["15"]
		});
	context.ir->RegisterItem({
		.id = 16,
		.tier = ItemTier::Legendary,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_16>(data["16"]),
		.data = data["16"]
		});
	context.ir->RegisterItem({
		.id = 17,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_17>(data["17"]),
		.data = data["17"]
		});
	context.ir->RegisterItem({
		.id = 18,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_18>(data["18"]),
		.data = data["18"]
		});
	context.ir->RegisterItem({
		.id = 19,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_19>(data["19"]),
		.data = data["19"]
		});
	context.ir->RegisterItem({
		.id = 20,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_20>(data["20"]),
		.data = data["20"]
		});
	context.ir->RegisterItem({
		.id = 21,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_21>(data["21"]),
		.data = data["21"]
		});
	context.ir->RegisterItem({
		.id = 22,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_22>(data["22"]),
		.data = data["22"]
		});
	context.ir->RegisterItem({
		.id = 23,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_23>(data["23"]),
		.data = data["23"]
		});
	context.ir->RegisterItem({
		.id = 24,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_24>(data["24"]),
		.data = data["24"]
		});
	context.ir->RegisterItem({
		.id = 25,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_25>(data["25"]),
		.data = data["25"]
		});
	context.ir->RegisterItem({
		.id = 26,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_26>(data["26"]),
		.data = data["26"]
		});
	context.ir->RegisterItem({
		.id = 27,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_27>(data["27"]),
		.data = data["27"]
		});
	context.ir->RegisterItem({
		.id = 28,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_28>(data["28"]),
		.data = data["28"]
		});
	context.ir->RegisterItem({
		.id = 29,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_29>(data["29"]),
		.data = data["29"]
		});
	context.ir->RegisterItem({
		.id = 30,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_30>(data["30"]),
		.data = data["30"]
		});
	context.ir->RegisterItem({
		.id = 31,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_31>(data["31"]),
		.data = data["31"]
		});
	context.ir->RegisterItem({
		.id = 32,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_32>(data["32"]),
		.data = data["32"]
		});
	context.ir->RegisterItem({
		.id = 33,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_33>(data["33"]),
		.data = data["33"]
		});
	context.ir->RegisterItem({
		.id = 34,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_34>(data["34"]),
		.data = data["34"]
		});
	context.ir->RegisterItem({
		.id = 35,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_35>(data["35"]),
		.data = data["35"]
		});
	context.ir->RegisterItem({
		.id = 36,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_36>(data["36"]),
		.data = data["36"]
		});
	context.ir->RegisterItem({
		.id = 37,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_37>(data["37"]),
		.data = data["37"]
		});
	context.ir->RegisterItem({
		.id = 38,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_38>(data["38"]),
		.data = data["38"]
		});
	context.ir->RegisterItem({
		.id = 39,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_39>(data["39"]),
		.data = data["39"]
		});
	context.ir->RegisterItem({
		.id = 40,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_40>(data["40"]),
		.data = data["40"]
		});
	context.ir->RegisterItem({
		.id = 41,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_41>(data["41"]),
		.data = data["41"]
		});
	context.ir->RegisterItem({
		.id = 42,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_42>(data["42"]),
		.data = data["42"]
		});
	context.ir->RegisterItem({
		.id = 43,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_43>(data["43"]),
		.data = data["43"]
		});
	context.ir->RegisterItem({
		.id = 44,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_44>(data["44"]),
		.data = data["44"]
		});
	context.ir->RegisterItem({
		.id = 45,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_45>(data["45"]),
		.data = data["45"]
		});
	context.ir->RegisterItem({
		.id = 46,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_46>(data["46"]),
		.data = data["46"]
		});
	context.ir->RegisterItem({
		.id = 47,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_47>(data["47"]),
		.data = data["47"]
		});
	context.ir->RegisterItem({
		.id = 48,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_48>(data["48"]),
		.data = data["48"]
		});
	context.ir->RegisterItem({
		.id = 49,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_49>(data["49"]),
		.data = data["49"]
		});
	context.ir->RegisterItem({
		.id = 50,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_50>(data["50"]),
		.data = data["50"]
		});
	context.ir->RegisterItem({
		.id = 51,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_51>(data["51"]),
		.data = data["51"]
		});
	context.ir->RegisterItem({
		.id = 52,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_52>(data["52"]),
		.data = data["52"]
		});
	context.ir->RegisterItem({
		.id = 53,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_53>(data["53"]),
		.data = data["53"]
		});
	context.ir->RegisterItem({
		.id = 54,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_54>(data["54"]),
		.data = data["54"]
		});
	context.ir->RegisterItem({
		.id = 55,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_55>(data["55"]),
		.data = data["55"]
		});
	context.ir->RegisterItem({
		.id = 56,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_56>(data["56"]),
		.data = data["56"]
		});
	context.ir->RegisterItem({
		.id = 57,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_57>(data["57"]),
		.data = data["57"]
		});
	context.ir->RegisterItem({
		.id = 58,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_58>(data["58"]),
		.data = data["58"]
		});
	context.ir->RegisterItem({
		.id = 59,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_59>(data["59"]),
		.data = data["59"]
		});
	context.ir->RegisterItem({
		.id = 60,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_60>(data["60"]),
		.data = data["60"]
		});
	context.ir->RegisterItem({
		.id = 61,
		.tier = ItemTier::Legendary,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_61>(data["61"]),
		.data = data["61"]
		});
	context.ir->RegisterItem({
		.id = 62,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_62>(data["62"]),
		.data = data["62"]
		});
	context.ir->RegisterItem({
		.id = 63,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_62>(data["63"]),
		.data = data["63"]
		});
	context.ir->RegisterItem({
		.id = 64,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_64>(data["64"]),
		.data = data["64"]
		});
	context.ir->RegisterItem({
		.id = 65,
		.tier = ItemTier::Legendary,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_65>(data["65"]),
		.data = data["65"]
		});
	context.ir->RegisterItem({
		.id = 66,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_66>(data["66"]),
		.data = data["66"]
		});
	context.ir->RegisterItem({
			.id = 67,
			.tier = ItemTier::Utility,
			.effect = ItemEffect::CreateItemEffectFromJson<ItemId_67>(data["67"]),
			.data = data["67"]
		});
	context.ir->RegisterItem({
		.id = 68,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_68>(data["68"]),
		.data = data["68"]
		});
	context.ir->RegisterItem({
		.id = 69,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_69>(data["69"]),
		.data = data["69"]
		});
	context.ir->RegisterItem({
		.id = 70,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_70>(data["70"]),
		.data = data["70"]
		});
	context.ir->RegisterItem({
		.id = 71,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_71>(data["71"]),
		.data = data["71"]
		});
	context.ir->RegisterItem({
		.id = 72,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_72>(data["72"]),
		.data = data["72"]
		});
	context.ir->RegisterItem({
		.id = 73,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_73>(data["73"]),
		.data = data["73"]
		});
	context.ir->RegisterItem({
		.id = 74,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_74>(data["74"]),
		.data = data["74"]
		});
	context.ir->RegisterItem({
		.id = 75,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_74>(data["75"]),
		.data = data["75"]
		});
	context.ir->RegisterItem({
		.id = 76,
		.tier = ItemTier::Epic,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_74>(data["76"]),
		.data = data["76"]
		});
}
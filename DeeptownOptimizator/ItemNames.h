#pragma once
#include <vector>
#include <sstream>

#define AWESOME_MAKE_ENUM(name, ...) enum class name { __VA_ARGS__, __COUNT}; \
inline std::ostream& operator<<(std::ostream& os, name value) { \
std::string enumName = #name; \
std::string str = #__VA_ARGS__; \
int len = str.length(); \
std::vector<std::string> strings; \
std::ostringstream temp; \
for(int i = 0; i < len; i ++) { \
if(isspace(str[i])) continue; \
else if(str[i] == ',') { \
strings.push_back(temp.str()); \
temp.str(std::string());\
} \
else temp<< str[i]; \
} \
strings.push_back(temp.str()); \
os << enumName << "::" << strings[static_cast<int>(value)]; \
return os;}


AWESOME_MAKE_ENUM(ItemName,
	COAL,
	COPPER,
	IRON,
	AMBER,
	ALUMINIUM,
	SILVER,
	GOLD,
	EMERALD,
	PLATINUM,
	TOPAZ,
	RUBY,
	SAPPHIRE,
	AMETHYST,
	DIAMOND,
	SULFUR,
	SILICON,
	SODIUM,
	OIL,

	COPPER_BAR,
	IRON_BAR,
	GLASS,
	ALUMINIUM_BAR,
	STEEL_BAR,
	SILVER_BAR,
	GOLD_BAR,
	STEEL_PLATE,

	GRAPHITE,
	COPPER_NAIL,
	WIRE,
	BATTERY,
	CIRCUITS,
	LAMP,
	LAB_FLASK,
	AMBER_CHARGER,
	ALUMINIUM_BOTTLE,
	AMBER_INSULATION,
	INSULATED_WIRE,
	GREEN_LASER,
	DIAMOND_CUTTER,
	MOTHERBOARD,
	SOLID_PROPELLANT,
	ACCUMULATOR,
	SOLAR_PANEL,

	POLISHED_AMBER,
	POLISHED_EMERALD,
	POLISHED_TOPAZ,
	POLISHED_RUBY,
	POLISHED_SAPPHIRE,
	POLISHED_AMETHYST,
	POLISHED_DIAMOND,
	AMBER_BRACELET,
	EMERALD_RING,
	WATER,
	TREE_SEED,
	LIANA_SEED,
	GRAPE_SEED,

	WOOD,
	LIANA,
	GRAPE,

	CLEAN_WATER,
	HYDROGEN,
	OXYGEN,
	RUBBER,
	SULFRIC_ACID,
	ETHANOL,
	REFINED_OIL,
	PLASTIC,

	ALEXANDRITE,
	URANIUM,
	TITANIUM,
	POLISHED_ALEXANDRITE,
	GUNPOWDER,
	TITANIUM_ORE,
	TITANIUM_BAR,
	DIETHYL_ETHER,
	URANIUM_ROD,
	GEAR,
	BOMB

);
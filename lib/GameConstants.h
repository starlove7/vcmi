/*
 * GameConstants.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#pragma once

#include "ConstTransitivePtr.h"

VCMI_LIB_NAMESPACE_BEGIN

class Artifact;
class ArtifactService;
class Creature;
class CreatureService;

namespace spells
{
	class Spell;
	class Service;
}

class CArtifact;
class CArtifactInstance;
class CCreature;
class CHero;
class CSpell;
class CSkill;
class CGameInfoCallback;
class CNonConstInfoCallback;


struct IdTag
{};

namespace GameConstants
{
	DLL_LINKAGE extern const std::string VCMI_VERSION;

	constexpr int PUZZLE_MAP_PIECES = 48;

	constexpr int MAX_HEROES_PER_PLAYER = 8;
	constexpr int AVAILABLE_HEROES_PER_PLAYER = 2;

	constexpr int ALL_PLAYERS = 255; //bitfield

	constexpr int CREATURES_PER_TOWN = 7; //without upgrades
	constexpr int SPELL_LEVELS = 5;
	constexpr int SPELL_SCHOOL_LEVELS = 4;
	constexpr int DEFAULT_SCHOOLS = 4;
	constexpr int CRE_LEVELS = 10; // number of creature experience levels

	constexpr int HERO_GOLD_COST = 2500;
	constexpr int SPELLBOOK_GOLD_COST = 500;
	constexpr int SKILL_GOLD_COST = 2000;
	constexpr int BATTLE_SHOOTING_PENALTY_DISTANCE = 10; //if the distance is > than this, then shooting stack has distance penalty
	constexpr int BATTLE_SHOOTING_RANGE_DISTANCE = std::numeric_limits<uint8_t>::max(); // used when shooting stack has no shooting range limit
	constexpr int ARMY_SIZE = 7;
	constexpr int SKILL_PER_HERO = 8;
	constexpr ui32 HERO_HIGH_LEVEL = 10; // affects primary skill upgrade order

	constexpr int SKILL_QUANTITY=28;
	constexpr int PRIMARY_SKILLS=4;
	constexpr int RESOURCE_QUANTITY=8;
	constexpr int HEROES_PER_TYPE=8; //amount of heroes of each type

	// amounts of OH3 objects. Can be changed by mods, should be used only during H3 loading phase
	constexpr int F_NUMBER = 9;
	constexpr int ARTIFACTS_QUANTITY=171;
	constexpr int HEROES_QUANTITY=156;
	constexpr int SPELLS_QUANTITY=70;
	constexpr int CREATURES_COUNT = 197;

	constexpr ui32 BASE_MOVEMENT_COST = 100; //default cost for non-diagonal movement

	constexpr int HERO_PORTRAIT_SHIFT = 9;// 2 special frames + 7 extra portraits

	constexpr std::array<int, 11> POSSIBLE_TURNTIME = {1, 2, 4, 6, 8, 10, 15, 20, 25, 30, 0};
}

#define ID_LIKE_CLASS_COMMON(CLASS_NAME, ENUM_NAME)	\
constexpr CLASS_NAME(const CLASS_NAME & other) = default;	\
constexpr CLASS_NAME & operator=(const CLASS_NAME & other) = default;	\
explicit constexpr CLASS_NAME(si32 id)				\
	: num(static_cast<ENUM_NAME>(id))				\
{}													\
constexpr operator ENUM_NAME() const				\
{													\
	return num;										\
}													\
constexpr si32 getNum() const						\
{													\
	return static_cast<si32>(num);					\
}													\
constexpr ENUM_NAME toEnum() const					\
{													\
	return num;										\
}													\
template <typename Handler> void serialize(Handler &h, const int version)	\
{													\
	h & num;										\
}													\
constexpr CLASS_NAME & advance(int i)				\
{													\
	num = static_cast<ENUM_NAME>(static_cast<int>(num) + i);		\
	return *this;									\
}


// Operators are performance-critical and to be inlined they must be in header
#define ID_LIKE_OPERATORS_INTERNAL(A, B, AN, BN)	\
STRONG_INLINE constexpr bool operator==(const A & a, const B & b)	\
{													\
	return AN == BN ;								\
}													\
STRONG_INLINE constexpr bool operator!=(const A & a, const B & b)	\
{													\
	return AN != BN ;								\
}													\
STRONG_INLINE constexpr bool operator<(const A & a, const B & b)	\
{													\
	return AN < BN ;								\
}													\
STRONG_INLINE constexpr bool operator<=(const A & a, const B & b)	\
{													\
	return AN <= BN ;								\
}													\
STRONG_INLINE constexpr bool operator>(const A & a, const B & b)	\
{													\
	return AN > BN ;								\
}													\
STRONG_INLINE constexpr bool operator>=(const A & a, const B & b)	\
{													\
	return AN >= BN ;								\
}

#define ID_LIKE_OPERATORS(CLASS_NAME, ENUM_NAME)	\
	ID_LIKE_OPERATORS_INTERNAL(CLASS_NAME, CLASS_NAME, a.num, b.num)	\
	ID_LIKE_OPERATORS_INTERNAL(CLASS_NAME, ENUM_NAME, a.num, b)	\
	ID_LIKE_OPERATORS_INTERNAL(ENUM_NAME, CLASS_NAME, a, b.num)


#define INSTID_LIKE_CLASS_COMMON(CLASS_NAME, NUMERIC_NAME)	\
public:														\
constexpr CLASS_NAME(const CLASS_NAME & other):						\
	BaseForID<CLASS_NAME, NUMERIC_NAME>(other)				\
{															\
}															\
constexpr CLASS_NAME & operator=(const CLASS_NAME & other) = default;	\
constexpr CLASS_NAME & operator=(NUMERIC_NAME other) { num = other; return *this; };	\
explicit constexpr CLASS_NAME(si32 id = -1)								\
	: BaseForID<CLASS_NAME, NUMERIC_NAME>(id)				\
{}

template < typename Derived, typename NumericType>
class BaseForID : public IdTag
{
protected:
	NumericType num;

public:
	constexpr NumericType getNum() const
	{
		return num;
	}

	//to make it more similar to IDLIKE
	constexpr NumericType toEnum() const
	{
		return num;
	}

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & num;
	}

	constexpr explicit BaseForID(NumericType _num = -1) :
		num(_num)
	{
	}

	constexpr void advance(int change)
	{
		num += change;
	}

	constexpr bool operator == (const BaseForID & b) const { return num == b.num; }
	constexpr bool operator <= (const BaseForID & b) const { return num <= b.num; }
	constexpr bool operator >= (const BaseForID & b) const { return num >= b.num; }
	constexpr bool operator != (const BaseForID & b) const { return num != b.num; }
	constexpr bool operator <  (const BaseForID & b) const { return num <  b.num; }
	constexpr bool operator >  (const BaseForID & b) const { return num >  b.num; }

	constexpr BaseForID & operator++() { ++num; return *this; }

	constexpr operator NumericType() const
	{
		return num;
	}
};

template<typename Der, typename Num>
std::ostream & operator << (std::ostream & os, BaseForID<Der, Num> id);

template<typename Der, typename Num>
std::ostream & operator << (std::ostream & os, BaseForID<Der, Num> id)
{
	//We use common type with short to force char and unsigned char to be promoted and formatted as numbers.
	typedef typename std::common_type<short, Num>::type Number;
	return os << static_cast<Number>(id.getNum());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class EntityBase
{
public:
	int32_t num;

	struct hash
	{
		size_t operator()(const EntityBase & id) const
		{
			return std::hash<int>()(id.num);
		}
	};
};

template<typename T>
class EntityIdentifier : public T
{
	using EnumType = typename T::Type;

	static_assert(std::is_same_v<std::underlying_type_t<EnumType>, int32_t>, "Entity Identifier must use int32_t");
public:
	constexpr int32_t getNum() const
	{
		return T::num;
	}

	constexpr EnumType toEnum() const
	{
		return static_cast<EnumType>(T::num);
	}

	template <typename Handler> void serialize(Handler &h, const int version)
	{
		h & T::num;
	}

	constexpr EntityIdentifier(const EnumType & enumValue)
	{
		T::num = static_cast<int32_t>(enumValue);
	}

	constexpr EntityIdentifier(int32_t _num = -1)
	{
		T::num = _num;
	}

	constexpr void advance(int change)
	{
		T::num += change;
	}

	constexpr bool operator == (const EnumType & b) const { return T::num == static_cast<int32_t>(b); }
	constexpr bool operator <= (const EnumType & b) const { return T::num <= static_cast<int32_t>(b); }
	constexpr bool operator >= (const EnumType & b) const { return T::num >= static_cast<int32_t>(b); }
	constexpr bool operator != (const EnumType & b) const { return T::num != static_cast<int32_t>(b); }
	constexpr bool operator <  (const EnumType & b) const { return T::num <  static_cast<int32_t>(b); }
	constexpr bool operator >  (const EnumType & b) const { return T::num >  static_cast<int32_t>(b); }

	constexpr bool operator == (const EntityIdentifier & b) const { return T::num == b.num; }
	constexpr bool operator <= (const EntityIdentifier & b) const { return T::num <= b.num; }
	constexpr bool operator >= (const EntityIdentifier & b) const { return T::num >= b.num; }
	constexpr bool operator != (const EntityIdentifier & b) const { return T::num != b.num; }
	constexpr bool operator <  (const EntityIdentifier & b) const { return T::num <  b.num; }
	constexpr bool operator >  (const EntityIdentifier & b) const { return T::num >  b.num; }

	constexpr EntityIdentifier & operator++()
	{
		++T::num;
		return *this;
	}

	constexpr EntityIdentifier operator++(int)
	{
		EntityIdentifier ret(*this);
		++T::num;
		return ret;
	}

	constexpr operator int32_t () const
	{
		return T::num;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ArtifactInstanceID : public BaseForID<ArtifactInstanceID, si32>
{
	INSTID_LIKE_CLASS_COMMON(ArtifactInstanceID, si32)

	friend class CGameInfoCallback;
	friend class CNonConstInfoCallback;
};

class QueryID : public BaseForID<QueryID, si32>
{
	INSTID_LIKE_CLASS_COMMON(QueryID, si32)

	QueryID & operator++()
	{
		++num;
		return *this;
	}
};

class ObjectInstanceID : public BaseForID<ObjectInstanceID, si32>
{
	INSTID_LIKE_CLASS_COMMON(ObjectInstanceID, si32)

	DLL_LINKAGE static const ObjectInstanceID NONE;

	friend class CGameInfoCallback;
	friend class CNonConstInfoCallback;
};

class HeroClassID : public BaseForID<HeroClassID, si32>
{
	INSTID_LIKE_CLASS_COMMON(HeroClassID, si32)
};

class HeroTypeID : public BaseForID<HeroTypeID, si32>
{
	INSTID_LIKE_CLASS_COMMON(HeroTypeID, si32)

	///json serialization helpers
	static si32 decode(const std::string & identifier);
	static std::string encode(const si32 index);

	DLL_LINKAGE static const HeroTypeID NONE;
};

class SlotID : public BaseForID<SlotID, si32>
{
	INSTID_LIKE_CLASS_COMMON(SlotID, si32)

	friend class CGameInfoCallback;
	friend class CNonConstInfoCallback;

	DLL_LINKAGE static const SlotID COMMANDER_SLOT_PLACEHOLDER;
	DLL_LINKAGE static const SlotID SUMMONED_SLOT_PLACEHOLDER; ///<for all summoned creatures, only during battle
	DLL_LINKAGE static const SlotID WAR_MACHINES_SLOT; ///<for all war machines during battle
	DLL_LINKAGE static const SlotID ARROW_TOWERS_SLOT; ///<for all arrow towers during battle

	bool validSlot() const
	{
		return getNum() >= 0  &&  getNum() < GameConstants::ARMY_SIZE;
	}
};

class PlayerColor : public BaseForID<PlayerColor, ui8>
{
	INSTID_LIKE_CLASS_COMMON(PlayerColor, ui8)

	enum EPlayerColor
	{
		PLAYER_LIMIT_I = 8,
	};

	using Mask = uint8_t;

	DLL_LINKAGE static const PlayerColor SPECTATOR; //252
	DLL_LINKAGE static const PlayerColor CANNOT_DETERMINE; //253
	DLL_LINKAGE static const PlayerColor UNFLAGGABLE; //254 - neutral objects (pandora, banks)
	DLL_LINKAGE static const PlayerColor NEUTRAL; //255
	DLL_LINKAGE static const PlayerColor PLAYER_LIMIT; //player limit per map

	DLL_LINKAGE bool isValidPlayer() const; //valid means < PLAYER_LIMIT (especially non-neutral)
	DLL_LINKAGE bool isSpectator() const;

	DLL_LINKAGE std::string getStr(bool L10n = false) const;
	DLL_LINKAGE std::string getStrCap(bool L10n = false) const;

	friend class CGameInfoCallback;
	friend class CNonConstInfoCallback;
};

class TeamID : public BaseForID<TeamID, ui8>
{
	INSTID_LIKE_CLASS_COMMON(TeamID, ui8)

	DLL_LINKAGE static const TeamID NO_TEAM;

	friend class CGameInfoCallback;
	friend class CNonConstInfoCallback;
};

class TeleportChannelID : public BaseForID<TeleportChannelID, si32>
{
	INSTID_LIKE_CLASS_COMMON(TeleportChannelID, si32)

	friend class CGameInfoCallback;
	friend class CNonConstInfoCallback;
};

// Enum declarations
enum class PrimarySkill : int8_t
{
	NONE = -1,
	ATTACK,
	DEFENSE,
	SPELL_POWER,
	KNOWLEDGE,
	EXPERIENCE = 4 //for some reason changePrimSkill uses it
};

class SecondarySkillBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		WRONG = -2,
		DEFAULT = -1,
		PATHFINDING = 0, ARCHERY, LOGISTICS, SCOUTING, DIPLOMACY, NAVIGATION, LEADERSHIP, WISDOM, MYSTICISM,
		LUCK, BALLISTICS, EAGLE_EYE, NECROMANCY, ESTATES, FIRE_MAGIC, AIR_MAGIC, WATER_MAGIC, EARTH_MAGIC,
		SCHOLAR, TACTICS, ARTILLERY, LEARNING, OFFENCE, ARMORER, INTELLIGENCE, SORCERY, RESISTANCE,
		FIRST_AID, SKILL_SIZE
	};
	static_assert(GameConstants::SKILL_QUANTITY == SKILL_SIZE, "Incorrect number of skills");
};

class SecondarySkill : public EntityIdentifier<SecondarySkillBase>
{
public:
	using EntityIdentifier<SecondarySkillBase>::EntityIdentifier;
};

enum class EAlignment : uint8_t { GOOD, EVIL, NEUTRAL };

class FactionIDBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		NONE = -2,
		DEFAULT = -1,
		RANDOM = -1,
		ANY = -1,
		CASTLE,
		RAMPART,
		TOWER,
		INFERNO,
		NECROPOLIS,
		DUNGEON,
		STRONGHOLD,
		FORTRESS,
		CONFLUX,
		NEUTRAL
	};

	static si32 decode(const std::string& identifier);
	static std::string encode(const si32 index);
	static std::string entityType();
};

class FactionID : public EntityIdentifier<FactionIDBase>
{
public:
	using EntityIdentifier<FactionIDBase>::EntityIdentifier;
};

using ETownType = FactionID;

class BuildingID
{
public:
	//Quite useful as long as most of building mechanics hardcoded
	// NOTE: all building with completely configurable mechanics will be removed from list
	enum EBuildingID
	{
		DEFAULT = -50,
		HORDE_PLACEHOLDER7 = -36,
		HORDE_PLACEHOLDER6 = -35,
		HORDE_PLACEHOLDER5 = -34,
		HORDE_PLACEHOLDER4 = -33,
		HORDE_PLACEHOLDER3 = -32,
		HORDE_PLACEHOLDER2 = -31,
		HORDE_PLACEHOLDER1 = -30,
		NONE = -1,
		FIRST_REGULAR_ID = 0,
		MAGES_GUILD_1 = 0,  MAGES_GUILD_2, MAGES_GUILD_3,     MAGES_GUILD_4,   MAGES_GUILD_5,
		TAVERN,         SHIPYARD,      FORT,              CITADEL,         CASTLE,
		VILLAGE_HALL,   TOWN_HALL,     CITY_HALL,         CAPITOL,         MARKETPLACE,
		RESOURCE_SILO,  BLACKSMITH,    SPECIAL_1,         HORDE_1,         HORDE_1_UPGR,
		SHIP,           SPECIAL_2,     SPECIAL_3,         SPECIAL_4,       HORDE_2,
		HORDE_2_UPGR,   GRAIL,         EXTRA_TOWN_HALL,   EXTRA_CITY_HALL, EXTRA_CAPITOL,
		DWELL_FIRST=30, DWELL_LVL_2, DWELL_LVL_3, DWELL_LVL_4, DWELL_LVL_5, DWELL_LVL_6, DWELL_LAST=36,
		DWELL_UP_FIRST=37,  DWELL_LVL_2_UP, DWELL_LVL_3_UP, DWELL_LVL_4_UP, DWELL_LVL_5_UP,
		DWELL_LVL_6_UP, DWELL_UP_LAST=43,

		DWELL_LVL_1 = DWELL_FIRST,
		DWELL_LVL_7 = DWELL_LAST,
		DWELL_LVL_1_UP = DWELL_UP_FIRST,
		DWELL_LVL_7_UP = DWELL_UP_LAST,

		//Special buildings for towns.
		LIGHTHOUSE  = SPECIAL_1,
		STABLES     = SPECIAL_2, //Castle
		BROTHERHOOD = SPECIAL_3,

		MYSTIC_POND         = SPECIAL_1,
		FOUNTAIN_OF_FORTUNE = SPECIAL_2, //Rampart
		TREASURY            = SPECIAL_3,

		ARTIFACT_MERCHANT = SPECIAL_1,
		LOOKOUT_TOWER     = SPECIAL_2, //Tower
		LIBRARY           = SPECIAL_3,
		WALL_OF_KNOWLEDGE = SPECIAL_4,

		STORMCLOUDS   = SPECIAL_2,
		CASTLE_GATE   = SPECIAL_3, //Inferno
		ORDER_OF_FIRE = SPECIAL_4,

		COVER_OF_DARKNESS    = SPECIAL_1,
		NECROMANCY_AMPLIFIER = SPECIAL_2, //Necropolis
		SKELETON_TRANSFORMER = SPECIAL_3,

		//ARTIFACT_MERCHANT - same ID as in tower
		MANA_VORTEX      = SPECIAL_2,
		PORTAL_OF_SUMMON = SPECIAL_3, //Dungeon
		BATTLE_ACADEMY   = SPECIAL_4,

		ESCAPE_TUNNEL     = SPECIAL_1,
		FREELANCERS_GUILD = SPECIAL_2, //Stronghold
		BALLISTA_YARD     = SPECIAL_3,
		HALL_OF_VALHALLA  = SPECIAL_4,

		CAGE_OF_WARLORDS = SPECIAL_1,
		GLYPHS_OF_FEAR   = SPECIAL_2, // Fortress
		BLOOD_OBELISK    = SPECIAL_3,

		//ARTIFACT_MERCHANT - same ID as in tower
		MAGIC_UNIVERSITY = SPECIAL_2, // Conflux
	};

	BuildingID(EBuildingID _num = NONE) : num(_num)
	{}

	STRONG_INLINE
	bool IsSpecialOrGrail() const
	{
		return num == SPECIAL_1 || num == SPECIAL_2 || num == SPECIAL_3 || num == SPECIAL_4 || num == GRAIL;
	}

	ID_LIKE_CLASS_COMMON(BuildingID, EBuildingID)

	EBuildingID num;
};

ID_LIKE_OPERATORS(BuildingID, BuildingID::EBuildingID)

namespace BuildingSubID
{
	enum EBuildingSubID
	{
		DEFAULT = -50,
		NONE = -1,
		STABLES,
		BROTHERHOOD_OF_SWORD,
		CASTLE_GATE,
		CREATURE_TRANSFORMER,
		MYSTIC_POND,
		FOUNTAIN_OF_FORTUNE,
		ARTIFACT_MERCHANT,
		LOOKOUT_TOWER,
		LIBRARY,
		MANA_VORTEX,
		PORTAL_OF_SUMMONING,
		ESCAPE_TUNNEL,
		FREELANCERS_GUILD,
		BALLISTA_YARD,
		ATTACK_VISITING_BONUS,
		MAGIC_UNIVERSITY,
		SPELL_POWER_GARRISON_BONUS,
		ATTACK_GARRISON_BONUS,
		DEFENSE_GARRISON_BONUS,
		DEFENSE_VISITING_BONUS,
		SPELL_POWER_VISITING_BONUS,
		KNOWLEDGE_VISITING_BONUS,
		EXPERIENCE_VISITING_BONUS,
		LIGHTHOUSE,
		TREASURY,
		CUSTOM_VISITING_BONUS,
		CUSTOM_VISITING_REWARD
	};
}

enum class EMarketMode : int8_t
{
	RESOURCE_RESOURCE, RESOURCE_PLAYER, CREATURE_RESOURCE, RESOURCE_ARTIFACT,
	ARTIFACT_RESOURCE, ARTIFACT_EXP, CREATURE_EXP, CREATURE_UNDEAD, RESOURCE_SKILL,
	MARTKET_AFTER_LAST_PLACEHOLDER
};

namespace MappedKeys
{

	static const std::map<std::string, BuildingID> BUILDING_NAMES_TO_TYPES =
	{
		{ "special1", BuildingID::SPECIAL_1 },
		{ "special2", BuildingID::SPECIAL_2 },
		{ "special3", BuildingID::SPECIAL_3 },
		{ "special4", BuildingID::SPECIAL_4 },
		{ "grail", BuildingID::GRAIL },
		{ "mageGuild1", BuildingID::MAGES_GUILD_1 },
		{ "mageGuild2", BuildingID::MAGES_GUILD_2 },
		{ "mageGuild3", BuildingID::MAGES_GUILD_3 },
		{ "mageGuild4", BuildingID::MAGES_GUILD_4 },
		{ "mageGuild5", BuildingID::MAGES_GUILD_5 },
		{ "tavern", BuildingID::TAVERN },
		{ "shipyard", BuildingID::SHIPYARD },
		{ "fort", BuildingID::FORT },
		{ "citadel", BuildingID::CITADEL },
		{ "castle", BuildingID::CASTLE },
		{ "villageHall", BuildingID::VILLAGE_HALL },
		{ "townHall", BuildingID::TOWN_HALL },
		{ "cityHall", BuildingID::CITY_HALL },
		{ "capitol", BuildingID::CAPITOL },
		{ "marketplace", BuildingID::MARKETPLACE },
		{ "resourceSilo", BuildingID::RESOURCE_SILO },
		{ "blacksmith", BuildingID::BLACKSMITH },
		{ "horde1", BuildingID::HORDE_1 },
		{ "horde1Upgr", BuildingID::HORDE_1_UPGR },
		{ "horde2", BuildingID::HORDE_2 },
		{ "horde2Upgr", BuildingID::HORDE_2_UPGR },
		{ "ship", BuildingID::SHIP },
		{ "dwellingLvl1", BuildingID::DWELL_LVL_1 },
		{ "dwellingLvl2", BuildingID::DWELL_LVL_2 },
		{ "dwellingLvl3", BuildingID::DWELL_LVL_3 },
		{ "dwellingLvl4", BuildingID::DWELL_LVL_4 },
		{ "dwellingLvl5", BuildingID::DWELL_LVL_5 },
		{ "dwellingLvl6", BuildingID::DWELL_LVL_6 },
		{ "dwellingLvl7", BuildingID::DWELL_LVL_7 },
		{ "dwellingUpLvl1", BuildingID::DWELL_LVL_1_UP },
		{ "dwellingUpLvl2", BuildingID::DWELL_LVL_2_UP },
		{ "dwellingUpLvl3", BuildingID::DWELL_LVL_3_UP },
		{ "dwellingUpLvl4", BuildingID::DWELL_LVL_4_UP },
		{ "dwellingUpLvl5", BuildingID::DWELL_LVL_5_UP },
		{ "dwellingUpLvl6", BuildingID::DWELL_LVL_6_UP },
		{ "dwellingUpLvl7", BuildingID::DWELL_LVL_7_UP },
	};

	static const std::map<std::string, BuildingSubID::EBuildingSubID> SPECIAL_BUILDINGS =
	{
		{ "mysticPond", BuildingSubID::MYSTIC_POND },
		{ "artifactMerchant", BuildingSubID::ARTIFACT_MERCHANT },
		{ "freelancersGuild", BuildingSubID::FREELANCERS_GUILD },
		{ "magicUniversity", BuildingSubID::MAGIC_UNIVERSITY },
		{ "castleGate", BuildingSubID::CASTLE_GATE },
		{ "creatureTransformer", BuildingSubID::CREATURE_TRANSFORMER },//only skeleton transformer yet
		{ "portalOfSummoning", BuildingSubID::PORTAL_OF_SUMMONING },
		{ "ballistaYard", BuildingSubID::BALLISTA_YARD },
		{ "stables", BuildingSubID::STABLES },
		{ "manaVortex", BuildingSubID::MANA_VORTEX },
		{ "lookoutTower", BuildingSubID::LOOKOUT_TOWER },
		{ "library", BuildingSubID::LIBRARY },
		{ "brotherhoodOfSword", BuildingSubID::BROTHERHOOD_OF_SWORD },//morale garrison bonus
		{ "fountainOfFortune", BuildingSubID::FOUNTAIN_OF_FORTUNE },//luck garrison bonus
		{ "spellPowerGarrisonBonus", BuildingSubID::SPELL_POWER_GARRISON_BONUS },//such as 'stormclouds', but this name is not ok for good towns
		{ "attackGarrisonBonus", BuildingSubID::ATTACK_GARRISON_BONUS },
		{ "defenseGarrisonBonus", BuildingSubID::DEFENSE_GARRISON_BONUS },
		{ "escapeTunnel", BuildingSubID::ESCAPE_TUNNEL },
		{ "attackVisitingBonus", BuildingSubID::ATTACK_VISITING_BONUS },
		{ "defenceVisitingBonus", BuildingSubID::DEFENSE_VISITING_BONUS },
		{ "spellPowerVisitingBonus", BuildingSubID::SPELL_POWER_VISITING_BONUS },
		{ "knowledgeVisitingBonus", BuildingSubID::KNOWLEDGE_VISITING_BONUS },
		{ "experienceVisitingBonus", BuildingSubID::EXPERIENCE_VISITING_BONUS },
		{ "lighthouse", BuildingSubID::LIGHTHOUSE },
		{ "treasury", BuildingSubID::TREASURY }
	};

	static const std::map<std::string, EMarketMode> MARKET_NAMES_TO_TYPES =
	{
		{ "resource-resource", EMarketMode::RESOURCE_RESOURCE },
		{ "resource-player", EMarketMode::RESOURCE_PLAYER },
		{ "creature-resource", EMarketMode::CREATURE_RESOURCE },
		{ "resource-artifact", EMarketMode::RESOURCE_ARTIFACT },
		{ "artifact-resource", EMarketMode::ARTIFACT_RESOURCE },
		{ "artifact-experience", EMarketMode::ARTIFACT_EXP },
		{ "creature-experience", EMarketMode::CREATURE_EXP },
		{ "creature-undead", EMarketMode::CREATURE_UNDEAD },
		{ "resource-skill", EMarketMode::RESOURCE_SKILL },
	};
}

enum class EAiTactic : int8_t
{
	NONE = -1,
	RANDOM,
	WARRIOR,
	BUILDER,
	EXPLORER
};

enum class EBuildingState : int8_t
{
	HAVE_CAPITAL, NO_WATER, FORBIDDEN, ADD_MAGES_GUILD, ALREADY_PRESENT, CANT_BUILD_TODAY,
	NO_RESOURCES, ALLOWED, PREREQUIRES, MISSING_BASE, BUILDING_ERROR, TOWN_NOT_OWNED
};

enum class ESpellCastProblem : int8_t
{
	OK, NO_HERO_TO_CAST_SPELL, CASTS_PER_TURN_LIMIT, NO_SPELLBOOK,
	HERO_DOESNT_KNOW_SPELL, NOT_ENOUGH_MANA, ADVMAP_SPELL_INSTEAD_OF_BATTLE_SPELL,
	SPELL_LEVEL_LIMIT_EXCEEDED, NO_SPELLS_TO_DISPEL,
	NO_APPROPRIATE_TARGET, STACK_IMMUNE_TO_SPELL, WRONG_SPELL_TARGET, ONGOING_TACTIC_PHASE,
	MAGIC_IS_BLOCKED, //For Orb of Inhibition and similar - no casting at all
	INVALID
};

namespace ECommander
{
	enum SecondarySkills {ATTACK, DEFENSE, HEALTH, DAMAGE, SPEED, SPELL_POWER, CASTS, RESISTANCE};
	const int MAX_SKILL_LEVEL = 5;
}

enum class EWallPart : int8_t
{
	INDESTRUCTIBLE_PART_OF_GATE = -3, INDESTRUCTIBLE_PART = -2, INVALID = -1,
	KEEP = 0, BOTTOM_TOWER, BOTTOM_WALL, BELOW_GATE, OVER_GATE, UPPER_WALL, UPPER_TOWER, GATE,
	PARTS_COUNT /* This constant SHOULD always stay as the last item in the enum. */
};

enum class EWallState : int8_t
{
	NONE = -1, //no wall
	DESTROYED,
	DAMAGED,
	INTACT,
	REINFORCED, // walls in towns with castle
};

enum class EGateState : int8_t
{
	NONE,
	CLOSED,
	BLOCKED, // gate is blocked in closed state, e.g. by creature
	OPENED,
	DESTROYED
};


enum class ETileType : int8_t
{
	FREE,
	POSSIBLE,
	BLOCKED,
	USED
};

enum class ETeleportChannelType : int8_t
{
	IMPASSABLE,
	BIDIRECTIONAL,
	UNIDIRECTIONAL,
	MIXED
};

class ObjBase : public EntityBase
{
public:
	enum Type
	{
		NO_OBJ = -1,
		ALTAR_OF_SACRIFICE [[deprecated]] = 2,
		ANCHOR_POINT = 3,
		ARENA = 4,
		ARTIFACT = 5,
		PANDORAS_BOX = 6,
		BLACK_MARKET [[deprecated]] = 7,
		BOAT = 8,
		BORDERGUARD = 9,
		KEYMASTER = 10,
		BUOY = 11,
		CAMPFIRE = 12,
		CARTOGRAPHER = 13,
		SWAN_POND = 14,
		COVER_OF_DARKNESS = 15,
		CREATURE_BANK = 16,
		CREATURE_GENERATOR1 = 17,
		CREATURE_GENERATOR2 = 18,
		CREATURE_GENERATOR3 = 19,
		CREATURE_GENERATOR4 = 20,
		CURSED_GROUND1 = 21,
		CORPSE = 22,
		MARLETTO_TOWER = 23,
		DERELICT_SHIP = 24,
		DRAGON_UTOPIA = 25,
		EVENT = 26,
		EYE_OF_MAGI = 27,
		FAERIE_RING = 28,
		FLOTSAM = 29,
		FOUNTAIN_OF_FORTUNE = 30,
		FOUNTAIN_OF_YOUTH = 31,
		GARDEN_OF_REVELATION = 32,
		GARRISON = 33,
		HERO = 34,
		HILL_FORT = 35,
		GRAIL = 36,
		HUT_OF_MAGI = 37,
		IDOL_OF_FORTUNE = 38,
		LEAN_TO = 39,
		LIBRARY_OF_ENLIGHTENMENT = 41,
		LIGHTHOUSE = 42,
		MONOLITH_ONE_WAY_ENTRANCE = 43,
		MONOLITH_ONE_WAY_EXIT = 44,
		MONOLITH_TWO_WAY = 45,
		MAGIC_PLAINS1 = 46,
		SCHOOL_OF_MAGIC = 47,
		MAGIC_SPRING = 48,
		MAGIC_WELL = 49,
		MARKET_OF_TIME = 50,
		MERCENARY_CAMP = 51,
		MERMAID = 52,
		MINE = 53,
		MONSTER = 54,
		MYSTICAL_GARDEN = 55,
		OASIS = 56,
		OBELISK = 57,
		REDWOOD_OBSERVATORY = 58,
		OCEAN_BOTTLE = 59,
		PILLAR_OF_FIRE = 60,
		STAR_AXIS = 61,
		PRISON = 62,
		PYRAMID = 63,//subtype 0
		WOG_OBJECT = 63,//subtype > 0
		RALLY_FLAG = 64,
		RANDOM_ART = 65,
		RANDOM_TREASURE_ART = 66,
		RANDOM_MINOR_ART = 67,
		RANDOM_MAJOR_ART = 68,
		RANDOM_RELIC_ART = 69,
		RANDOM_HERO = 70,
		RANDOM_MONSTER = 71,
		RANDOM_MONSTER_L1 = 72,
		RANDOM_MONSTER_L2 = 73,
		RANDOM_MONSTER_L3 = 74,
		RANDOM_MONSTER_L4 = 75,
		RANDOM_RESOURCE = 76,
		RANDOM_TOWN = 77,
		REFUGEE_CAMP = 78,
		RESOURCE = 79,
		SANCTUARY = 80,
		SCHOLAR = 81,
		SEA_CHEST = 82,
		SEER_HUT = 83,
		CRYPT = 84,
		SHIPWRECK = 85,
		SHIPWRECK_SURVIVOR = 86,
		SHIPYARD = 87,
		SHRINE_OF_MAGIC_INCANTATION = 88,
		SHRINE_OF_MAGIC_GESTURE = 89,
		SHRINE_OF_MAGIC_THOUGHT = 90,
		SIGN = 91,
		SIRENS = 92,
		SPELL_SCROLL = 93,
		STABLES = 94,
		TAVERN = 95,
		TEMPLE = 96,
		DEN_OF_THIEVES = 97,
		TOWN = 98,
		TRADING_POST [[deprecated]] = 99,
		LEARNING_STONE = 100,
		TREASURE_CHEST = 101,
		TREE_OF_KNOWLEDGE = 102,
		SUBTERRANEAN_GATE = 103,
		UNIVERSITY [[deprecated]] = 104,
		WAGON = 105,
		WAR_MACHINE_FACTORY = 106,
		SCHOOL_OF_WAR = 107,
		WARRIORS_TOMB = 108,
		WATER_WHEEL = 109,
		WATERING_HOLE = 110,
		WHIRLPOOL = 111,
		WINDMILL = 112,
		WITCH_HUT = 113,
		HOLE = 124,
		RANDOM_MONSTER_L5 = 162,
		RANDOM_MONSTER_L6 = 163,
		RANDOM_MONSTER_L7 = 164,
		BORDER_GATE = 212,
		FREELANCERS_GUILD [[deprecated]] = 213,
		HERO_PLACEHOLDER = 214,
		QUEST_GUARD = 215,
		RANDOM_DWELLING = 216,
		RANDOM_DWELLING_LVL = 217, //subtype = creature level
		RANDOM_DWELLING_FACTION = 218, //subtype = faction
		GARRISON2 = 219,
		ABANDONED_MINE = 220,
		TRADING_POST_SNOW [[deprecated]] = 221,
		CLOVER_FIELD = 222,
		CURSED_GROUND2 = 223,
		EVIL_FOG = 224,
		FAVORABLE_WINDS = 225,
		FIERY_FIELDS = 226,
		HOLY_GROUNDS = 227,
		LUCID_POOLS = 228,
		MAGIC_CLOUDS = 229,
		MAGIC_PLAINS2 = 230,
		ROCKLANDS = 231,
	};
};

class Obj : public EntityIdentifier<ObjBase>
{
public:
	using EntityIdentifier<ObjBase>::EntityIdentifier;
};

class RoadIdBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		NO_ROAD = 0,
		FIRST_REGULAR_ROAD = 1,
		DIRT_ROAD = 1,
		GRAVEL_ROAD = 2,
		COBBLESTONE_ROAD = 3,
		ORIGINAL_ROAD_COUNT //+1
	};
};

class RoadId : public EntityIdentifier<RoadIdBase>
{
public:
	using EntityIdentifier<RoadIdBase>::EntityIdentifier;
};

class RiverIdBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		NO_RIVER = 0,
		FIRST_REGULAR_RIVER = 1,
		WATER_RIVER = 1,
		ICY_RIVER = 2,
		MUD_RIVER = 3,
		LAVA_RIVER = 4,
		ORIGINAL_RIVER_COUNT //+1
	};
};

class RiverId : public EntityIdentifier<RiverIdBase>
{
public:
	using EntityIdentifier<RiverIdBase>::EntityIdentifier;
};

using River = RiverId;
using Road = RoadId;

namespace SecSkillLevel
{
	enum SecSkillLevel
	{
		NONE,
		BASIC,
		ADVANCED,
		EXPERT,
		LEVELS_SIZE
	};
}

enum class Date : int8_t
{
	DAY = 0,
	DAY_OF_WEEK = 1,
	WEEK = 2,
	MONTH = 3,
	DAY_OF_MONTH
};

enum class EActionType : int8_t
{
	NO_ACTION,

	END_TACTIC_PHASE,
	RETREAT,
	SURRENDER,

	HERO_SPELL,

	WALK,
	WAIT,
	DEFEND,
	WALK_AND_ATTACK,
	SHOOT,
	CATAPULT,
	MONSTER_SPELL,
	BAD_MORALE,
	STACK_HEAL,
};

DLL_LINKAGE std::ostream & operator<<(std::ostream & os, const EActionType actionType);

enum class EDiggingStatus : int8_t
{
	UNKNOWN = -1,
	CAN_DIG = 0,
	LACK_OF_MOVEMENT,
	WRONG_TERRAIN,
	TILE_OCCUPIED,
	BACKPACK_IS_FULL
};

class DLL_LINKAGE EPathfindingLayer
{
public:
	enum EEPathfindingLayer : ui8
	{
		LAND = 0, SAIL = 1, WATER, AIR, NUM_LAYERS, WRONG, AUTO
	};

	EPathfindingLayer(EEPathfindingLayer _num = WRONG) : num(_num)
	{}

	ID_LIKE_CLASS_COMMON(EPathfindingLayer, EEPathfindingLayer)

	EEPathfindingLayer num;
};

DLL_LINKAGE std::ostream & operator<<(std::ostream & os, const EPathfindingLayer & pathfindingLayer);

ID_LIKE_OPERATORS(EPathfindingLayer, EPathfindingLayer::EEPathfindingLayer)

enum class EPlayerStatus
{
	WRONG = -1,
	INGAME,
	LOSER,
	WINNER
};

enum class PlayerRelations
{
	ENEMIES,
	ALLIES,
	SAME_PLAYER
};

class ArtifactPosition
{
public:
	enum EArtifactPosition
	{
		TRANSITION_POS = -3,
		FIRST_AVAILABLE = -2,
		PRE_FIRST = -1, //sometimes used as error, sometimes as first free in backpack
		HEAD, SHOULDERS, NECK, RIGHT_HAND, LEFT_HAND, TORSO, //5
		RIGHT_RING, LEFT_RING, FEET, //8
		MISC1, MISC2, MISC3, MISC4, //12
		MACH1, MACH2, MACH3, MACH4, //16
		SPELLBOOK, MISC5, //18
		AFTER_LAST,
		//cres
		CREATURE_SLOT = 0,
		COMMANDER1 = 0, COMMANDER2, COMMANDER3, COMMANDER4, COMMANDER5, COMMANDER6, COMMANDER_AFTER_LAST
	};

	static_assert (AFTER_LAST == 19, "incorrect number of artifact slots");

	ArtifactPosition(EArtifactPosition _num = PRE_FIRST) : num(_num)
	{}

	ArtifactPosition(std::string slotName);

	ID_LIKE_CLASS_COMMON(ArtifactPosition, EArtifactPosition)

	EArtifactPosition num;

        STRONG_INLINE EArtifactPosition operator+(const int arg)
	{
		return EArtifactPosition(static_cast<int>(num) + arg);
	}
	STRONG_INLINE EArtifactPosition operator+(const EArtifactPosition & arg)
	{
		return EArtifactPosition(static_cast<int>(num) + static_cast<int>(arg));
	}
};

ID_LIKE_OPERATORS(ArtifactPosition, ArtifactPosition::EArtifactPosition)

namespace GameConstants
{
	const auto BACKPACK_START = ArtifactPosition::AFTER_LAST;
}

class ArtifactIDBase : public EntityBase
{
public:
	enum Type
	{
		NONE = -1,
		SPELLBOOK = 0,
		SPELL_SCROLL = 1,
		GRAIL = 2,
		CATAPULT = 3,
		BALLISTA = 4,
		AMMO_CART = 5,
		FIRST_AID_TENT = 6,
		VIAL_OF_DRAGON_BLOOD = 127,
		ARMAGEDDONS_BLADE = 128,
		TITANS_THUNDER = 135,
		ART_SELECTION = 144,
		ART_LOCK = 145, // FIXME: We must get rid of this one since it's conflict with artifact from mods. See issue 2455
	};

	DLL_LINKAGE const CArtifact * toArtifact() const;
	DLL_LINKAGE const Artifact * toArtifact(const ArtifactService * service) const;

	///json serialization helpers
	static si32 decode(const std::string & identifier);
	static std::string encode(const si32 index);
};

class ArtifactID : public EntityIdentifier<ArtifactIDBase>
{
public:
	using EntityIdentifier<ArtifactIDBase>::EntityIdentifier;
};

class CreatureIDBase : public EntityBase
{
public:
	enum Type
	{
		NONE = -1,
		ARCHER = 2, // for debug / fallback
		IMP = 42, // for Deity of Fire
		SKELETON = 56, // for Skeleton Transformer
		BONE_DRAGON = 68, // for Skeleton Transformer
		TROGLODYTES = 70, // for Abandoned Mine
		MEDUSA = 76, // for Siege UI workaround
		HYDRA = 110, // for Skeleton Transformer
		CHAOS_HYDRA = 111, // for Skeleton Transformer
		AIR_ELEMENTAL = 112, // for tests
		FIRE_ELEMENTAL = 114, // for tests
		PSYCHIC_ELEMENTAL = 120, // for hardcoded ability
		MAGIC_ELEMENTAL = 121, // for hardcoded ability
		CATAPULT = 145,
		BALLISTA = 146,
		FIRST_AID_TENT = 147,
		AMMO_CART = 148,
		ARROW_TOWERS = 149
	};

	DLL_LINKAGE const CCreature * toCreature() const;
	DLL_LINKAGE const Creature * toCreature(const CreatureService * creatures) const;

	///json serialization helpers
	static si32 decode(const std::string & identifier);
	static std::string encode(const si32 index);
};

class CreatureID : public EntityIdentifier<CreatureIDBase>
{
public:
	using EntityIdentifier<CreatureIDBase>::EntityIdentifier;
};

class SpellIDBase : public EntityBase
{
public:
	enum Type
	{
		SPELLBOOK_PRESET = -3,
		PRESET = -2,
		NONE = -1,
		SUMMON_BOAT=0, SCUTTLE_BOAT=1, VISIONS=2, VIEW_EARTH=3, DISGUISE=4, VIEW_AIR=5,
		FLY=6, WATER_WALK=7, DIMENSION_DOOR=8, TOWN_PORTAL=9,

		QUICKSAND=10, LAND_MINE=11, FORCE_FIELD=12, FIRE_WALL=13, EARTHQUAKE=14,
		MAGIC_ARROW=15, ICE_BOLT=16, LIGHTNING_BOLT=17, IMPLOSION=18,
		CHAIN_LIGHTNING=19, FROST_RING=20, FIREBALL=21, INFERNO=22,
		METEOR_SHOWER=23, DEATH_RIPPLE=24, DESTROY_UNDEAD=25, ARMAGEDDON=26,
		SHIELD=27, AIR_SHIELD=28, FIRE_SHIELD=29, PROTECTION_FROM_AIR=30,
		PROTECTION_FROM_FIRE=31, PROTECTION_FROM_WATER=32,
		PROTECTION_FROM_EARTH=33, ANTI_MAGIC=34, DISPEL=35, MAGIC_MIRROR=36,
		CURE=37, RESURRECTION=38, ANIMATE_DEAD=39, SACRIFICE=40, BLESS=41,
		CURSE=42, BLOODLUST=43, PRECISION=44, WEAKNESS=45, STONE_SKIN=46,
		DISRUPTING_RAY=47, PRAYER=48, MIRTH=49, SORROW=50, FORTUNE=51,
		MISFORTUNE=52, HASTE=53, SLOW=54, SLAYER=55, FRENZY=56,
		TITANS_LIGHTNING_BOLT=57, COUNTERSTRIKE=58, BERSERK=59, HYPNOTIZE=60,
		FORGETFULNESS=61, BLIND=62, TELEPORT=63, REMOVE_OBSTACLE=64, CLONE=65,
		SUMMON_FIRE_ELEMENTAL=66, SUMMON_EARTH_ELEMENTAL=67, SUMMON_WATER_ELEMENTAL=68, SUMMON_AIR_ELEMENTAL=69,

		STONE_GAZE=70, POISON=71, BIND=72, DISEASE=73, PARALYZE=74, AGE=75, DEATH_CLOUD=76, THUNDERBOLT=77,
		DISPEL_HELPFUL_SPELLS=78, DEATH_STARE=79, ACID_BREATH_DEFENSE=80, ACID_BREATH_DAMAGE=81,

		FIRST_NON_SPELL = 70, AFTER_LAST = 82
	};

	DLL_LINKAGE const CSpell * toSpell() const; //deprecated
	DLL_LINKAGE const spells::Spell * toSpell(const spells::Service * service) const;

	///json serialization helpers
	static si32 decode(const std::string & identifier);
	static std::string encode(const si32 index);
};

class SpellID : public EntityIdentifier<SpellIDBase>
{
public:
	using EntityIdentifier<SpellIDBase>::EntityIdentifier;
};

class BattleFieldInfo;
class BattleField : public BaseForID<BattleField, si32>
{
	INSTID_LIKE_CLASS_COMMON(BattleField, si32)

	DLL_LINKAGE static const BattleField NONE;

	DLL_LINKAGE friend bool operator==(const BattleField & l, const BattleField & r);
	DLL_LINKAGE friend bool operator!=(const BattleField & l, const BattleField & r);
	DLL_LINKAGE friend bool operator<(const BattleField & l, const BattleField & r);

	DLL_LINKAGE const BattleFieldInfo * getInfo() const;
};

class BoatIdBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		NONE = -1,
		NECROPOLIS = 0,
		CASTLE,
		FORTRESS
	};
};

class BoatId : public EntityIdentifier<BoatIdBase>
{
public:
	using EntityIdentifier<BoatIdBase>::EntityIdentifier;
};

class TerrainIdBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		NATIVE_TERRAIN = -4,
		ANY_TERRAIN = -3,
		NONE = -1,
		FIRST_REGULAR_TERRAIN = 0,
		DIRT = 0,
		SAND,
		GRASS,
		SNOW,
		SWAMP,
		ROUGH,
		SUBTERRANEAN,
		LAVA,
		WATER,
		ROCK,
		ORIGINAL_REGULAR_TERRAIN_COUNT = ROCK
	};

	static si32 decode(const std::string & identifier);
	static std::string encode(const si32 index);
	static std::string entityType();
};

class TerrainId : public EntityIdentifier<TerrainIdBase>
{
public:
	using EntityIdentifier<TerrainIdBase>::EntityIdentifier;
};

using ETerrainId = TerrainId;

class ObstacleInfo;
class Obstacle : public BaseForID<Obstacle, si32>
{
	INSTID_LIKE_CLASS_COMMON(Obstacle, si32)

	DLL_LINKAGE const ObstacleInfo * getInfo() const;
};

class SpellSchoolBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		ANY 	= -1,
		AIR 	= 0,
		FIRE 	= 1,
		WATER 	= 2,
		EARTH 	= 3,
	};
};

class SpellSchool : public EntityIdentifier<SpellSchoolBase>
{
public:
	using EntityIdentifier<SpellSchoolBase>::EntityIdentifier;
};

using ESpellSchool = SpellSchool;

enum class EMetaclass: int8_t
{
	INVALID = 0,
	ARTIFACT,
	CREATURE,
	FACTION,
	EXPERIENCE,
	HERO,
	HEROCLASS,
	LUCK,
	MANA,
	MORALE,
	MOVEMENT,
	OBJECT,
	PRIMARY_SKILL,
	SECONDARY_SKILL,
	SPELL,
	RESOURCE
};

enum class EHealLevel: ui8
{
	HEAL,
	RESURRECT,
	OVERHEAL
};

enum class EHealPower : ui8
{
	ONE_BATTLE,
	PERMANENT
};

enum class EBattleResult : int8_t
{
	NORMAL = 0,
	ESCAPE = 1,
	SURRENDER = 2
};

class GameResIDBase : public EntityBase
{
public:
	enum Type : int32_t
	{
		WOOD = 0, MERCURY, ORE, SULFUR, CRYSTAL, GEMS, GOLD, MITHRIL,
		COUNT,

		WOOD_AND_ORE = 127,  // special case for town bonus resource
		INVALID = -1
	};
};

class GameResID : public EntityIdentifier<GameResIDBase>
{
public:
	using EntityIdentifier<GameResIDBase>::EntityIdentifier;
};

using EGameResID = GameResID;

// Typedef declarations
using TExpType = si64;
using TQuantity = si32;

using TRmgTemplateZoneId = int;

#undef ID_LIKE_CLASS_COMMON
#undef ID_LIKE_OPERATORS
#undef ID_LIKE_OPERATORS_INTERNAL
#undef INSTID_LIKE_CLASS_COMMON

VCMI_LIB_NAMESPACE_END

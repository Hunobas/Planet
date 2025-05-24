// PlanetConst.h
#pragma once



namespace PlanetConst
{
	
#pragma region Math

	static constexpr float PI_IN_DEGREES = 180.0f;
	
#pragma endregion
	
	
#pragma region Gameplay Statics

	static constexpr float PLAYTIME								= 1200.0f;
	static constexpr EAutoReceiveInput::Type SINGLEPLAYER_INDEX	= EAutoReceiveInput::Player0;

	static constexpr float ENEMY_FIRE_DELAY						= 2.0f;
	static constexpr float JUSTAIM_WINDOW_DURATION				= 0.3f;

	static constexpr int32 REWARD_COUNT							= 3;
	static constexpr int32 WEAPON_MAX_SLOT						= 7;
	static constexpr int32 ITEM_MAX_SLOT						= 6;

	static constexpr float WEAPON_APPEAR_RATE					= 0.6f;
	static constexpr float ITEM_APPEAR_RATE						= 0.35f;
	static constexpr float POWER_UP_APPEAR_RATE					= 0.05f;

	static constexpr float HP_BOOST								= 10.0f;
	static constexpr float DAMAGE_BOOST							= 2.0f;
	static constexpr float CRITICAL_BOOST						= 5.0f;
	static constexpr float CRITICAL_DAMAGE_BOOST				= 20.0f;
	static constexpr float HASTE_BOOST							= 5.0f;
	static constexpr float XP_GAIN_BOOST						= 10.0f;
	static constexpr float XP_SPEED_BOOST						= 20.0f;
	
#pragma endregion

	
#pragma region Default Tag
	
	inline const FName DEFAULT_SUN_TAG(TEXT("Sun"));
	inline const FName OBJECT_POOL_TAG(TEXT("Object Pool"));
	inline const FName DEFAULT_ENEMY_SPAWN_TAG(TEXT("Enemy Spawn Celestial"));
	inline const FName FIRE_MANAGER_TAG(TEXT("Fire Manager"));
	inline const FName HP_TAG(TEXT("HP"));
	inline const FName HP_WIDGET_TAG(TEXT("HP Widget"));
	inline const FName FLYING_MOVER_TAG(TEXT("Flying Mover"));
	inline const FName FOLLOW_MOVER_TAG(TEXT("Follow Mover"));
	
#pragma endregion


#pragma region UI Matching Key

	inline const FString WEAPON_REWARD_TAG(TEXT("Weapon"));
	inline const FString PASSIVEITEM_REWARD_TAG(TEXT("PassiveItem"));
	inline const FString POWERUP_REWARD_TAG(TEXT("PowerUp"));

	inline const FString REWARD_ICON(TEXT("RewardIcon"));
	inline const FString REWARD_NAME(TEXT("RewardName"));
	inline const FString REWARD_DESC(TEXT("RewardDesc"));
	
#pragma endregion

	
#pragma region Order
	
	inline constexpr int32 HIGH_ORDER = 100;
	inline constexpr int32 DEFAULT_ORDER = 0;
	inline constexpr int32 LOW_ORDER = -100;
	
#pragma endregion
	
}
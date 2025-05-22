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
	
#pragma endregion

	
#pragma region Default Tag
	
	inline const FName DEFAULT_SUN_TAG(TEXT("Sun"));
	inline const FName OBJECT_POOL_TAG(TEXT("Object Pool"));
	inline const FName DEFAULT_ENEMY_SPAWN_TAG(TEXT("Enemy Spawn Celestial"));
	inline const FName FIRE_MANAGER_TAG(TEXT("Fire Manager"));
	inline const FName HP_TAG(TEXT("HP"));
	inline const FName FLYING_MOVER_TAG(TEXT("Flying Mover"));
	inline const FName FOLLOW_MOVER_TAG(TEXT("Follow Mover"));
	
#pragma endregion

#pragma region Order
	
	inline constexpr int32 HIGH_ORDER = 100;
	inline constexpr int32 DEFAULT_ORDER = 0;
	inline constexpr int32 LOW_ORDER = -100;
	
#pragma endregion
	
}
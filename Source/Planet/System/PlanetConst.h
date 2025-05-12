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
	
#pragma endregion

	
#pragma region Default Tag
	
	inline const FName DEFAULT_SUN_TAG(TEXT("Sun"));
	inline const FName OBJECT_POOL_TAG(TEXT("Object Pool"));
	inline const FName FIRE_MANAGER_TAG(TEXT("Fire Manager"));
	
#pragma endregion
	
}
// PlanetConst.h
#pragma once



namespace PlanetConst
{
#pragma region Gameplay Statics

	static constexpr float PLAYTIME								= 1200.0f;
	static constexpr EAutoReceiveInput::Type SINGLEPLAYER_INDEX	= EAutoReceiveInput::Player0;
	
#pragma endregion

	
#pragma region Default Tag
	
	inline const FName DEFAULT_SUN_TAG(TEXT("Sun"));
	inline const FName DEFAULT_ENEMY_CELESTIAL_TAG(TEXT("Enemy Spawn Celestial"));
	
#pragma endregion
	
}
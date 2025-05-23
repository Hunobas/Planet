// GameplayUtils.h
#pragma once

#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

#include "SurvivorGameModeBase.h"

namespace GameplayUtils
{
	
#pragma region Gameplay Formulations

	inline float CalculateDamage(const float& _pawnDamage, const float& _weaponDamage = 0.0f, const float& _activeBuffScale = 1.0f)
	{
		return (_pawnDamage + _weaponDamage) * _activeBuffScale;
	}

	inline float CalulateDefaultSigmoid(const float& roughStart, const float& roughEnd, const float& _inclination, const float& _inflectionPoint, const float& x)
	{
		return roughStart + (roughEnd - roughStart)
			/ (1 + FMath::Exp(_inclination * (_inflectionPoint - x)));
	}
	
#pragma endregion


#pragma region Actor & Component Helpers

	/**
	 * 게임 모드를 가져옵니다.
	 * @param _worldContextObject	꺼내올 게임 모드의 월드 컨텍스트에 속한 오브젝트
	 * @return						우리 게임 모드
	 */
	inline ASurvivorGameModeBase* GetPlanetGameMode(const UObject* _worldContextObject)
	{
		return Cast<ASurvivorGameModeBase>(UGameplayStatics::GetGameMode(_worldContextObject));
	}
	
	/**
	 * EAutoReceiveInput 기준으로 플레이어 폰을 가져옵니다.
	 * @param _targetPlayer			EAutoReceiveInput enum 값 (Player0~, Disabled 시 에러)
	 * @param _worldContextObject	GetPlayerPawn 호출에 사용할 컨텍스트 오브젝트 (e.g. this)
	 * @return						유효한 PlayerPawn
	 */
	inline APawn* GetTargetPlayerPawn(const TEnumAsByte<EAutoReceiveInput::Type>& _targetPlayer, const UObject* _worldContextObject)
	{
		check(_worldContextObject);
		check(_targetPlayer != EAutoReceiveInput::Disabled);

		const int32 playerIndex = static_cast<int32>(_targetPlayer.GetValue()) - 1;
		return UGameplayStatics::GetPlayerPawn(_worldContextObject, playerIndex);
	}
	
	/**
	 * 액터를 구성하는 컴포넌트 중, 태그로 첫 번째 컴포넌트를 찾아 반환합니다.
	 * @tparam T				찾을 컴포넌트 타입 (USceneComponent, UStaticMeshComponent 등)
	 * @param _actorToSearch	검색 대상 액터
	 * @param _componentTag		검색할 컴포넌트의 태그
	 * @param out_component		결과 컴포넌트를 받는 레퍼런스
	 * @return					찾으면 true, 못 찾으면 false
	 */
	template<typename T>
	bool TryGetFirstComponentWithTag(const AActor* _actorToSearch, const FName& _componentTag, T*& out_component)
	{
		if (!_actorToSearch)
			return false;

		TArray<T*> components;
		_actorToSearch->GetComponents<T>(components);

		for (T* comp : components)
		{
			if (comp && comp->ComponentHasTag(_componentTag))
			{
				out_component = comp;
				return true;
			}
		}

		return false;
	}

	/**
	 * 월드 컨텍스트에서 지정한 태그의 액터를 모두 찾은 뒤, 첫 번째 것만 반환합니다.
	 * @tparam T		AActor로부터 파생된 클래스 타입
	 * @param _world	찾을 액터가 속한 월드 컨텍스트
	 * @param _actorTag	찾을 액터의 태그
	 * @param out_actor	찾은 액터 포인터
	 * @return			찾으면 true, 못 찾으면 false
	 */
	template<typename T>
	inline bool TryGetFirstActorWithTag(const UWorld* _world, const FName& _actorTag, T*& out_actor)
	{
		if (!_world)
			return false;

		static_assert(TIsDerivedFrom<T, AActor>::Value, "TryGetFirstActorWithTag<T>: T는 AActor로부터 파생된 클래스여야 함.");

		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsWithTag(_world, _actorTag, foundActors);
		
		if (foundActors.Num() == 0)
			return false;

		for (AActor* actor : foundActors)
		{
			if (T* typedActor = Cast<T>(actor))
			{
				out_actor = typedActor;
				return true;
			}
		}

		return false;
	}

	/**
	 * 이펙트를 소환해 컴포넌트 하위에 붙이고, 이펙트의 트랜스폼을 컴포넌트의 위치 및 정면 방향으로 조정합니다.
	 * @param _systemTemplate		소환할 Niagara 시스템 템플릿
	 * @param _attachToComponent	이펙트를 붙일 SceneComponent
	 * @return						소환된 이펙트 컴포넌트
	 */
	inline UNiagaraComponent* SpawnSystemAttachedFacingForward(UNiagaraSystem* _systemTemplate, USceneComponent* _attachToComponent)
	{
		if (_systemTemplate == nullptr)
			return nullptr;
		
		check(_attachToComponent);

		const FRotator forwardRotation = _attachToComponent->GetComponentRotation();

		return UNiagaraFunctionLibrary::SpawnSystemAttached(
			_systemTemplate,
			_attachToComponent,
			NAME_None,
			FVector::ZeroVector,
			forwardRotation,
			EAttachLocation::SnapToTarget,
			true
		);
	}
	
#pragma endregion


#pragma region Name Parsing

	/**
	 * 월드 컨텍스트로부터 레벨 이름 "/Game/Maps/Level_3"을 얻어서 "Level_3" 에서 뒤쪽 숫자 3만 파싱합니다.
	 * 실패 시 -1 반환
	 * @param _world	파싱할 월드 컨텍스트
	 * @return			현재 레벨 인덱스, 실패하면 -1
	 */
	inline int32 ParseLevelIndex(const UWorld* _world)
	{
		if (!_world)
			return -1;
		
		FString fullMapName = _world->GetMapName();
		FString levelName;
		if (!fullMapName.Split(TEXT("/"), nullptr, &levelName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			levelName = fullMapName;
		}

		int32 endIdx = levelName.Len() - 1;
		int32 startIdx = endIdx;
		while (startIdx >= 0 && levelName[startIdx] != '_' && FChar::IsDigit(levelName[startIdx]))
		{
			startIdx--;
		}
		UE_LOG(LogTemp, Display, TEXT("Level Name: %s [%d %d]"), *levelName, startIdx, endIdx);
		
		if (startIdx == endIdx)
		{
			return -1;
		}

		int32 numStart = startIdx + 1;
		int32 numLength = endIdx - startIdx;

		const FString numberStr = levelName.Mid(numStart, numLength);
		return FCString::Atoi(*numberStr);
	}

#pragma endregion
	
	
#pragma region Game Math

	inline float EaseOut(float num, int32 exp)
	{
		return 1 - FMath::Pow(1 - num, exp);
	}

	inline float EaseOutTetri(float num)
	{
		return EaseOut(num, 3);
	}

	inline float EaseOutQuint(float num)
	{
		return EaseOut(num, 5);
	}

	inline float NormalizeAngle(float degree)
	{
		while (degree > 180.f)
			degree -= 360.f;
		while (degree < -180.f)
			degree += 360.f;
		return degree;
	}

	inline FRotator LerpAngle(FRotator a, FRotator b, float x)
	{
		const float pitch = NormalizeAngle(a.Pitch + NormalizeAngle(b.Pitch - a.Pitch) * x);
		const float yaw = NormalizeAngle(a.Yaw + NormalizeAngle(b.Yaw - a.Yaw) * x);
		const float roll = NormalizeAngle(a.Roll + NormalizeAngle(b.Roll - a.Roll) * x);
		return FRotator(pitch, yaw, roll);
	}
	
#pragma endregion
	
}
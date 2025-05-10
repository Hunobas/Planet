// GameplayUtils.h
#pragma once

#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

namespace GameplayUtils
{
#pragma region Gameplay Formulations
	
	inline float CalculateDamage(float _pawnDamage, float _weaponDamage = 0.0f, float _damageScale = 1.0f)
	{
		return (_pawnDamage + _weaponDamage) * _damageScale;
	}
	
#pragma endregion


#pragma region Actor & Component Helpers

	/**
	 * EAutoReceiveInput 기준으로 플레이어 폰을 가져옵니다.
	 * @param _targetPlayer			EAutoReceiveInput enum 값 (Player0~3, Disabled 시 에러)
	 * @param _worldContextObject	GetPlayerPawn 호출에 사용할 컨텍스트 오브젝트 (e.g. this)
	 * @return						유효한 PlayerPawn
	 */
	inline APawn* GetTargetPlayerPawn(const TEnumAsByte<EAutoReceiveInput::Type>& _targetPlayer, const UObject* _worldContextObject)
	{
		check(_worldContextObject);
		check(_targetPlayer != EAutoReceiveInput::Disabled);

		const int32 PlayerIndex = static_cast<int32>(_targetPlayer.GetValue()) - 1;
		return UGameplayStatics::GetPlayerPawn(_worldContextObject, PlayerIndex);
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
	 * @param _world	찾을 액터가 속한 월드 컨텍스트
	 * @param _actorTag	찾을 액터의 태그
	 * @param out_actor	찾은 액터 포인터
	 * @return			찾으면 true, 못 찾으면 false
	 */
	inline bool TryGetFirstActorWithTag(const UWorld* _world, const FName& _actorTag, AActor*& out_actor)
	{
		if (!_world)
			return false;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(_world, _actorTag, FoundActors);

		if (FoundActors.Num() > 0)
		{
			out_actor = FoundActors[0];
			return true;
		}

		return false;
	}

	/**
	 * 이펙트를 소환해 컴포넌트 하위에 붙이고, 이펙트의 트랜스폼을 컴포넌트의 위치 및 정면 방향으로 조정합니다.
	 * @param _systemTemplate		소환할 Niagara 시스템 템플릿
	 * @param _attachToComponent	이펙트를 붙일 SceneComponent
	 */
	inline void SpawnSystemAttachedFacingForward(UNiagaraSystem* _systemTemplate, USceneComponent* _attachToComponent)
	{
		if (_systemTemplate == nullptr)
			return;
		
		check(_attachToComponent);

		// const FRotator ForwardRotation = _attachToComponent->GetComponentRotation();

		UNiagaraFunctionLibrary::SpawnSystemAttached(
			_systemTemplate,
			_attachToComponent,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
	
#pragma endregion
}
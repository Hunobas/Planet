// GameplayUtils.h
#pragma once

#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

namespace GameplayUtils
{
	inline float CalculateDamage(float _pawnAttack, float _weaponAttack = 0.0f, float _multiplier = 1.0f)
	{
		return (_pawnAttack + _weaponAttack) * _multiplier;
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
	bool TryGetFirstComponentWithTag(AActor* _actorToSearch, const FName& _componentTag, T*& out_component)
	{
		if (!_actorToSearch)
		{
			return false;
		}

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
	 * @param _actorTag	찾을 액터의 태그
	 * @param out_actor	찾은 액터 포인터
	 * @return			찾으면 true, 못 찾으면 false
	 */
	inline bool TryGetFirstActorWithTag(const FName& _actorTag, AActor*& out_actor)
	{
		UWorld* World = GEngine ? GEngine->GetWorldFromContextObjectChecked(GEngine) : nullptr;
		if (!World)
		{
			return false;
		}

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(World, _actorTag, FoundActors);

		if (FoundActors.Num() > 0)
		{
			out_actor = FoundActors[0];
			return true;
		}

		return false;
	}
}
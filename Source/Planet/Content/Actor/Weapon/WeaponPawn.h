// WeaponPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WeaponPawn.generated.h"

enum class EWeaponType : uint8;

UCLASS()
class PLANET_API AWeaponPawn : public APawn
{
    GENERATED_BODY()

public:
    AWeaponPawn();
    virtual void LevelUp(const int32& _newLevel);
    virtual void Fire() { }

    virtual void StartAttack() { }
    virtual void StopAttack() { }
    
    UPROPERTY(EditAnywhere, Category = "Blueprint Components")
    USceneComponent* Root;

    UPROPERTY(EditAnywhere, Category = "Config")
    UTexture2D* WeaponIcon;
    UPROPERTY(EditAnywhere, Category = "Config")
    EWeaponType WeaponType;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    float Damage        = 25.0f;
    UPROPERTY(EditAnywhere, Category = "Combat")
    float BaseFireRate  = 0.5f;
    UPROPERTY(EditAnywhere, Category = "DEBUG")
    int32 CurrentLevel  = 0;

};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Ability/PWEffectReceiver.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ProjectWTpsCharacter.generated.h"

class UPWAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AProjectWTpsCharacter : public ACharacter, public IAbilitySystemInterface, public IPWEffectReceiver
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MouseLookAction;

public:
	AProjectWTpsCharacter();
	
	//~IPWEffectReceiver
	virtual UObject* GetEffectReceiver_Implementation() override;
protected:
	//~UE classes
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnAttributeChanged_MovementSpeed(const FOnAttributeChangeData& OnAttributeChangeData);
	virtual void SetupComponents();
	UFUNCTION(BlueprintImplementableEvent)
	void OnSetupComponents();

protected:
	void OnGameplayTagUpdate(FGameplayTag Tag, int Count);
	UFUNCTION(BlueprintImplementableEvent, Category="Input", meta=(DisplayName="OnGameplayTagUpdate"))
	void BP_OnGameplayTagUpdate(FGameplayTag Tag, int Count);
	
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	bool bWasMovingLastFrame = false;

public:
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPWAbilitySystemComponent* GetASC() const;
};


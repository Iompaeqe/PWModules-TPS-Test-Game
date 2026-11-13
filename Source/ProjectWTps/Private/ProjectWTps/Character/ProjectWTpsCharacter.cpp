// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectWTPS/Character/ProjectWTpsCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ProjectWTps.h"
#include "ProjectWTPS/Tags/ProjectWTpsTags.h"
#include "GAS/ASC/PWAbilitySystemComponent.h"
#include "Macros/LogMacros.h"
#include "ProjectWTPS/AbilitySystem/AttributeSet_Core.h"
#include "ProjectWTps/AbilitySystem/Library/AttributeLibrary.h"
#include "ProjectWTPS/Player/TPSPlayerState.h"
#include "ProjectWTPS/AI/TPSAIController.h"

AProjectWTpsCharacter::AProjectWTpsCharacter()
{
	// A character placed in the level with no player becomes a test AI caster.
	AIControllerClass = ATPSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.6f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

UObject* AProjectWTpsCharacter::GetEffectReceiver_Implementation()
{
	return GetPlayerState();
}

void AProjectWTpsCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectWTpsCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	SetupComponents();
}

void AProjectWTpsCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetupComponents();
}

void AProjectWTpsCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const bool bIsMovingNow = GetVelocity().SizeSquared() > KINDA_SMALL_NUMBER;
	if (bIsMovingNow != bWasMovingLastFrame)
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			if (bIsMovingNow)
				ASC->AddLooseGameplayTag(PWTags::ASC::State::Moving);
			else
				ASC->RemoveLooseGameplayTag(PWTags::ASC::State::Moving);
		}
		bWasMovingLastFrame = bIsMovingNow;
	}
}

void AProjectWTpsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectWTpsCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &AProjectWTpsCharacter::StopMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AProjectWTpsCharacter::StopMove);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AProjectWTpsCharacter::Look);
	}
	else
	{
		UE_LOG(LogProjectWTps, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectWTpsCharacter::OnAttributeChanged_MovementSpeed(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AProjectWTpsCharacter::SetupComponents()
{
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	ASC->RegisterGenericGameplayTagEvent().AddUObject(this, &ThisClass::OnGameplayTagUpdate);
	ASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Core::GetMovementSpeedAttribute()).AddUObject(this, &ThisClass::OnAttributeChanged_MovementSpeed);

	OnSetupComponents();
}

void AProjectWTpsCharacter::OnGameplayTagUpdate(const FGameplayTag Tag, const int Count)
{
	if (Tag.MatchesTag(PWTags::ASC::State::Casting::Root))
	{
		if (Count > 0) GetCharacterMovement()->bOrientRotationToMovement = false;
		else GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	BP_OnGameplayTagUpdate(Tag, Count);
}

void AProjectWTpsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	GetCharacterMovement()->bOrientRotationToMovement = false;

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AProjectWTpsCharacter::StopMove(const FInputActionValue& Value)
{
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(PWTags::ASC::State::Casting::Root)) return;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AProjectWTpsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, -LookAxisVector.Y);
}

void AProjectWTpsCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AProjectWTpsCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AProjectWTpsCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AProjectWTpsCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

UAbilitySystemComponent* AProjectWTpsCharacter::GetAbilitySystemComponent() const
{
	return Cast<UPWAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPlayerState<ATPSPlayerState>()));
}

UPWAbilitySystemComponent* AProjectWTpsCharacter::GetASC() const
{
	return Cast<UPWAbilitySystemComponent>(GetAbilitySystemComponent());
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "TatakaeRunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATatakaeRunnerCharacter::ATatakaeRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	MoveDirection = 1;
	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	SetDefaultMovement();
	CurrentHealth = MaxHealth;
	// Configure character movement


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////

void ATatakaeRunnerCharacter::SetDefaultMovement()
{
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
}


void ATatakaeRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Gravity", IE_Pressed, this, &ATatakaeRunnerCharacter::GravityStart);
	PlayerInputComponent->BindAction("Gravity", IE_Released, this, &ATatakaeRunnerCharacter::GravityEnd);
	PlayerInputComponent->BindAction("ChangeView", IE_Pressed, this, &ATatakaeRunnerCharacter::ChangeView);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATatakaeRunnerCharacter::MoveRight);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATatakaeRunnerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATatakaeRunnerCharacter::TouchStopped);
}

void ATatakaeRunnerCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value * MoveDirection);
}

void ATatakaeRunnerCharacter::ChangeView()
{
	CameraBoom->AddRelativeRotation(FRotator(0, 180, 0));
	MoveDirection *= -1;
}

void ATatakaeRunnerCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ATatakaeRunnerCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ATatakaeRunnerCharacter::GravityStart()
{
	GetCharacterMovement()->GravityScale = 7.f;
}

void ATatakaeRunnerCharacter::GravityEnd()
{
	SetDefaultMovement();
}

void ATatakaeRunnerCharacter::TakeDamage(int Value)
{
	if (Value >= CurrentHealth)
	{
		CurrentHealth = 0;
		RestartLevel();
	}
	else
	{
		CurrentHealth -= Value;
	}
}

void ATatakaeRunnerCharacter::RestartLevel()
{
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TatakaeRunnerCharacter.generated.h"

UCLASS(config = Game)
class ATatakaeRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		int MoveDirection;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		int CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		int MaxHealth = 100; 
	//Sets the default preferences for character movement
	void SetDefaultMovement();
	//Called for side to side input
	void MoveRight(float Val);
	//Changes the side of thge camera view by 180 degrees
	void ChangeView();
	//Handle touch inputs
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	//Handle touch stop event
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);
	//Increase the acceleration of gravity
	void GravityStart();
	//Stop the increased gravity mode
	void GravityEnd();
	//Decreasing the current health of the character
	UFUNCTION()
	void TakeDamage(int Value);
	//Restarts the level
	UFUNCTION()
	void RestartLevel();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	ATatakaeRunnerCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

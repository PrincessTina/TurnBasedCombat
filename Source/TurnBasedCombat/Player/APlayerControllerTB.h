#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "APlayerControllerTB.generated.h"

class ACamera;
class APlayerCharacter;
class UNiagaraSystem;
struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(abstract)
class APlayerControllerTB : public APlayerController
{
	GENERATED_BODY()
	
protected:
	/** Input mapping context to use with mouse input */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* UIMappingContext;
	
	UPROPERTY()
	UInputMappingContext* CurrentMappingContext = nullptr;
	
	/** Input Action for zooming the camera */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ZoomCameraAction;
	
	/** Input Action for rotating the camera */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* RotateCameraAction;
	
	/** Input Action for moving to the destination */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveToAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UNiagaraSystem* FXDestinationPoint = nullptr;
	
	UPROPERTY(BlueprintReadWrite , Category="Input")
	FVector MovementDestination = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly, Category="Camera")
	ACamera* CameraPawn = nullptr;
	
	UPROPERTY(BlueprintReadWrite, Category="Player")
	APlayerCharacter* ControlledCharacter = nullptr;

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void SetMappingContext(const bool IsFocusedOnUI);
	
	UFUNCTION(BlueprintCallable, Category="Player")
	void SelectPlayerCharacter(APlayerCharacter* PlayerCharacter);
	
protected:
	UFUNCTION(BlueprintPure, Category="Camera")
	USpringArmComponent* GetCameraSpringArm() const;
	
	// The implementation is in the blueprint because of Timeline usage
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void ZoomCamera(const FInputActionValue& Value);
	
	void RotateCamera(const FInputActionValue& Value);
	void MoveToDestination(const FInputActionInstance& Instance);
	
private:
	/** Set cursor in the center of the screen */
	void CenterMouseOnScreen();
};
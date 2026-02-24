#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "APlayerControllerTB.generated.h"

class UNiagaraSystem;
struct FInputActionInstance;
class USpringArmComponent;
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
	USpringArmComponent* CameraSpringArm = nullptr;

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION(BlueprintCallable, Category="Input")
	void SetMappingContext(const bool IsFocusedOnUI);
	
protected:
	// The implementation is in the blueprint because of Timeline usage
	UFUNCTION(BlueprintImplementableEvent, Category="Camera")
	void ZoomCamera(const FInputActionValue& Value);
	
	void RotateCamera(const FInputActionValue& Value);
	void MoveToDestination(const FInputActionInstance& Instance);
	
private:
	/** Set cursor in the center of the screen */
	void CenterMouseOnScreen();
	/** Edge scrolling logic */
	void HandleCameraMovement() const;
	void ResetCameraMovement() const;
};
#pragma once

#include "CoreMinimal.h"
#include "ACamera.generated.h"

struct FInputActionValue;
class APlayerControllerTB;
class USpringArmComponent;
class APlayerCharacter;

UCLASS(abstract)
class ACamera : public APawn
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArm = nullptr;
	
private:
	UPROPERTY()
	APlayerControllerTB* PlayerController = nullptr;
	
	UPROPERTY()
	APlayerCharacter* FollowedCharacter = nullptr;
	
	bool bEnableCameraMovement = false;
	bool bFollowActiveCharacter = false;
	
public:
	ACamera();
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* InController) override;
	
	USpringArmComponent* GetSpringArm() const { return SpringArm; }
	void SetEnableCameraMovement(const bool EnableCameraMovement) { bEnableCameraMovement = EnableCameraMovement; }
	
	void RotateCamera(const FInputActionValue& Value) const;
	void CenterOnPlayerCharacter(APlayerCharacter* PlayerCharacter);
	
private:
	/** Edge scrolling logic */
	void HandleCameraMovement() const;
};
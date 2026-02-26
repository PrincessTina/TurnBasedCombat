#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "APlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterMovementStarted);
DECLARE_MULTICAST_DELEGATE(FOnCharacterMovementFinished);

UCLASS(abstract)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	FOnCharacterMovementStarted OnCharacterMovementStarted;
	FOnCharacterMovementFinished OnCharacterMovementFinished;
	
public:
	APlayerCharacter();
	
	void MoveToLocation(const FVector& Location) const;
	void StopMovement() const;

protected:
	virtual void NotifyControllerChanged() override;
};
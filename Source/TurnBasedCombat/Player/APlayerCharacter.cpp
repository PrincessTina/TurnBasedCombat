#include "APlayerCharacter.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// ensure this unit has a valid AI controller to handle move requests
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// configure movement
	GetCharacterMovement()->GravityScale = 1.5f;
	GetCharacterMovement()->MaxAcceleration = 1000.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.0f;
	GetCharacterMovement()->PerchRadiusThreshold = 20.0f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	/* Additional stats (weren't set by default in tutorials' Blueprint */
	GetCharacterMovement()->AvoidanceConsiderationRadius = 150.0f;
	GetCharacterMovement()->AvoidanceWeight = 1.0f;
	/**/
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->SetFixedBrakingDistance(200.0f);
	GetCharacterMovement()->SetFixedBrakingDistance(true);
}

void APlayerCharacter::MoveToLocation(const FVector& Location) const
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Location);
	OnCharacterMovementStarted.Broadcast();
}

void APlayerCharacter::StopMovement() const
{
	GetCharacterMovement()->StopMovementImmediately();
}

void APlayerCharacter::NotifyControllerChanged()
{
	if (const AAIController* AIController = Cast<AAIController>(Controller))
	{
		if (UPathFollowingComponent* PFComp = AIController->GetPathFollowingComponent())
		{
			PFComp->OnRequestFinished.AddLambda([this](FAIRequestID RequestID, const FPathFollowingResult& Result)
			{
				OnCharacterMovementFinished.Broadcast();
			});
		}
	}
}
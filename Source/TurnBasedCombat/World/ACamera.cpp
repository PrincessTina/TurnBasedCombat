#include "ACamera.h"

#include "InputActionValue.h"
#include "IntVectorTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/APlayerCharacter.h"
#include "Player/APlayerControllerTB.h"

ACamera::ACamera()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	
	SpringArm->SetWorldRotation(FRotator(-40.f, 45.f, 0.f));
	SpringArm->TargetArmLength = 1100.f;
	SpringArm->SocketOffset = FVector(0, 0, 40);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;
}

void ACamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bFollowActiveCharacter && SpringArm->TargetOffset == FVector::ZeroVector)
	{
		SetActorLocation(FollowedCharacter->GetActorLocation());
	}
	
	if (bEnableCameraMovement)
	{
		HandleCameraMovement();
	}
}

void ACamera::PossessedBy(AController* InController)
{
	Super::PossessedBy(InController);
	
	PlayerController = Cast<APlayerControllerTB>(InController);
	check(PlayerController);
}

void ACamera::RotateCamera(const FInputActionValue& Value) const
{
	constexpr float RotationScaling = 3.f;
	SpringArm->AddWorldRotation(FRotator(0.f, Value.Get<float>() * RotationScaling, 0.f));
}

void ACamera::CenterOnPlayerCharacter(APlayerCharacter* PlayerCharacter)
{
	if (FollowedCharacter != PlayerCharacter)
	{
		if (FollowedCharacter)
		{
			FollowedCharacter->OnCharacterMovementStarted.RemoveAll(this);
			FollowedCharacter->OnCharacterMovementFinished.RemoveAll(this);
		}
		
		FollowedCharacter = PlayerCharacter;
		
		FollowedCharacter->OnCharacterMovementStarted.AddLambda([this](){ bFollowActiveCharacter = true; });
		FollowedCharacter->OnCharacterMovementFinished.AddLambda([this](){ bFollowActiveCharacter = false; });
	}
	
	SetActorLocation(FollowedCharacter->GetActorLocation());
	SpringArm->TargetOffset = FVector::ZeroVector;
}

void ACamera::HandleCameraMovement() const
{
	const FVector MoveScaling = FVector(8.f, 8.f, 0.f);
	const FVector RightVector = SpringArm->GetRightVector();
	const FVector ForwardVector = SpringArm->GetForwardVector();
	UE::Geometry::FVector2i ViewportSize;
	FVector2f MousePosition = FVector2f::ZeroVector;
	FVector MoveDirection = FVector::ZeroVector;
	
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	PlayerController->GetViewportSize(ViewportSize.X, ViewportSize.Y);
	
	MousePosition.X /= ViewportSize.X;
	MousePosition.Y /= ViewportSize.Y;
	
	if (MousePosition.X > 0.99f)
	{
		MoveDirection = MoveScaling * RightVector;
	} else if (MousePosition.X < 0.01f)
	{
		MoveDirection = -1.f * MoveScaling * RightVector;
	}
	
	if (MousePosition.Y > 0.99f)
	{
		MoveDirection += -1.f * MoveScaling * ForwardVector;
	} else if (MousePosition.Y < 0.01f)
	{
		MoveDirection += MoveScaling * ForwardVector;
	}
	
	SpringArm->TargetOffset += MoveDirection;
}
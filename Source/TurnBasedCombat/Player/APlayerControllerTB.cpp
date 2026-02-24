#include "APlayerControllerTB.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "IntVectorTypes.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"

void APlayerControllerTB::BeginPlay()
{
	Super::BeginPlay();
	CenterMouseOnScreen();
}

void APlayerControllerTB::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		SetMappingContext(false);

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &APlayerControllerTB::ZoomCamera);
			EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &APlayerControllerTB::RotateCamera);
			EnhancedInputComponent->BindAction(MoveToAction, ETriggerEvent::Completed, this, &APlayerControllerTB::MoveToDestination);
		}
	}
}

void APlayerControllerTB::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	HandleCameraMovement();
}

void APlayerControllerTB::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CameraSpringArm = InPawn->GetComponentByClass<USpringArmComponent>();
	check(CameraSpringArm);
}

void APlayerControllerTB::SetMappingContext(const bool IsFocusedOnUI)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		UInputMappingContext* MappingContext = IsFocusedOnUI ? UIMappingContext : DefaultMappingContext;
		Subsystem->RemoveMappingContext(CurrentMappingContext);
		Subsystem->AddMappingContext(MappingContext, 0);
		CurrentMappingContext = MappingContext;
	}
}

void APlayerControllerTB::RotateCamera(const FInputActionValue& Value)
{
	constexpr float RotationScaling = 3.f;
	CameraSpringArm->AddWorldRotation(FRotator(0.f, Value.Get<float>() * RotationScaling, 0.f));
}

void APlayerControllerTB::MoveToDestination(const FInputActionInstance& Instance)
{
	constexpr float PressedThreshold = 0.5f;
	if (Instance.GetElapsedTime() >= PressedThreshold)
	{
		return;
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MovementDestination);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXDestinationPoint, MovementDestination, 
		FRotator::ZeroRotator,FVector::OneVector, true, true, ENCPoolMethod::None, true);
}

void APlayerControllerTB::CenterMouseOnScreen()
{
	FTimerHandle TimerUntilNextFrame;
	GetWorldTimerManager().SetTimer(TimerUntilNextFrame, [this, &TimerUntilNextFrame]()
	{
		UE::Geometry::FVector2i ViewportSize;
		GetViewportSize(ViewportSize.X, ViewportSize.Y);
		SetMouseLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
		
		GetWorldTimerManager().ClearTimer(TimerUntilNextFrame);
	}, 0.01f, false);
}

void APlayerControllerTB::HandleCameraMovement() const
{
	const FVector MoveScaling = FVector(8.f, 8.f, 0.f);
	const FVector RightVector = CameraSpringArm->GetRightVector();
	const FVector ForwardVector = CameraSpringArm->GetForwardVector();
	UE::Geometry::FVector2i ViewportSize;
	FVector2f MousePosition = FVector2f::ZeroVector;
	FVector MoveDirection = FVector::ZeroVector;
	
	GetMousePosition(MousePosition.X, MousePosition.Y);
	GetViewportSize(ViewportSize.X, ViewportSize.Y);
	
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
	
	CameraSpringArm->TargetOffset += MoveDirection;
}

void APlayerControllerTB::ResetCameraMovement() const
{
	CameraSpringArm->TargetOffset = FVector::ZeroVector;
}
#include "APlayerControllerTB.h"

#include "APlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IntVectorTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "World/ACamera.h"

void APlayerControllerTB::BeginPlay()
{
	Super::BeginPlay();
	
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::LockAlways);
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

void APlayerControllerTB::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	CameraPawn = Cast<ACamera>(InPawn);
	check(CameraPawn);
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

void APlayerControllerTB::SelectPlayerCharacter(APlayerCharacter* PlayerCharacter)
{
	if (ControlledCharacter && ControlledCharacter != PlayerCharacter)
	{
		ControlledCharacter->StopMovement();
	}
	
	ControlledCharacter = PlayerCharacter;
	check(ControlledCharacter);
	CameraPawn->CenterOnPlayerCharacter(ControlledCharacter);
}

USpringArmComponent* APlayerControllerTB::GetCameraSpringArm() const
{
	if (CameraPawn)
	{
		return CameraPawn->GetSpringArm();
	}
	
	return nullptr;
}

void APlayerControllerTB::RotateCamera(const FInputActionValue& Value)
{
	CameraPawn->RotateCamera(Value);
}

void APlayerControllerTB::MoveToDestination(const FInputActionInstance& Instance)
{
	constexpr float PressedThreshold = 0.5f;
	if (Instance.GetElapsedTime() >= PressedThreshold)
	{
		return;
	}
	
	ControlledCharacter->MoveToLocation(MovementDestination);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FXDestinationPoint, MovementDestination, 
		FRotator::ZeroRotator,FVector::OneVector, true, true, ENCPoolMethod::None, true);
}

void APlayerControllerTB::CenterMouseOnScreen()
{
	FTimerHandle TimerUntilNextFrame;
	GetWorldTimerManager().SetTimer(TimerUntilNextFrame, [this, &TimerUntilNextFrame]()
	{
		GetWorldTimerManager().ClearTimer(TimerUntilNextFrame);
		
		UE::Geometry::FVector2i ViewportSize;
		GetViewportSize(ViewportSize.X, ViewportSize.Y);
		SetMouseLocation(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
		
		CameraPawn->SetEnableCameraMovement(true);
	}, 0.01f, false);
}
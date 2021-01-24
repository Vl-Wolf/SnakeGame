// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "SnakeBase.h"
#include "Food.h"
#include "Interactable.h"
#include "Bonus.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	RootComponent = PawnCamera;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(-90, 0, 0));
	CreateSnakeActor();
	CreateFoodActor();
	CreateBonusActor();
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawnBase::HandlePlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawnBase::HandlePlayerHorizontalInput);
}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
}

void APlayerPawnBase::CreateFoodActor() 
{
	int a = -790;
	int b = 800;
	int x = rand() % (b - a) + a;
	int y = rand() % (b - a) + a;
	FVector NewLocation(x, y, 30);
	FTransform NewTransform(NewLocation);
	FoodActor = GetWorld()->SpawnActor<AFood>(FoodActorClass, NewTransform);
}

void APlayerPawnBase::CreateBonusActor()
{
	int a = -790;
	int b = 800;
	int x = rand() % (b - a) + a;
	int y = rand() % (b - a) + a;
	FVector NewLocation(x, y, 30);
	FTransform NewTransform(NewLocation);
	BonusActor = GetWorld()->SpawnActor<ABonus>(BonusActorClass, NewTransform);
}

void APlayerPawnBase::HandlePlayerVerticalInput(float value)
{
	if (IsValid(SnakeActor))
	{
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::DOWN)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::UP;
		}
		if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::UP)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::DOWN;
		}
	}
}

void APlayerPawnBase::HandlePlayerHorizontalInput(float value)
{
	if (IsValid(SnakeActor))
	{
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::LEFT)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::RIGHT;
		}
		if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::RIGHT)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::LEFT;
		}
	}
}

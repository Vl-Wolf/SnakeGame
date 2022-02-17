// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"
// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 60.f;
	MovementSpeed = float(0.4);
	LastMoveDirection = EMovementDirection::DOWN;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorTickInterval(MovementSpeed);
	Move();
	//SnakeDeadZone();
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->SetActorHiddenInGame(true);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0)
		{
			NewSnakeElem->SetFirstElementType();
			NewSnakeElem->SetActorHiddenInGame(false);
		}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);

	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}

	
	//AddActorWorldOffset(MovementVector);

	SnakeElements[0]->ToggleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
		CurrentElement->SetActorHiddenInGame(false);
	}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirst);
		}
	}


}

void ASnakeBase::SnakeDeadZone()
{
	auto SnakeHead = SnakeElements[0];
	FVector HeadLocation = SnakeHead->GetActorLocation();
	if ((HeadLocation.X < -970) && (HeadLocation.X > 970))
	{
		Destroy();
	}
	if ((HeadLocation.Y < -970) && (HeadLocation.Y > 970))
	{
		Destroy();
	}
}
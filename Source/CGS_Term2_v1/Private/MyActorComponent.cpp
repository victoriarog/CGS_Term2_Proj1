// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorComponent.h"

// Sets default values for this component's properties
UMyActorComponent::UMyActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();
	//DesiredRotation = FRotator(0.0f, 90.0f, 0.0f);
	//DeltaRotation = DesiredRotation - GetOwner()->GetActorRotation();
	//FinalRotation = GetOwner()->GetActorRotation() + DesiredRotation;
	
	// ...
	
}


// Called every frame
void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	if (!CurrentRotation.Equals(FinalRotation, 5.0f))
	{
		CurrentRotation += DeltaRotation * DeltaTime;
		GetOwner()->SetActorRotation(CurrentRotation);
	}

	// ...
}


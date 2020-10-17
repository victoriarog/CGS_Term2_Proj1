// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorComponentv1ch1.h"
#include "Math/UnrealMathUtility.h"


// Sets default values for this component's properties
UMyActorComponentv1ch1::UMyActorComponentv1ch1()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MinSpeed = 0.f;
	MaxSpeed = 1.f;

	// ...
}


// Called when the game starts
void UMyActorComponentv1ch1::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
	
}


// Called every frame
void UMyActorComponentv1ch1::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* SpaceOwner = GetOwner();
	FVector CurrentLocation = SpaceOwner ? SpaceOwner->GetActorLocation() : FVector();
	CurrentLocation.Z += CurrentSpeed * DeltaTime;
	if (SpaceOwner) 
	{
		SpaceOwner->SetActorLocation(CurrentLocation);
	}

}


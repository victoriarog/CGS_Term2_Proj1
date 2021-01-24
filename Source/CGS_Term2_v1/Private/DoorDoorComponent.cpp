// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorDoorComponent.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UDoorDoorComponent::UDoorDoorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("box capsule"));
	if (GetOwner() && GetOwner()->GetRootComponent())
	{
		BoxComponent->SetupAttachment(GetOwner()->GetRootComponent());
	}
	else if(GetOwner())
	{
		GetOwner()->SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	}
	
	//binding to events
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UDoorDoorComponent::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UDoorDoorComponent::OnOverlapEnd);
}

void UDoorDoorComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("UDoorDoorComponent::OnOverlapBegin"));
	OverLapInfo.Actor = OtherActor;
	FVector OwnerForward = GetOwner()->GetActorRightVector();
	FVector ActorToOverlapPos = GetOwner()->GetActorLocation() - OtherActor->GetActorLocation();
	OverLapInfo.DotResult = FVector::DotProduct(OwnerForward, ActorToOverlapPos.GetSafeNormal());
	UE_LOG(LogTemp, Warning, TEXT("UDoorDoorComponent::BeginDot %f"), OverLapInfo.DotResult);
}


void UDoorDoorComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UDoorDoorComponent::OnOverlapEnd"));
	if (OverLapInfo.Actor == OtherActor)
	{
		OverLapInfo.Actor = OtherActor;
		FVector OwnerForward = GetOwner()->GetActorRightVector();
		FVector ActorToOverlapPos = GetOwner()->GetActorLocation() - OtherActor->GetActorLocation();
		float DotResult = FVector::DotProduct(OwnerForward, ActorToOverlapPos.GetSafeNormal());
		bool isSuccess = false;
		if (OverLapInfo.DotResult > 0.0f)
		{
			isSuccess = DotResult < 0.0f;
		}
		else
		{
			isSuccess = DotResult > 0.0f;
		}
		if (isSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("UDoorDoorComponent SUCCESS walked through door"));
		}
		//UE_LOG(LogTemp, Warning, TEXT("UDoorDoorComponent::EndDot %f"), OverLapInfo.DotResult);
	}

	OverLapInfo.Actor = nullptr;
	OverLapInfo.DotResult = 0.0f;
	
}

// Called when the game starts
void UDoorDoorComponent::BeginPlay()
{
	Super::BeginPlay();

	StartRotation = GetOwner()->GetActorRotation();
	FinalRotation = GetOwner()->GetActorRotation() + DesiredRotation;
	CurrentRotationTime = 0.0f;


	// ...
	
}


// Called every frame
void UDoorDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentRotationTime < TimeToRotate)
	{
		//if (TriggerBox && GetWorld() && GetWorld()->GetFirstLocalPlayerFromController())
		{
			//APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (OverLapInfo.Actor != nullptr)
			{
				CurrentRotationTime += DeltaTime;
				const float TimeRatio = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.0f, 1.0f);
				const float RotationAlpha = OpenCurve.GetRichCurveConst()->Eval(TimeRatio);
				//we want the door to open away from us
				//so we use the dot product to let us know which way it should rotate
				FRotator AdjustedFinalRotation = FinalRotation;
				if (IsAnnoying)
				{
					AdjustedFinalRotation = OverLapInfo.DotResult < 0 ? FinalRotation * -1.0f : FinalRotation;
				}
				else
				{
					AdjustedFinalRotation = OverLapInfo.DotResult > 0 ? FinalRotation * -1.0f : FinalRotation;
				}
				const FRotator CurrentRotation = FMath::Lerp(StartRotation, AdjustedFinalRotation, RotationAlpha);
				GetOwner()->SetActorRotation(CurrentRotation);
			}
		}
	}
	// ...
}


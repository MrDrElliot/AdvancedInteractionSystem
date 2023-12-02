// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "InteractionOption.h"
#include "InteractionQuery.h"
#include "InteractionMethods/InteractionMethod.h"
#include "Interface/InteractionTarget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	

}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabledAsync(bUsePersistentTrace);
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(bUsePersistentTrace && PersistentInteractionMethod)
	{
		PersistentInteraction(PersistentInteractionMethod);
	}
}

void UInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

#if WITH_EDITOR
void UInteractionComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	/** Check if bUsesPersistentTrace has been changed, and created or remove the object as needed. */
	if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, bUsePersistentTrace))
	{
		if(bUsePersistentTrace && PersistentInteractMethodClass && !PersistentInteractionMethod)
		{
			PersistentInteractionMethod = NewObject<UInteractionMethod>(this, PersistentInteractMethodClass);
		}
	}
	
	/** Check if the persistent interact class method has been changed, and created or remove the object as needed. */
	if(PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, PersistentInteractMethodClass))
	{
		if(PersistentInteractMethodClass)
		{
			PersistentInteractionMethod = NewObject<UInteractionMethod>(this, PersistentInteractMethodClass);
			return;
		}
		PersistentInteractionMethod = nullptr;
	}
}
#endif
	
void UInteractionComponent::TryInteraction(TSubclassOf<UInteractionMethod> InteractionMethod, float Range, float Accuracy, bool TraceComplex, EDrawDebugTrace::Type DebugMode, FGameplayTag  InteractionTag, ETraceTypeQuery TraceChannel, EInteractionReplicationMethod ReplicationMethod, FInteractionOption& OutOption)
{
	UInteractionMethod* InteractMethod = NewObject<UInteractionMethod>(GetOwner(), InteractionMethod);
	InteractMethod->InteractionComponent = this;

	FInteractionQuery InteractionQuery;
	InteractionQuery.RequestingActor = GetOwner();
	InteractionQuery.RequestingController = GetControllerFromOwner();
	InteractionQuery.CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	
	const TArray<FInteractionOption> InteractionOptions = InteractMethod->GetTargetData(Range, Accuracy, TraceComplex, DebugMode, InteractionTag, TraceChannel, InteractionQuery);
	
	for (auto Option : InteractionOptions)
	{
		OnInteractTraceHit.Broadcast(Option);
		
		if(!Option.GetInteractableTarget()) return;
	
		OutOption = Option;
	
		if(!Option.GetInteractableTarget()->Implements<UInteractionTarget>()) return;

		if(GetOwnerRole() == ROLE_Authority && ReplicationMethod != NetMulticast)
		{
			IInteractionTarget::Execute_InteractedWith(Option.GetInteractableTarget(), InteractionQuery, Option);
		}
	
		if(GetOwnerRole() != ROLE_Authority && ReplicationMethod == Local)
		{
			IInteractionTarget::Execute_InteractedWith(Option.GetInteractableTarget(), InteractionQuery, Option);
			return;
		}

		if(GetOwnerRole() == ROLE_Authority)
		{
			if(ReplicationMethod == NetMulticast)
			{
				Multicast_TryInteraction(InteractionQuery, Option);
				return;
			}
			if(ReplicationMethod == Server)
			{
				return;
			}
		}
	
		if(ReplicationMethod == Server)
		{
			Server_TryInteraction(InteractionQuery, Option, Server);
			return;
		}
		if(ReplicationMethod == NetMulticast)
		{
			Server_TryInteraction(InteractionQuery, Option, NetMulticast);
		}
	}
	
}

void UInteractionComponent::PersistentInteraction(UInteractionMethod* InteractionMethod)
{
		FInteractionQuery InteractionQuery;
		InteractionMethod->InteractionComponent = this;
		InteractionQuery.RequestingActor = GetOwner();
		InteractionQuery.RequestingController = GetControllerFromOwner();
		InteractionQuery.CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	
	TArray<FInteractionOption> InteractionOptions = InteractionMethod->GetTargetData(PersistentTraceRange, PersistentAccuracy, bPersistentTraceComplex, PersistentDrawDebug, PersistentGameplayTag, PersistentTraceTypeQuery, InteractionQuery);

	for (const auto Option : InteractionOptions)
	{
		OnPersistentTrace.Broadcast(Option);
	}
}


void UInteractionComponent::Multicast_TryInteraction_Implementation(const FInteractionQuery InteractionQuery, const FInteractionOption InteractionOption)
{
	IInteractionTarget::Execute_InteractedWith(InteractionOption.GetInteractableTarget(), InteractionQuery, InteractionOption);
}

void UInteractionComponent::Server_TryInteraction_Implementation(const FInteractionQuery InteractionQuery, const FInteractionOption InteractionOption, EInteractionReplicationMethod ReplicationMethod)
{
	if(ReplicationMethod == NetMulticast)
	{
		Multicast_TryInteraction(InteractionQuery, InteractionOption);
		return;
	}
	if(!InteractionOption.InteractableTarget.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("InteractionComponent::Server_TryInteraction || Interact Target is not valid"))
	}
	IInteractionTarget::Execute_InteractedWith(InteractionOption.GetInteractableTarget(), InteractionQuery, InteractionOption);
}

AController* UInteractionComponent::GetControllerFromOwner() const
{
	const APawn* Pawn = CastChecked<APawn>(GetOwner());

	return (Pawn ? Pawn->GetController() : nullptr );
}


  
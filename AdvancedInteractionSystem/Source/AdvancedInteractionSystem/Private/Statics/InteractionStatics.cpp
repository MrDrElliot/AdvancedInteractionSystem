// Fill out your copyright notice in the Description page of Project Settings.


#include "Statics/InteractionStatics.h"

#include "GameplayTagContainer.h"
#include "InteractionOption.h"
#include "InteractionQuery.h"

FInteractionOption UInteractionStatics::MakeInteractOptionFromHitResult(const FHitResult& HitResult, FGameplayTag GameplayTag)
{
	FInteractionOption InteractionOption;
	InteractionOption.HitResult = HitResult;
	InteractionOption.GameplayTag = GameplayTag;
	InteractionOption.InteractableTarget = HitResult.GetActor();
	return InteractionOption;
}

void UInteractionStatics::ForwardTraceHelperFromComponent(const USceneComponent* SceneComponent, const float Range, FVector& Start, FVector& End)
{
	const FVector S = SceneComponent->GetComponentLocation();
	const FVector E = S + SceneComponent->GetForwardVector() * Range;
	Start = S;
	End = E;
}

void UInteractionStatics::ForwardTraceHelperFromActor(const AActor* Actor, const float Range, FVector& Start, FVector& End)
{
	const FVector S = Actor->GetActorLocation();
	const FVector E = S + Actor->GetActorForwardVector() * Range;
	Start = S;
	End = E;
}

bool UInteractionStatics::IsValidInteractionQuery(const FInteractionQuery& Query)
{
	return Query.RequestingActor.IsValid();
}

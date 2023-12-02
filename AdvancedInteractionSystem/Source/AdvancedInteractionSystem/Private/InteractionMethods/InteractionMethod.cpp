// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMethods/InteractionMethod.h"

#include "InteractionOption.h"
#include "InteractionQuery.h"


UWorld* UInteractionMethod::GetWorldContext() const
{
	return GetWorld();
}

TArray<FInteractionOption> UInteractionMethod::GetTargetData_Implementation(float Range, float Accuracy, bool TraceComplex, EDrawDebugTrace::Type DebugMode, FGameplayTag InteractionTag, ETraceTypeQuery TraceChannel, FInteractionQuery& InteractionQuery)
{
	TArray<FInteractionOption> Option;
	return Option;
}


UInteractionComponent* UInteractionMethod::GetCallingComponent() const
{
	return InteractionComponent.Get();
}

// Copyright 2023 TheOtherSideGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionMethod.h"
#include "InteractMethod_ClosestToCenter.generated.h"

class UPrimitiveComponent;
/**
 * 
 */
UCLASS()
class ADVANCEDINTERACTIONSYSTEM_API UInteractMethod_ClosestToCenter : public UInteractionMethod
{
	GENERATED_BODY()

	virtual TArray<FInteractionOption> GetTargetData_Implementation(float Range, float Accuracy, bool TraceComplex, EDrawDebugTrace::Type DebugMode, FGameplayTag InteractionTag, ETraceTypeQuery TraceChannel, FInteractionQuery& InteractionQuery) override;

	float GetComponentDistanceFromLine(const FVector& Start, const FVector& End, const UPrimitiveComponent* Component, FName HitBone) const;

	EDrawDebugTrace::Type DrawDebugType;

	float InteractRange;
};

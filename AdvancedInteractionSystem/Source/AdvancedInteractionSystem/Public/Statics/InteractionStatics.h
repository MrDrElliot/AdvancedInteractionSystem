// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InteractionStatics.generated.h"

struct FInteractionQuery;
struct FGameplayTag;
struct FInteractionOption;
class IInteractionTarget;
/**
 * 
 */
UCLASS()
class ADVANCEDINTERACTIONSYSTEM_API UInteractionStatics : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @param HitResult Hit result from whatever was used to gather target data.
	 * @param GameplayTag Optionally allows you to send a GameplayTag through the interact.
	 * @return 
	 */
	UFUNCTION(BlueprintPure)
	static FInteractionOption MakeInteractOptionFromHitResult(const FHitResult& HitResult, FGameplayTag GameplayTag);

	/**
	 * 
	 * @param SceneComponent Scene Component to trace from
	 * @param Range Range of the trace.
	 * @param Start Start of the trace.
	 * @param End End of the trace.
	 */
	UFUNCTION(BlueprintPure)
	static void ForwardTraceHelperFromComponent(const USceneComponent* SceneComponent, float Range, FVector& Start, FVector &End);

	/**
	 * 
	 * @param Actor Actor to trace from.
	 * @param Range Range of the trace.
	 * @param Start Start of the trace.
	 * @param End End of the trace.
	 */
	UFUNCTION(BlueprintPure)
	static void ForwardTraceHelperFromActor(const AActor* Actor, float Range, FVector& Start, FVector &End);

	
	UFUNCTION(BlueprintPure)
	static bool IsValidInteractionQuery(const FInteractionQuery& Query);
};

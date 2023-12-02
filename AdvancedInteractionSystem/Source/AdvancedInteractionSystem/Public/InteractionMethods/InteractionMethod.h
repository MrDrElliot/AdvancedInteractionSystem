// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Object.h"
#include "InteractionMethod.generated.h"



struct FGameplayTag;
class UInteractionComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionComplete);

struct FInteractionOption;
struct FInteractionQuery;
/**
 * Base class for obtaining interaction target data. Abstract, as this class should not be instantiated.
 */
UCLASS(Blueprintable, Abstract, DefaultToInstanced, meta = (ShowWorldContextPin))
class ADVANCEDINTERACTIONSYSTEM_API UInteractionMethod : public UObject
{
	GENERATED_BODY()
	
public:


	UPROPERTY(BlueprintAssignable)
	FOnInteractionComplete OnInteractionComplete;

	UPROPERTY()
	TWeakObjectPtr<UInteractionComponent> InteractionComponent;

	/** Helper function for getting a world context object if desired */
	UFUNCTION(BlueprintPure)
	UWorld* GetWorldContext() const;


	/**
	 * @param Range This is optional, allows you to pass in a float for any trace information you might want.
	 * @param Accuracy Accuracy for traces.
	 * @param TraceComplex Trace Complex Geometry
	 * @param DebugMode Debug Mode
	 * @param InteractionTag GameplayTag for the interaction.
	 * @param TraceChannel Trace Channel
	 * @param InteractionQuery This is the query of who and what is asking to perform the trace result.
	 * @return Returns information about the target.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "TargetData")
	TArray<FInteractionOption> GetTargetData(float Range, float Accuracy, bool TraceComplex, EDrawDebugTrace::Type DebugMode, FGameplayTag InteractionTag, ETraceTypeQuery TraceChannel, UPARAM(ref)FInteractionQuery &InteractionQuery);

	/** Helper function to get the calling Interaction Component */
	UFUNCTION(BlueprintPure, Category = "TargetData")
	UInteractionComponent* GetCallingComponent() const;
};

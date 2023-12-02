// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractionOption.generated.h"



class UInteractionMethod;
class IInteractionTarget;


UENUM(BlueprintType)
enum EInteractionReplicationMethod : uint8
{
	Local,
	Server,
	NetMulticast,
};

USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

public:

	FInteractionOption()
		:InteractableTarget(nullptr)
		,InteractionMethod(nullptr)
	{}
	
	/** The interactable target */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> InteractableTarget;

	UPROPERTY(BlueprintReadWrite)
	FHitResult HitResult;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag GameplayTag;

	/** Simple text the interaction might return */
	UPROPERTY(BlueprintReadWrite)
	FText Text;

	/** Simple sub-text the interaction might return */
	UPROPERTY(BlueprintReadWrite)
	FText SubText;

	/** Method used to obtain interaction target data */
	UPROPERTY(BlueprintReadWrite)
	TSoftClassPtr<UInteractionMethod> InteractionMethod;

	/** Optional object to pass through the interaction. */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> OptionalObject1;

	/** Optional object to pass through the interaction. */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> OptionalObject2;


	FORCEINLINE AActor* GetInteractableTarget() const;

	
};

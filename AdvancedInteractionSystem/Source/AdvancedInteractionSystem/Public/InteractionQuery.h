// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractionQuery.generated.h"
struct FGameplayTag;

USTRUCT(BlueprintType)
struct FInteractionQuery
{
	GENERATED_BODY()

public:
	
	/** Constructors **/
	FInteractionQuery()
	{
	}
	
	/** Actor requesting interaction, should be valid */
	UPROPERTY(BLueprintReadWrite)
	TWeakObjectPtr<AActor> RequestingActor;

	/** Controller requesting interaction, may be valid */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AController> RequestingController;

	/** Optional Camera Manager class, often null, not for use with AI */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<APlayerCameraManager> CameraManager;

	/** Optional UObject to pass through the interaction, often null */
	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<UObject> OptionalObjectData;

	FORCEINLINE bool IsValid() const
	{
		return RequestingActor.IsValid();
	}
	
};

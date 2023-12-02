// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "InteractionQuery.h"
#include "UObject/Interface.h"
#include "InteractionTarget.generated.h"


UINTERFACE()
class UInteractionTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interaction interface used in the plugin.
 */
class ADVANCEDINTERACTIONSYSTEM_API IInteractionTarget
{
	GENERATED_BODY()

public:
	/**
	 * 
	 * @param InteractionQuery Who and what started this interaction.
	 * @param InteractionOption FInteractionOption created from the InteractionMethod class.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractedWith(FInteractionQuery InteractionQuery, FInteractionOption InteractionOption);
	
};

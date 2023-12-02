// Copyright 2023 TheOtherSideGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InteractionTypes.generated.h"

USTRUCT(BlueprintType)
struct FDistanceBasedComponents
{
	GENERATED_BODY()

	FDistanceBasedComponents(const FHitResult& InHit, const float InDistance)
	{
		if(!InHit.GetActor()) return;

		Hit = InHit;
		Distance = InDistance;
	}
	
	FDistanceBasedComponents()
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult Hit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;

	FORCEINLINE bool operator <(const FDistanceBasedComponents& Other) const
	{
		return Distance < Other.Distance;
	}
	
};

// Copyright 2023 TheOtherSideGame. All Rights Reserved.


#include "InteractionMethods/InteractMethod_ClosestToCenter.h"
#include "GameplayTagContainer.h"
#include "InteractionOption.h"
#include "InteractionQuery.h"
#include "Components/InteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Statics/InteractionStatics.h"
#include "Types/InteractionTypes.h"

TArray<FInteractionOption> UInteractMethod_ClosestToCenter::GetTargetData_Implementation(float Range, float Accuracy, bool TraceComplex, EDrawDebugTrace::Type DebugMode, FGameplayTag InteractionTag, ETraceTypeQuery TraceChannel, FInteractionQuery& InteractionQuery)
{
	if(InteractionQuery.IsValid())
	{
		DrawDebugType = DebugMode;
		InteractRange = Range;
		TArray<FHitResult> HitResultMulti;
		
		FVector Start = InteractionQuery.CameraManager->GetCameraLocation();
		FVector End = Start + InteractionQuery.CameraManager->GetCameraRotation().Vector() * ((Range) + FVector::Distance(Start, InteractionQuery.RequestingActor->GetActorLocation()));
		TArray<AActor*> IgnoreActors = { GetCallingComponent()->GetOwner() };

		UKismetSystemLibrary::SphereTraceMulti(this, Start, End, Accuracy , TraceChannel, TraceComplex, IgnoreActors, DebugMode, HitResultMulti,true);

		FVector SphereEnd = InteractionQuery.RequestingActor->GetActorLocation();
		FVector SphereStart = (End- SphereEnd).GetSafeNormal() * Range / 2;
		SphereStart += SphereEnd;

		FCollisionQueryParams Params("InteractTrace", SCENE_QUERY_STAT_ONLY(KismetTraceUtils), TraceComplex);
		Params.bReturnFaceIndex = false;

		ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

		GetWorld()->SweepMultiByChannel(HitResultMulti, SphereStart, SphereEnd, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(Range / 1.5f));

		/*if(DebugMode > 0)
		{
			FColor DrawColor = HitResultMulti.Num() <= 0 ? FColor::Red : FColor::Green;

			FVector const TraceVec = SphereEnd - SphereStart;
			float const Dist = TraceVec.Size();

			FVector const Center = SphereStart + TraceVec * 0.5f;
			float const HalfHeight = (Dist * 0.5f) + Range / 1.5f;

			FQuat const CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
			DrawDebugCapsule(GetWorld(), Center, HalfHeight, Range / 1.5f, CapsuleRot, DrawColor, false, 2.f, 0);
		}*/

		
		if(HitResultMulti.Num() > 0)
		{
			/* If there is only one interact, then there is no need to do all the extra math. */
			if(HitResultMulti.Num() == 1)
			{
				if(!(HitResultMulti[0].GetActor()))
				{
					return Super::GetTargetData_Implementation(Range, Accuracy, TraceComplex, DebugMode, InteractionTag, TraceChannel, InteractionQuery);
				}
				
				TArray<FInteractionOption> InteractionOptions;
				InteractionOptions.Add(UInteractionStatics::MakeInteractOptionFromHitResult(HitResultMulti[0], InteractionTag));
				return InteractionOptions;
			}
			
			TArray<FDistanceBasedComponents> DistanceBasedComponents;
		
			/* Get the closest actor to the players cross hair using a dot product */
			for(const FHitResult& Hit : HitResultMulti)
			{
				if(!Hit.HasValidHitObjectHandle()) continue;

				float ComponentDistance = GetComponentDistanceFromLine(Start, End, Hit.GetComponent(), Hit.BoneName);

				DistanceBasedComponents.Add(FDistanceBasedComponents(Hit, ComponentDistance));
				
			}
			
			DistanceBasedComponents.Sort();

			if(!DistanceBasedComponents.IsEmpty())
			{
				TArray<FInteractionOption> InteractionOptions;
				InteractionOptions.Add(UInteractionStatics::MakeInteractOptionFromHitResult(DistanceBasedComponents[0].Hit, InteractionTag));
				return InteractionOptions;
			}
			
		}
	}
	return Super::GetTargetData_Implementation(Range, Accuracy, TraceComplex, DebugMode, InteractionTag, TraceChannel, InteractionQuery);
}

float UInteractMethod_ClosestToCenter::GetComponentDistanceFromLine(const FVector& Start, const FVector& End, const UPrimitiveComponent* Component, const FName HitBone) const
{
	float MinDistance = UE_BIG_NUMBER;

	FVector DebugLineStart;
	FVector DebugLineEnd;
	
	for (const float X : TArray{0.95f, 0.865f, 0.7825f, 0.7f,})
	{
		FVector LinePoint = FMath::Lerp(Start, End, X);
		FVector ClosestPoint;

		Component->GetClosestPointOnCollision(LinePoint, ClosestPoint, HitBone);

		FVector PointOnLine = FMath::ClosestPointOnSegment(ClosestPoint, Start, End);

		const float DistanceFromLine = FVector::Distance(ClosestPoint, PointOnLine);

		if(DistanceFromLine < MinDistance)
		{
			MinDistance = DistanceFromLine;
			DebugLineStart = ClosestPoint;
			DebugLineEnd = PointOnLine;
		}

		if(DrawDebugType > 0)
		{
			DrawDebugPoint(GetWorld(), LinePoint, 32, FColor::Blue, false, 0.f);
		}
	}

	if(DrawDebugType)
	{
		const FColor DrawColor = FLinearColor::LerpUsingHSV(FLinearColor::Green, FLinearColor::Red, MinDistance / InteractRange).ToFColor(true);
		DrawDebugLine(GetWorld(), DebugLineStart, DebugLineEnd, DrawColor, false, 2.f, 0, 0.25f);

		DrawDebugString(GetWorld(), DebugLineStart, FString::Printf(TEXT("Distance: %d"), static_cast<int32>(MinDistance)), nullptr, DrawColor, 2.f, true);
	}

	return MinDistance;
}

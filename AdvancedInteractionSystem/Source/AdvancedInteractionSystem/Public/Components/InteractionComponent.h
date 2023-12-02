// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTraceHit, FInteractionOption, OutOption);



struct FInteractionQuery;
class UAsync_TryInteraction;
enum EInteractionReplicationMethod : uint8;
class UInteractionMethod;
struct FInteractionOption;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=("Sockets", "ComponentReplication", "Activation", "Cooking", "ComponentTick", "AssetUserData", "Replication", "Collision"))
class ADVANCEDINTERACTIONSYSTEM_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractionComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;
	
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
	
	AController* GetControllerFromOwner() const;
	void PersistentInteraction(UInteractionMethod* InteractionMethod);


	/**
	 * 
	 * @param InteractionMethod User created class allowing customized data on how to gather target data.
	 * @param Range Optional interaction range to pass through to the InteractionMethod class.
	 * @param Accuracy Optional Accuracy Input to be used.
	 * @param TraceComplex Trace Complex Geomtry
	 * @param DebugMode Debug Mode
	 * @param InteractionTag FGamplayTag for interaction.
	 * @param TraceChannel TraceChannel
	 * @param ReplicationMethod Replication Method for RPC's.
	 * @param OutOption Passthrough of the FInteractionOption created from this function.
	 */
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "OutOption", AdvancedDisplay = "1"))
	void TryInteraction(TSubclassOf<UInteractionMethod> InteractionMethod, float Range, float Accuracy, bool TraceComplex, EDrawDebugTrace::Type DebugMode, FGameplayTag InteractionTag, ETraceTypeQuery TraceChannel, EInteractionReplicationMethod ReplicationMethod, FInteractionOption& OutOption);

	UFUNCTION(Server, Reliable)
	void Server_TryInteraction(FInteractionQuery InteractionQuery, FInteractionOption InteractionOption, EInteractionReplicationMethod ReplicationMethod);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TryInteraction(FInteractionQuery InteractionQuery,FInteractionOption InteractionOption);


	UFUNCTION(BlueprintPure)
	FInteractionOption GetPersistentInteractionOption() { return PersistentInteractionOption; }

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PersistentTracing")
	bool bUsePersistentTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	TSubclassOf<UInteractionMethod> PersistentInteractMethodClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	float PersistentTraceRange = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	float PersistentAccuracy = 25.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	bool bPersistentTraceComplex = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	TEnumAsByte<EDrawDebugTrace::Type> PersistentDrawDebug;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	FGameplayTag PersistentGameplayTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bUsePersistentTrace", EditConditionHides), Category = "PersistentTracing")
	TEnumAsByte<ETraceTypeQuery> PersistentTraceTypeQuery = TraceTypeQuery1;
	
	UPROPERTY()
	FInteractionOption PersistentInteractionOption;
	
	UPROPERTY()
	TObjectPtr<UInteractionMethod> PersistentInteractionMethod;

	UPROPERTY(BlueprintAssignable)
	FOnTraceHit OnPersistentTrace;

	/** Local Only */
	UPROPERTY(BlueprintAssignable)
	FOnTraceHit OnInteractTraceHit;
	
	
};

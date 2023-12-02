// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionOption.h"

AActor* FInteractionOption::GetInteractableTarget() const
{
	if(InteractableTarget.IsValid())
	{
		return InteractableTarget.Get();
	}
	return nullptr;
}

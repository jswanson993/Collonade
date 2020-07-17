// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CollonadeAICharacterController.generated.h"

/**
 * 
 */
UCLASS()
class COLLONADE_API ACollonadeAICharacterController : public AAIController
{
	GENERATED_BODY()

protected:
	int32 numberOfActions;
	FString actionType;


private:
	bool blocked;
	bool readyToPerform;
	bool approachingTarget;
	bool actionsFinished;
	float currTime;
	class ACollonadeCharacterBase* controlledCharacter;

public:
	ACollonadeAICharacterController();

	void OnAttackAction(FString actionType, int32 numOfAttacks);

	void OnBonusAction();

	UFUNCTION(BlueprintCallable)
		void BeginActionWindow();
	UFUNCTION(BlueprintCallable)
		void EndActionWindow();
	UFUNCTION(BlueprintCallable)
	void FinishedCurrentAction();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void MoveCharacterToTarget();

	UFUNCTION(BlueprintCallable)
	void MoveCharacterBack();

private:
	void PerformAction();
	float CalculateDamage();
	FVector lineupLocation;
};
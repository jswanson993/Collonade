// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CollonadePlayerCombatController.generated.h"


class ACollonadeCharacterBase;


/**
 * 
 */
UCLASS()
class COLLONADE_API ACollonadePlayerCombatController : public APlayerController
{
	GENERATED_BODY()

	public:
		ACollonadePlayerCombatController();

		// Called to bind functionality to input
		virtual void SetupInputComponent();

		UFUNCTION(BlueprintNativeEvent)
		void AttackWindowStart();
		UFUNCTION(BlueprintNativeEvent)
		void AttackWindowEnd();

		UFUNCTION()
		void OnCharacterDied(ACollonadeCharacterBase* killedCharacter);

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "Characters")
			ACollonadeCharacterBase* selectedCharacter;


		UPROPERTY(BlueprintReadOnly, Category = "Attack Window")
			bool isAttackWindow;

		bool attacking;

		FVector cameraOrigin;

		class ACollonadeCombatPlayerView* controlledPawn;

		UFUNCTION()
			void BeginTargetSelection();

		virtual void BeginPlay() override;

	private:

		UPROPERTY(EditAnywhere, Category = "Characters")
			TArray<ACollonadeCharacterBase*> party;
		UPROPERTY(EditAnywhere, Category = "Enemies")
			TArray<ACollonadeCharacterBase*> enemies;

		bool targetSelect;

		int targetSpace;

		FString nextAction;
		
		/**Tuple order: attacking character, target, action type*/
		TQueue < TTuple < ACollonadeCharacterBase*, ACollonadeCharacterBase*, FString >> actionQueue;

		TArray<ACollonadeCharacterBase*> targetGroup;

		/** Selects the character being used by the player */
		void SelectCharacter(int characterID);

		/** Selects which enemy is going to be attacked*/
		void SelectTarget(FString direction);

		/** Starts a light attack from the selected character*/
		void Action(FString actionType);


		
};

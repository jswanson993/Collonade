// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollonadeCombatPlayerView.generated.h"

class ACollonadeCharacterBase;

UCLASS()
class COLLONADE_API ACollonadeCombatPlayerView : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollonadeCombatPlayerView();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent* camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/** Gets the party array */
	UFUNCTION(BlueprintGetter)
		TArray<ACollonadeCharacterBase*> getParty();

	/** Gets the enemy array */
	UFUNCTION(BlueprintGetter)
		TArray<ACollonadeCharacterBase*> getEnemies();

	void SetTarget(ACollonadeCharacterBase* target);

	void StopTargeting();


private:


	UPROPERTY(EditAnywhere)
		TArray<ACollonadeCharacterBase*> party;
	UPROPERTY(EditAnywhere)
		TArray<ACollonadeCharacterBase*> enemies;

	ACollonadeCharacterBase* character;

	ACollonadeCharacterBase* currentTarget;

	void Highlight();

};

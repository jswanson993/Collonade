// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollonadeHealthAndEnergyComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilled, ACollonadeCharacterBase*, killedCharacter);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLLONADE_API UCollonadeHealthAndEnergyComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCollonadeHealthAndEnergyComp();

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers");
	FOnKilled killed;


	UFUNCTION(BlueprintCallable)
		void SpendEnergy(float energy);

	UFUNCTION(BlueprintCallable)
		void RegainEnergy(float energy);


	/*
	GETTERS AND SETTERS
	
	
	*/

	UFUNCTION(BlueprintGetter)
		float GetEnergyConsumed();
	UFUNCTION(BlueprintGetter)
		float GetDamageTaken();
	UFUNCTION(BlueprintGetter)
		float GetMaxHealth();
	UFUNCTION(BlueprintGetter)
		float GetEnergyRecoveryRate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter = "GetMaxHealth", Category = "Health and Energy")
		float maxHealth;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = "GetDamageTaken", Category = "Health and Energy")
		float damageTaken;

	UPROPERTY(BlueprintReadOnly, BlueprintGetter = "GetEnergyConsumed", Category = "Health and Energy")
		float energyConsumed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintGetter = "GetEnergyRecoveryRate", Category = "Health and Energy")
		float energyRecoveryRate;

	UFUNCTION()
		void TakeDamage(AActor* damagedActor, float damage, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser);

	

};

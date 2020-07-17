// Fill out your copyright notice in the Description page of Project Settings.


#include "CollonadeHealthAndEnergyComp.h"
#include "../CollonadeCharacterBase.h"




// Sets default values for this component's properties
UCollonadeHealthAndEnergyComp::UCollonadeHealthAndEnergyComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	maxHealth = 8;
	damageTaken = 0;
	energyConsumed = 0;
	energyRecoveryRate = .5;
}


// Called when the game starts
void UCollonadeHealthAndEnergyComp::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UCollonadeHealthAndEnergyComp::TakeDamage);
	}
	
}

void UCollonadeHealthAndEnergyComp::TakeDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* instigatedBy, AActor* damageCauser) {
	if (damage < 0) {
		return;
	}

	damageTaken = FMath::Clamp(damageTaken + damage, 0.0f, maxHealth);

	if (damageTaken >= maxHealth) {
		killed.Broadcast(Cast<ACollonadeCharacterBase>(this->GetOwner()));
	}

}

void UCollonadeHealthAndEnergyComp::SpendEnergy(float energy) {
	energyConsumed = FMath::Clamp(energyConsumed + energy, 0.0f, maxHealth);
}

void UCollonadeHealthAndEnergyComp::RegainEnergy(float energy) {
	energyConsumed = FMath::Clamp(energyConsumed - energy, 0.0f, maxHealth);
}


/**
* GETTERS AND SETTERS
**/


float UCollonadeHealthAndEnergyComp::GetEnergyRecoveryRate() {
	return energyRecoveryRate;
}

float UCollonadeHealthAndEnergyComp::GetEnergyConsumed() {
	return energyConsumed;
}

float UCollonadeHealthAndEnergyComp::GetDamageTaken() {
	return damageTaken;
}

float UCollonadeHealthAndEnergyComp::GetMaxHealth() {
	return maxHealth;
}




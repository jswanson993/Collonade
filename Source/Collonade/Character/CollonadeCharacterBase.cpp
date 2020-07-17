// Fill out your copyright notice in the Description page of Project Settings.


#include "CollonadeCharacterBase.h"
#include "./Health/CollonadeHealthAndEnergyComp.h"

// Sets default values
ACollonadeCharacterBase::ACollonadeCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	healthComp = CreateDefaultSubobject<UCollonadeHealthAndEnergyComp>(TEXT("Health And Energy Comp"));
	
}


// Called when the game starts or when spawned
void ACollonadeCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//Listen for character death
	healthComp->killed.AddDynamic(this, &ACollonadeCharacterBase::OnDeath);
}

// Called every frame
void ACollonadeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the character has used some of there energy they cannot act again until it depletes
	if (this->EnergyUsed()) {
		canAct = false;
		if(healthComp->GetEnergyConsumed() + healthComp->GetDamageTaken() >= healthComp->GetMaxHealth())
		//stops energy from replenishing if neccessary
		if (!pauseEnergy) {
			healthComp->RegainEnergy(healthComp->GetEnergyRecoveryRate() * DeltaTime);
		}
	} else {
		canAct = true;
	}

}

// Called to bind functionality to input
void ACollonadeCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACollonadeCharacterBase::Attack(float damageDelt) {
	//Logic for when character attacks
	//Check if any character abilities affect attack

	/*
	USING FOR TESTING DELETE CODE WHEN DONE
	
	*/

	healthComp->SpendEnergy(5);
	
}


void ACollonadeCharacterBase::OnDeath_Implementation(ACollonadeCharacterBase* killedCharacter) {
	//Logic for what happens when the character dies
	//Temporary for testing
	if (this == killedCharacter) {
		this->SetActorHiddenInGame(true);
	}
}

//ACollonadeWeapon* ACollonadeCharacterBase::GetEquipedWeapon() {
//	return equipedWeapon;
//}

//ACollonadeWeapon* ACollonadeCharacterBase::SetEquipedWeaopn(ACollonadeWeapon* weapon) {
//	equipedWeapon = weapon;
	
//	return equipedWeapon;
//}

bool ACollonadeCharacterBase::getCanAct() {
	return canAct;
}

bool ACollonadeCharacterBase::setCanAct(bool newCanAct) {
	canAct = newCanAct;
	return canAct;
}

bool ACollonadeCharacterBase::EnergyUsed() {
	float energy = healthComp->GetEnergyConsumed();

	return (energy > 0);
}

bool ACollonadeCharacterBase::GetVulnerable() {
	return vulnerable;
}

ACollonadeCharacterBase* ACollonadeCharacterBase::GetTarget() {
	return target;
}

ACollonadeCharacterBase* ACollonadeCharacterBase::SetTarget(ACollonadeCharacterBase* newTarget) {
	target = newTarget;
	return target;
}

bool ACollonadeCharacterBase::GetBlocked() {
	return blocked;
}

bool ACollonadeCharacterBase::SetBlocked(bool newBlock) {
	blocked = newBlock;
	return blocked;
}


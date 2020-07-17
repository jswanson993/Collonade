// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CollonadeCharacterBase.generated.h"

UCLASS()
class COLLONADE_API ACollonadeCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACollonadeCharacterBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character info")
		FName name;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//	class ACollonadeWeapon* equipedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action");
	bool canAct;

	UPROPERTY(VisibleAnywhere, Category = "Action");
	ACollonadeCharacterBase* target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action")
		bool blocked;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
		bool vulnerable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
		class UCollonadeHealthAndEnergyComp* healthComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Checks if character can attack and performs attack calculations*/
	UFUNCTION(BlueprintCallable)
		void Attack(float damageDelt);

	/** Gets called when the character dies*/
	UFUNCTION(BlueprintNativeEvent)
	void OnDeath(ACollonadeCharacterBase* killedCharacter);

	/** Gets the current equiped weaopn*/
	//ACollonadeWeapon* GetEquipedWeapon();

	/** Sets the current equiped weapon*/
	//ACollonadeWeapon* SetEquipedWeaopn(ACollonadeWeapon* weapon);

	/** Checks to see if the character can take an action*/
	UFUNCTION(BlueprintGetter)
		bool getCanAct();

	/** Sets the characters ability to take an action*/
	UFUNCTION(BlueprintSetter)
		bool setCanAct(bool newCanAct);

	UFUNCTION(BlueprintGetter)
	bool GetVulnerable();

	ACollonadeCharacterBase* GetTarget();

	ACollonadeCharacterBase* SetTarget(ACollonadeCharacterBase* newTarget);

	bool GetBlocked();

	bool SetBlocked(bool newBlock);

private:
	bool pauseEnergy;

	bool EnergyUsed();
};

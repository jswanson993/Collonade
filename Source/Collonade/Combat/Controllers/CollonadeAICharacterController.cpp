// Fill out your copyright notice in the Description page of Project Settings.

#include "CollonadeAICharacterController.h"
#include "Tasks/AITask_MoveTo.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../../Character/CollonadeCharacterBase.h"


ACollonadeAICharacterController::ACollonadeAICharacterController() {
	blocked = false;
	readyToPerform = false;
	currTime = 0;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(false);
}


void ACollonadeAICharacterController::BeginPlay() {
	Super::BeginPlay();
	controlledCharacter = Cast<ACollonadeCharacterBase>(this->GetPawn());
	lineupLocation = controlledCharacter->GetActorLocation();
}



void ACollonadeAICharacterController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//Check to see if the character is finished moving
	if (GetMoveStatus() == EPathFollowingStatus::Type::Moving) {
		approachingTarget = true;
	} else if(approachingTarget == true) {
		approachingTarget = false;

		//Checks to see if they're about to start their actions or finished them
		if (!actionsFinished) {
			readyToPerform = true;
		}
	}

	//Start series of actions
	if (readyToPerform) {
		readyToPerform = false;
		PerformAction();
	}
	
}


void ACollonadeAICharacterController::OnAttackAction(FString action, int32 numOfAttacks) {
	//Run pre attack setup
	//Run to enemy if necessary
	numberOfActions = numOfAttacks;
	this->actionType = action;

	PrimaryActorTick.SetTickFunctionEnable(true);

	if (!actionType.Equals(TEXT("Ranged"))) {
		MoveCharacterToTarget();
	} else {
		readyToPerform = true;
	}
}

void ACollonadeAICharacterController::FinishedCurrentAction() {

	if (controlledCharacter->GetTarget()->GetBlocked()) {
		blocked = true;
	}

	if (numberOfActions - 1 == 0) {
		MoveCharacterBack();
		float damageRecieved = CalculateDamage();
		controlledCharacter->GetTarget()->TakeDamage(damageRecieved, FDamageEvent::FDamageEvent(), Cast<AController>(this), controlledCharacter);
	} else {
		numberOfActions--;
		float damageRecieved = CalculateDamage();
		controlledCharacter->GetTarget()->TakeDamage(damageRecieved, FDamageEvent::FDamageEvent(), Cast<AController>(this), controlledCharacter);
		PerformAction();
	}

	
}

void ACollonadeAICharacterController::PerformAction() {
	//Run Attack animation

	/**
	*REMOVE AFTER ANIMATIONS GET ADDED
	**/
	FinishedCurrentAction();
}

float ACollonadeAICharacterController::CalculateDamage() {
	//if character successfuly blocked, No damage is applied
	if(blocked){
		return 0;
	}
	//Go through all possible combat effects and add them to the damage total
	int32 totalDamage = 0;
	if (actionType.Equals(TEXT("Weapon Action Attack 1"))) {
		totalDamage += 1; //placeholder until weapon and weapon ability get implemented
	} else if (actionType.Equals(TEXT("Weapon Action Attack 2"))) {
		totalDamage += 2; //placeholder until weapon and weapon ability get implemented
	}
	
	//TEST
	totalDamage = 10;
	return totalDamage;
}

void ACollonadeAICharacterController::MoveCharacterToTarget() {
	AAIController* myController = Cast<AAIController>(controlledCharacter->GetController());
	if (myController) {
		if (controlledCharacter->GetTarget()) {
			myController->MoveToActor(controlledCharacter->GetTarget(), 0.0);
		}
	}
}

void ACollonadeAICharacterController::MoveCharacterBack() {
	AAIController* myController = Cast<AAIController>(controlledCharacter->GetController());
	if (myController) {
		myController->MoveToLocation(lineupLocation, 0.0f);
	}
}

void ACollonadeAICharacterController::BeginActionWindow() {
	//Begin the action window
}

void ACollonadeAICharacterController::EndActionWindow() {
	//End the action window
}

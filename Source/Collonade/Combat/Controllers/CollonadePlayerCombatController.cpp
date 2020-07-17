#include "CollonadePlayerCombatController.h"
#include "../../Character/CollonadeCharacterBase.h"
#include "../CollonadeCombatPlayerView.h"
#include "Components/BoxComponent.h"
#include "Containers/Array.h"
#include "Engine/StaticMeshActor.h"
#include "CollonadeAICharacterController.h"
#include "../../Character/Health/CollonadeHealthAndEnergyComp.h"


namespace {

	/**Helper method for Select target that calculates the space to move the target to based off of user input*/
	int moveTarget(int targetSpace, int spaces, int arrayLen) {
		//Make sure that new new space is in the bounds of the array and return it
		if (targetSpace + spaces < arrayLen && targetSpace + spaces >= 0) {
			return targetSpace + spaces;
		} else {
			return targetSpace;
		}
	}

	/**Helper method for CharacterDied. Replaces dead characters with null values*/
	TArray<ACollonadeCharacterBase*> removeDeadCharacter(TArray<ACollonadeCharacterBase*> characterArray, ACollonadeCharacterBase* characterToRemove) {
		int index = characterArray.Find(characterToRemove);
		characterArray[index] = nullptr;
		return characterArray;
	}

	/** Helper method for Action. Checks to see if there are any conditions blocking the ablilty to perform an attack*/
	bool CheckIfCanAct(bool isAttackWindow, bool attacking, bool canAct) {
		//If there are any other attack conditions add it to this if statrement
		if (isAttackWindow || attacking || !canAct) {
			return false;
		} else {
			return true;
		}
	}
}


ACollonadePlayerCombatController::ACollonadePlayerCombatController() {

}

void ACollonadePlayerCombatController::BeginPlay() {
	Super::BeginPlay();

	controlledPawn = Cast<ACollonadeCombatPlayerView>(K2_GetPawn());
	if (controlledPawn) {
		party = controlledPawn->getParty();
		enemies = controlledPawn->getEnemies();
	}

	//Bind killed event to OnCharacterDied for each character
	for (ACollonadeCharacterBase* c : party) {
		UCollonadeHealthAndEnergyComp* healthComp = c->FindComponentByClass<UCollonadeHealthAndEnergyComp>();
		if (healthComp->IsValidLowLevel()) {
			healthComp->killed.AddDynamic(this, &ACollonadePlayerCombatController::OnCharacterDied);
		}
	}

	for (ACollonadeCharacterBase* c : enemies) {
		UCollonadeHealthAndEnergyComp* healthComp = c->FindComponentByClass<UCollonadeHealthAndEnergyComp>();
		if (healthComp->IsValidLowLevel()) {
			healthComp->killed.AddDynamic(this, &ACollonadePlayerCombatController::OnCharacterDied);
		}
	}

	//Temporary default for testing
	targetGroup = enemies;
	targetSpace = 0;
}


void ACollonadePlayerCombatController::SetupInputComponent(){
	Super::SetupInputComponent();

	//Create delegates to use for binding with function parameters
	DECLARE_DELEGATE_OneParam(FCharacterSelectDelegate, int);
	DECLARE_DELEGATE_OneParam(FActionDelegate, FString);
	DECLARE_DELEGATE_OneParam(FTargetSelectDelegate, FString);

	InputComponent->BindAction<FActionDelegate>("A", IE_Pressed, this, &ACollonadePlayerCombatController::Action, FString(TEXT("Weapon Action 1")));
	InputComponent->BindAction<FActionDelegate>("X", IE_Pressed, this, &ACollonadePlayerCombatController::Action, FString(TEXT("Weapon Action 2")));
	InputComponent->BindAction<FActionDelegate>("B", IE_Pressed, this, &ACollonadePlayerCombatController::Action, FString(TEXT("Character Ability")));
	InputComponent->BindAction<FActionDelegate>("Y", IE_Pressed, this, &ACollonadePlayerCombatController::Action, FString(TEXT("Something else")));
	
	InputComponent->BindAction<FCharacterSelectDelegate>("Outer Left", IE_Pressed, this, &ACollonadePlayerCombatController::SelectCharacter, 0);
	InputComponent->BindAction<FCharacterSelectDelegate>("Inner Left", IE_Pressed, this, &ACollonadePlayerCombatController::SelectCharacter, 1);
	InputComponent->BindAction<FCharacterSelectDelegate>("Inner Right", IE_Pressed, this, &ACollonadePlayerCombatController::SelectCharacter, 2);
	InputComponent->BindAction<FCharacterSelectDelegate>("Outer Right", IE_Pressed, this, &ACollonadePlayerCombatController::SelectCharacter, 3);


	InputComponent->BindAction<FTargetSelectDelegate>("LS Up", IE_Pressed, this, &ACollonadePlayerCombatController::SelectTarget, FString(TEXT("Up")));
	InputComponent->BindAction<FTargetSelectDelegate>("LS Down", IE_Pressed, this, &ACollonadePlayerCombatController::SelectTarget, FString(TEXT("Down")));
	InputComponent->BindAction<FTargetSelectDelegate>("LS Left", IE_Pressed, this, &ACollonadePlayerCombatController::SelectTarget, FString(TEXT("Left")));
	InputComponent->BindAction<FTargetSelectDelegate>("LS Right", IE_Pressed, this, &ACollonadePlayerCombatController::SelectTarget, FString(TEXT("Right")));

}

void ACollonadePlayerCombatController::OnCharacterDied(ACollonadeCharacterBase* killedCharacter) {
	//checks the two character arrays to see if they have the character that just dies replaces them with null values
	if (party.Contains(killedCharacter)) {
		party = removeDeadCharacter(party, killedCharacter);
	} else if (enemies.Contains(killedCharacter)) {
		enemies = removeDeadCharacter(enemies, killedCharacter);
	}
}

void ACollonadePlayerCombatController::BeginTargetSelection() {
	//Highlight targeted enemy and move camera above enemies
	controlledPawn->SetTarget(targetGroup[targetSpace]);
}

void ACollonadePlayerCombatController::AttackWindowStart_Implementation() {
	isAttackWindow = true;
}

void ACollonadePlayerCombatController::AttackWindowEnd_Implementation() {
	isAttackWindow = false;
}


void ACollonadePlayerCombatController::SelectCharacter(int characterID){
	
	if (party[characterID]) {
		//Make the character the current selected character
		selectedCharacter = party[characterID];
		targetSelect = false;
	}
}
	

void ACollonadePlayerCombatController::SelectTarget(FString direction) {
	if (targetSelect) {
		//gets the lenght of each row to compute what the bound of the 2d array would be
		int aWidth = FMath::Sqrt(float(targetGroup.Num()));
		//Move through the array depending on the direction of the input
		if (direction.Equals(TEXT("Up"))) {
			targetSpace = moveTarget(targetSpace, aWidth, targetGroup.Num());
		} else if (direction.Equals(TEXT("Down"))) {
			targetSpace = moveTarget(targetSpace, -aWidth, targetGroup.Num());
		} else if (direction.Equals(TEXT("Left"))) {
			//Don't allow moving to a new row by pressing left
			if (targetSpace % aWidth != 0) {
				targetSpace = moveTarget(targetSpace, -1, targetGroup.Num());
			}
		} else if (direction.Equals(TEXT("Right"))) { 
			//Don't allow moving to a new row by pressing right
			if ((targetSpace + 1) % aWidth != 0) {
				targetSpace = moveTarget(targetSpace, 1, targetGroup.Num());
			}
		}
		//update the view pawn to highlight the new target
		controlledPawn->SetTarget(targetGroup[targetSpace]);
	}
}

void ACollonadePlayerCombatController::Action(FString actionType) {
	if (selectedCharacter) {
		if (actionType.Equals("Weapon Action 1")) {
			//If player is in target selection mode, confirm the selected target
			if (targetSelect) {
				//confirm target selection
				if (targetGroup[targetSpace]) {
					ACollonadeCharacterBase* nextSelection = targetGroup[targetSpace];
					selectedCharacter->SetTarget(nextSelection);
					targetSelect = false;
					controlledPawn->StopTargeting();
					//IF there is already an attacking character add the action to the queue
					if (attacking) {
						TTuple < ACollonadeCharacterBase*, ACollonadeCharacterBase*, FString > newAction;
						newAction = TTuple<ACollonadeCharacterBase*, ACollonadeCharacterBase*, FString>(selectedCharacter, targetGroup[targetSpace], nextAction);
						actionQueue.Enqueue(newAction);
					} else {
						ACollonadeAICharacterController* aiController = Cast<ACollonadeAICharacterController>(selectedCharacter->GetController());
						if (aiController) {
							aiController->OnAttackAction(nextAction, 1);
						}
					}
				}
			} else {
				//logic for light attack
				if (CheckIfCanAct(isAttackWindow, attacking, Cast<ACollonadeCharacterBase>(selectedCharacter)->getCanAct())) {
					nextAction = actionType;
					targetSelect = true;
					
					//Enter target select
					this->BeginTargetSelection();
				} else if (attacking && selectedCharacter->getCanAct()) {
					nextAction = actionType;
					targetSelect = true;
					//Enter target select
					this->BeginTargetSelection();
				}
			}
		} else if (actionType.Equals("Weapon Action 2")) {
			targetSelect = true;
			//logic for heavy attack
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CollonadeCombatPlayerView.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Character/CollonadeCharacterBase.h"
#include "../Character/Health/CollonadeHealthAndEnergyComp.h"


// Sets default values
ACollonadeCombatPlayerView::ACollonadeCombatPlayerView() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void ACollonadeCombatPlayerView::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ACollonadeCombatPlayerView::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}



TArray<ACollonadeCharacterBase*> ACollonadeCombatPlayerView::getParty() {
	return party;
}

TArray< ACollonadeCharacterBase*> ACollonadeCombatPlayerView::getEnemies() {
	return enemies;
}

void ACollonadeCombatPlayerView::SetTarget(ACollonadeCharacterBase* target) {
	if (currentTarget) {
		this->StopTargeting();
	}
	currentTarget = target;
	this->Highlight();
}

void ACollonadeCombatPlayerView::StopTargeting() {
	Cast<ACollonadeCharacterBase>(currentTarget)->GetMesh()->SetRenderCustomDepth(false);
}

void ACollonadeCombatPlayerView::Highlight() {
	Cast<ACollonadeCharacterBase>(currentTarget)->GetMesh()->SetRenderCustomDepth(true);

}


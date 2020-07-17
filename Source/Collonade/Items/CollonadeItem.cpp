// Fill out your copyright notice in the Description page of Project Settings.


#include "CollonadeItem.h"

// Sets default values
ACollonadeItem::ACollonadeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollonadeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollonadeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "GameFramework/Character.h"


void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    if(CharacterRef)
    {
        Speed = CharacterRef->GetVelocity().Size();
    }
    else
    {
        CharacterRef = Cast<ACharacter>(GetOwningActor());
    }
}

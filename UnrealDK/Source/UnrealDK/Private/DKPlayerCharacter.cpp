
#include "DKPlayerCharacter.h"

// UnrealDK
#include "Items/DKItems.h"

ADKPlayerCharacter::ADKPlayerCharacter()
{
	OnActorBeginOverlap.AddDynamic(this, &ADKPlayerCharacter::ActorOnOverlap);
}

// Overlap actor
void ADKPlayerCharacter::ActorOnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ADKItems>(OtherActor))
	{
		// Do item overlap
		ADKItems* Item = Cast<ADKItems>(OtherActor);
		Item->PickUp();
	}
	// Do if rope overlap
}

void ADKPlayerCharacter::OnDamage()
{

}

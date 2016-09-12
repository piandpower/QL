//----------------------------------------
// Quarter Life
//
// MIT license
//
//  (\-/)
// (='.'=)
// (")-(")o
//----------------------------------------

#include "QL.h"
#include "QLPortalGunCompatibleActor.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortalGunCompatibleActor::AQLPortalGunCompatibleActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(200.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLPortalGunCompatibleActor::OnOverlapBeginForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::BeginPlay()
{
    Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortalGunCompatibleActor::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    // find if OtherActor is attached to or is being attached to by any actors
    FString info = OtherActor->GetName();

    //first, find parent
    AActor* parent = OtherActor->GetAttachParentActor();
    info += FString(TEXT("     parent = "));
    if (parent)
    {
        info += parent->GetName();
    }
    else
    {
        info += FString(TEXT("no parent"));
    }

    // second, find children
    TArray<AActor*> children;
    OtherActor->GetAttachedActors(children);
    info += FString(TEXT("     children = "));
    if (children.Num())
    {
        for (int32 i = 0; i < children.Num(); ++i)
        {
            info += children[i]->GetName() + " ";
        }
    }
    else
    {
        info += FString(TEXT("no children")) + " ";
    }

    QLUtility::QLSay(info);
}
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
#include "QLPortal.h"
#include "QLCharacter.h"
#include "QLWeaponPortalGun.h"

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortal::AQLPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(100.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
    StaticMeshComponent->SetStaticMesh(StaticMeshObj.Object);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    StaticMeshComponent->SetWorldScale3D(FVector(2.0f));
    float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -zDim));

    static ConstructorHelpers::FObjectFinder<UMaterial> BluePortalMaterialObj(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse"));
    static ConstructorHelpers::FObjectFinder<UMaterial> OrangePortalMaterialObj(TEXT("/Game/StarterContent/Materials/M_Basic_Floor"));
    if (BluePortalMaterialObj.Succeeded())
    {
        BluePortalMaterial = BluePortalMaterialObj.Object;
    }
    if (OrangePortalMaterialObj.Succeeded())
    {
        OrangePortalMaterial = OrangePortalMaterialObj.Object;
    }

    Spouse = nullptr;

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLPortal::OnOverlapBeginForActor);
    this->OnActorEndOverlap.AddDynamic(this, &AQLPortal::OnOverlapEndForActor);
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortal::BeginPlay()
{
	Super::BeginPlay();
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortal::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

//------------------------------------------------------------
// --- When a portal happens to overlap another actor upon
//     spawn, the overlap event seems to be triggered inside of SpawnActor() method
//     before the portal owner is manually set, which means at that time
//     PortalOwner is still nullptr !!!
//     To guarantee overlap is called after the actor is spawned, deferred
//     actor spawn method is used. See AQLWeaponPortalGun::CreatePortal().
// --- If an actor has 2 components, both having overlap event, then
//     upon creation by SpawnActor(), the 2 components will trigger
//     their overlap event against each other, resulting in 2
//     overlap calls, in name of the actor itself. So here we always only
//     specify physics/collision in RootComponent and set the children
//     components to NoCollision and Ignore.
// --- Fun fact: Apparently, character getting teleported precedes the attaching actors
//     triggering the overlap event.
// --- One critical issue in teleport implementation is repeated
//     teleport back and forth between 2 portals. In fact, this
//     directly causes the game to crash (stack overflow maybe?). The solution here
//     is to use a roll (i.e. register list) to keep track of the overlapping
//     actors. Specifically, when an actor BEGINS to overlap a portal, the portal adds
//     that actor to its spouse's roll. After the actor is teleported to the spouse
//     and ENDS to overlap the spouse, the spouse removes the actor from its own roll.
//     Teleport upon overlap is only permitted if the actor does not appear in the
//     overlapped portal's roll.
//------------------------------------------------------------
void AQLPortal::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    bool bCanTeleport = false;
    // if the other portal exists, teleport can be performed
    if (Spouse)
    {
        // if the actor does not in my roll, teleport can be performed
        if (!IsInMyRoll(OtherActor))
        {
            // if the overlapping actor is not a portal
            AQLPortal* AnotherPortal = Cast<AQLPortal>(OtherActor);
            if (!AnotherPortal)
            {
                // if OtherActor is character, teleport can be performed
                // this is not a type check, but identity check
                if (OtherActor == PortalOwner->GetWeaponOwner())
                {
                    bCanTeleport = true;
                }
                // if OtherActor is not character
                else
                {
                    // if OtherActor is not owned by character, teleport can be performed
                    // type check
                    AQLActor* QLOtherActor = Cast<AQLActor>(OtherActor);
                    if (QLOtherActor)
                    {
                        if (QLOtherActor->GetQLOwner() != PortalOwner->GetWeaponOwner())
                        {
                            bCanTeleport = true;
                        }
                    }
                }
            }
        }
    }

    // time to teleport!
    if (bCanTeleport)
    {
        // add actor to spouse's roll
        Spouse->AddToRoll(OtherActor);

        FVector NewLocation = Spouse->GetActorLocation();
        FRotator NewRotation = OtherActor->GetActorRotation();
        OtherActor->TeleportTo(NewLocation, NewRotation);
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::OnOverlapEndForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    // remove actor from my own roll if any
    RemoveFromRoll(OtherActor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetQLOwner(AActor* QLOwner)
{
    Super::SetQLOwner(QLOwner);
    this->PortalOwner = Cast<AQLWeaponPortalGun>(QLOwner);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::UnSetQLOwner()
{
    Super::SetQLOwner(nullptr);
    this->PortalOwner = nullptr;
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLWeaponPortalGun* AQLPortal::GetPortalOwner()
{
    return PortalOwner;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::SetSpouse(AQLPortal* Spouse)
{
    // no self-marriage
    if (this != Spouse)
    {
        this->Spouse = Spouse;
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
AQLPortal* AQLPortal::GetSpouse()
{
    return Spouse;
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::QueryPortal()
{
    if (Roll.Num() > 0)
    {
        for (auto It = Roll.CreateConstIterator(); It; ++It)
        {
            QLUtility::QLSayLong("--> roll = " + It.Value()->GetName() + " " + this->GetName());
        }
    }
    else
    {
        QLUtility::QLSayLong("--> empty roll " + this->GetName());
    }
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::AddToRoll(AActor* Actor)
{
    // if key exists, overwrite the value
    Roll.Add(Actor->GetName(), Actor);
}

//------------------------------------------------------------
//------------------------------------------------------------
void AQLPortal::RemoveFromRoll(AActor* Actor)
{
    // key does not have to exist
    AActor* Temp;
    Roll.RemoveAndCopyValue(Actor->GetName(), Temp);
}

//------------------------------------------------------------
//------------------------------------------------------------
bool AQLPortal::IsInMyRoll(AActor* Actor)
{
    return Roll.Contains(Actor->GetName());
}
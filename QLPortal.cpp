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
    Spouse = nullptr;

    // sound
    SoundList.Add("Teleport", CreateSoundComponent(RootComponent, TEXT("/Game/Sounds/teleport"), TEXT("SoundTeleportComp")));

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    BoxComponent->InitBoxExtent(FVector(20.0f, 100.0f, 100.0f));
    BoxComponent->SetSimulatePhysics(false);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
    StaticMeshComponent->SetStaticMesh(StaticMeshObj.Object);
    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    StaticMeshComponent->SetWorldScale3D(FVector(0.4f, 2.0f, 2.0f));
    float zDim = StaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -zDim));

    //SecondStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondStaticMeshComponent"));
    //const ConstructorHelpers::FObjectFinder<UStaticMesh> SecondStaticMeshObj(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
    //SecondStaticMeshComponent->SetStaticMesh(SecondStaticMeshObj.Object);
    //SecondStaticMeshComponent->SetSimulatePhysics(false);
    //SecondStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //SecondStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    //SecondStaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    //SecondStaticMeshComponent->SetWorldScale3D(FVector(0.4f, 0.4f, 2.0f));
    //zDim = SecondStaticMeshComponent->Bounds.BoxExtent.Z; // note: extent refers to half of the side
    //SecondStaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -zDim));
    //SecondStaticMeshComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    //DEBUG
    //static ConstructorHelpers::FObjectFinder<UMaterial> BluePortalMaterialObj(TEXT("/Game/Materials/Debug/QL_MatDebugType1"));
    //static ConstructorHelpers::FObjectFinder<UMaterial> OrangePortalMaterialObj(TEXT("/Game/Materials/Debug/QL_MatDebugType2"));
    //StaticMeshComponent->SetMaterial(0, BluePortalMaterialObj.Object);
    //SecondStaticMeshComponent->SetMaterial(0, OrangePortalMaterialObj.Object);

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

    // set up camera, texture at compile time
    PortalCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortalCamera"));
    PortalCamera->bCaptureEveryFrame = true;
    PortalCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
    PortalCamera->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
    PortalRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("PortalRenderTarget"));
    PortalRenderTarget->InitAutoFormat(1024, 1024);
    PortalRenderTarget->AddressX = TA_Wrap;
    PortalRenderTarget->AddressY = TA_Wrap;
    PortalCamera->TextureTarget = PortalRenderTarget;

    PortalCameraSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalCameraSphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> TempObj(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
    PortalCameraSphere->SetStaticMesh(TempObj.Object);
    PortalCameraSphere->SetSimulatePhysics(false);
    PortalCameraSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PortalCameraSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    PortalCameraSphere->AttachToComponent(PortalCamera, FAttachmentTransformRules::SnapToTargetIncludingScale);
    PortalCameraSphere->SetWorldScale3D(FVector(0.2f, 0.2f, 1.0f));
    PortalCameraSphere->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
    PortalCameraSphere->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    // built-in dynamic delegate
    this->OnActorBeginOverlap.AddDynamic(this, &AQLPortal::OnOverlapBeginForActor);
    this->OnActorEndOverlap.AddDynamic(this, &AQLPortal::OnOverlapEndForActor);

    static ConstructorHelpers::FObjectFinder<UTexture> PortalTextureObj(TEXT("/Game/Textures/T_Brick_Clay_Beveled_D"));

    PortalTexture = PortalTextureObj.Object;

    //PortalTexture = PortalRenderTarget;
}

//------------------------------------------------------------
// Called when the game starts or when spawned
//------------------------------------------------------------
void AQLPortal::BeginPlay()
{
    Super::BeginPlay();

    // set up camera, texture, material at runtime
    PortalCamera->UpdateContent();
    PortalDynamicMaterial = StaticMeshComponent->CreateDynamicMaterialInstance(0);
    //PortalDynamicMaterial = UMaterialInstanceDynamic::Create(BluePortalMaterial, NULL);
    PortalDynamicMaterial->SetTextureParameterValue("PortalTexture", PortalTexture);
    StaticMeshComponent->SetMaterial(0, PortalDynamicMaterial);
}

//------------------------------------------------------------
// Called every frame
//------------------------------------------------------------
void AQLPortal::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
    //if (PortalRenderTarget->TextureReference.IsInitialized())
    //{
    //    //PortalDynamicMaterial->SetTextureParameterValue("PortalTexture", PortalRenderTarget);
    //    StaticMeshComponent->SetMaterial(0, PortalDynamicMaterial);
    //}
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
// --- There is an annoying limitation: AActor only has velocity getter, not setter.
//------------------------------------------------------------
void AQLPortal::OnOverlapBeginForActor(AActor* OverlappedActor, AActor* OtherActor)
{
    bool bCanTeleport = false;
    AQLPortal* AnotherPortal = Cast<AQLPortal>(OtherActor);
    AQLCharacter* QLCharacter = Cast<AQLCharacter>(OtherActor);
    AQLActor* QLActor = Cast<AQLActor>(OtherActor);

    // if the other portal exists, teleport can be performed
    if (Spouse)
    {
        // if the actor is not currently appear in my roll, teleport can be performed
        if (!IsInMyRoll(OtherActor))
        {
            // if the overlapping actor is not a portal
            if (!AnotherPortal)
            {
                // if OtherActor is character, teleport can be performed
                if (QLCharacter)
                {
                    bCanTeleport = true;
                }
                else
                {
                    // if OtherActor is AQLActor, teleport can be performed
                    if (QLActor)
                    {
                        // if OtherActor is not owned by character, teleport can be performed
                        if (QLActor->GetQLOwner() != PortalOwner->GetWeaponOwner())
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

        // update actor phase space
        FVector NewDirection = Spouse->GetActorForwardVector();
        FVector Velocity = OtherActor->GetVelocity();
        float Speed = Velocity.Size();
        FVector NewVelocity = NewDirection * Speed;

        FVector NewLocation = Spouse->GetActorLocation();
        FRotator NewRotation = NewDirection.Rotation();
        FRotator NewRotationYawOnly = FRotator(0.0f, NewRotation.Yaw, 0.0f);
        // note: NewDirection.Rotation(); is eqvuivalent to UKismetMathLibrary::MakeRotFromX(NewDirection);
        // but the latter requires #include "Kismet/KismetMathLibrary.h"

        // teleport
        OtherActor->TeleportTo(NewLocation, NewRotationYawOnly);

        // if OtherActor is character, teleport can be performed
        if (QLCharacter)
        {
            // change velocity
            QLCharacter->GetMovementComponent()->Velocity = NewVelocity;

            // change controller direction
            QLCharacter->GetController()->SetControlRotation(NewRotation);
        }
        else
        {
            // if OtherActor is AQLActor, teleport can be performed
            if (QLActor)
            {
                // if OtherActor is not owned by character, teleport can be performed
                if (QLActor->GetQLOwner() != PortalOwner->GetWeaponOwner())
                {
                    // change velocity
                    // SetPhysicsLinearVelocity() can only be applied to the static mesh component
                    // but in gravity gun compatible actors the static mesh does not have physics/collision
                    // so instead AddImpulse() is applied here to the box component
                    // impulse (vector) = mass (scalar) x velocity change (vector)
                    // ps: AddImpulse(velocity change, NAME_None, false) does not seem to work as expectecd
                    FVector Impulse = QLActor->BoxComponent->GetMass() * (NewVelocity - Velocity);
                    QLActor->BoxComponent->AddImpulse(Impulse);
                }
            }
        }

        PlaySound("Teleport");
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
            QLUtility::QLSayLong("--> roll = " + It.Value()->GetName() + ". source = " + this->GetName());
        }
    }
    else
    {
        QLUtility::QLSayLong("--> empty roll. source = " + this->GetName());
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
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
#include "QLUtility.h"

namespace QLUtility
{
    //------------------------------------------------------------
    //------------------------------------------------------------
    void QLSay(const FString& string)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, string);
    }
}
// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettings.h"

#include "BlueprintableDeveloperSettingsManager.h"

#if WITH_EDITOR
#include "AssetRegistry/IAssetRegistry.h"
#include "ISettingsModule.h"
#include "FileHelpers.h"
#include "Editor.h"
#include "EditorAssetLibrary.h"
#endif


#if WITH_EDITOR
UBlueprintableDeveloperSettings::UBlueprintableDeveloperSettings()
{
	SectionData.ContainerName = TEXT("Editor");
	SectionData.CategoryName = TEXT("Blueprintable Developer Settings");
	SectionData.SectionName = TEXT("Base Settings");
	SectionData.DisplayName = FText::FromName(GetFName());
	SectionData.Description = FText::FromName(GetFName());
}

bool UBlueprintableDeveloperSettings::Modify(bool bAlwaysMarkDirty)
{
	return Super::Modify(bAlwaysMarkDirty);
}

bool UBlueprintableDeveloperSettings::Rename(const TCHAR* NewName, UObject* NewOuter, ERenameFlags InFlags)
{
	// Flags |= EBlueprintDeveloperSettingsFlags::Renaming;
	// const auto Result = Super::Rename(NewName, NewOuter, InFlags);
	//
	// return Result;
	return Super::Rename(NewName, NewOuter, InFlags);
}

void UBlueprintableDeveloperSettings::PostRename(UObject* OldOuter, const FName OldName)
{
	// UObject::PostRename(OldOuter, OldName);
	// Flags &= ~EBlueprintDeveloperSettingsFlags::Renaming;
	//
	// if (!UBlueprintableDeveloperSettingsManager::IsAppropriateObjectForSettings(this))
	// {
	// 	return;
	// }
	//
	// UnregisterSettings();
	// RegisterSettings();
	Super::PostRename(OldOuter, OldName);
}

void UBlueprintableDeveloperSettings::PostDuplicate(bool bDuplicateForPIE)
{
	UObject::PostDuplicate(bDuplicateForPIE);
	
	const auto* CDO = Cast<UBlueprintableDeveloperSettings>(UBlueprintableDeveloperSettings::StaticClass()->ClassDefaultObject);
	if (IsValid(CDO))
	{
		const auto ClassId = UBlueprintableDeveloperSettingsManager::FindClassIdBySectionData(CDO->SectionData);
		if (ClassId != INDEX_NONE)
		{
			UBlueprintableDeveloperSettingsManager::UnregisterSettings(ClassId);
		}
	}
}

void UBlueprintableDeveloperSettings::RegisterSettings()
{
	checkf(HasAnyFlags(RF_ClassDefaultObject), TEXT("Only CDO can register settings"));
	UBlueprintableDeveloperSettingsManager::RegisterSettings(GetClass());
}

void UBlueprintableDeveloperSettings::UnregisterSettings()
{
	checkf(HasAnyFlags(RF_ClassDefaultObject), TEXT("Only CDO can unregister settings"));
	UBlueprintableDeveloperSettingsManager::UnregisterSettings(GetClass());
}

void UBlueprintableDeveloperSettings::OnPackageMarkedDirty(UPackage* Package, bool bArg)
{
	// const auto ThisPackage = GetPackage();
	// if (Package != ThisPackage)
	// {
	// 	return;
	// }
	//
	// // We set dirty flag to false because we don't want to re-save blueprint if we change its fields through project / editor settings
	// if (Package->IsDirty() && Flags == EBlueprintDeveloperSettingsFlags::None)
	// {
	// 	Package->SetDirtyFlag(false);
	// }
}

void UBlueprintableDeveloperSettings::OnBlueprintPreCompile(UBlueprint* InBlueprint)
{
	// const auto bBlueprintValid = IsValid(InBlueprint);
	// const auto bGeneratedClassValid = bBlueprintValid && IsValid(InBlueprint->GeneratedClass) && InBlueprint->GeneratedClass == GetClass();
	// const auto bSettingsClass = bGeneratedClassValid && InBlueprint->GeneratedClass->IsChildOf(StaticClass());
	// const auto bDefaultClass = GetClass() == StaticClass();
	//
	// if (!bBlueprintValid || !bGeneratedClassValid || !bSettingsClass || bDefaultClass)
	// {
	// 	return;
	// }
	//
	// Flags |= EBlueprintDeveloperSettingsFlags::Compiling;
	//
	// auto* Blueprint = UBlueprint::GetBlueprintFromClass(GetClass());
	//
	// checkf(IsValid(Blueprint), TEXT("Blueprint is not valid. Make sure that you have called UBlueprintableDeveloperSettings::LoadBlueprints()"));
	// if (!Blueprint->OnChanged().IsBoundToObject(this))
	// {
	// 	Blueprint->OnChanged().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintChanged);
	// }
}

void UBlueprintableDeveloperSettings::OnBlueprintCompiled(UBlueprint* InBlueprint)
{
	// if (!(Flags & EBlueprintDeveloperSettingsFlags::Compiling))
	// {
	// 	return;
	// }
	//
	// Flags &= ~EBlueprintDeveloperSettingsFlags::Compiling;
}

void UBlueprintableDeveloperSettings::OnBlueprintChanged(UBlueprint* InBlueprint)
{
	// const auto* ThisBlueprint = UBlueprint::GetBlueprintFromClass(GetClass());
	// if (InBlueprint != ThisBlueprint)
	// {
	// 	return;
	// }
	//
	// Flags |= EBlueprintDeveloperSettingsFlags::Changing;
	//
	// ThisBlueprint->MarkPackageDirty();
	//
	// Flags &= ~EBlueprintDeveloperSettingsFlags::Changing;

}
#endif

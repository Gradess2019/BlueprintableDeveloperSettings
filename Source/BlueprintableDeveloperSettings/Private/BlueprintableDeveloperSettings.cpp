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
	ContainerName = TEXT("Editor");
	CategoryName = TEXT("Blueprintable Developer Settings");
	SectionName = TEXT("Base Settings");
	DisplayName = FText::FromName(GetFName());
	Description = FText::FromName(GetFName());
}

void UBlueprintableDeveloperSettings::PostCDOContruct()
{
	Super::PostCDOContruct();

	// const auto Class = GetClass();
	// const auto bSuperClass = Class == StaticClass();
	// if (bSuperClass)
	// {
	// 	return;
	// }

	// auto* ThisBlueprint = UBlueprint::GetBlueprintFromClass(GetClass());
	//
	// checkf(IsValid(ThisBlueprint), TEXT("ThisBlueprint is not valid"));
	// checkf(!ThisBlueprint->OnCompiled().IsBoundToObject(this), TEXT("%s->OnCompiled() is already bound to this object"), *ThisBlueprint->GetName());
	// ThisBlueprint->OnCompiled().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintCompiled);
	//
	// checkf(!FCoreUObjectDelegates::OnObjectsReplaced.IsBoundToObject(this), TEXT("FCoreUObjectDelegates::OnObjectsReplaced is already bound to this object"));
	// FCoreUObjectDelegates::OnObjectsReplaced.AddUObject(this, &UBlueprintableDeveloperSettings::OnObjectsReplaced);
	
	// if (!GEditor->OnBlueprintPreCompile().IsBoundToObject(this))
	// {
	// 	GEditor->OnBlueprintPreCompile().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintPreCompile);
	// }
	//
	// if (!GEditor->OnBlueprintCompiled().IsBoundToObject(this))
	// {
	// 	GEditor->OnBlueprintCompiled().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintCompiled);
	// }
	//
	// UPackage::PackageMarkedDirtyEvent.AddUObject(this, &UBlueprintableDeveloperSettings::OnPackageMarkedDirty);
	//
}

void UBlueprintableDeveloperSettings::BeginDestroy()
{
	// // FEditorDelegates::OnAssetsPreDelete
	// if (IsValid(this) && HasAnyFlags(RF_ClassDefaultObject))
	// {
	// 	// const auto ClassName = GetClass()->GetName();
	// 	// const auto bSkeletonClass = ClassName.StartsWith(TEXT("SKEL_"));
	// 	// const auto bReinstancedClass = ClassName.StartsWith(TEXT("REINST_"));
	// 	// if (!bSkeletonClass && !bReinstancedClass)
	// 	// {
	// 	// 	UnregisterSettings();
	// 	// }
	// }
	//
	UObject::BeginDestroy();
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

void UBlueprintableDeveloperSettings::LoadBlueprintSettings()
{
	// TArray<FAssetData> FoundSettingsAssets;
	// const auto& ParentClassName = StaticClass()->GetClassPathName();
	// UBlueprintableDeveloperSettingsLibrary::FindBlueprints({ParentClassName}, {}, FoundSettingsAssets);
	//
	// for (const auto& SettingsAsset : FoundSettingsAssets)
	// {
	// 	checkf(!SettingsAsset.IsAssetLoaded(), TEXT("Asset is already loaded"));
	// 	const auto& AssetPath = SettingsAsset.GetObjectPathString();
	// 	const auto LoadedSettingsBlueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(AssetPath));
	// 	const auto GeneratedClass = LoadedSettingsBlueprint->GeneratedClass;
	// 	const auto DefaultObject = Cast<UBlueprintableDeveloperSettings>(GeneratedClass->GetDefaultObject());
	// 	DefaultObject->Blueprint = LoadedSettingsBlueprint;
	// 	UE_LOG(LogTemp, Log, TEXT("Loaded settings blueprint: %s"), *AssetPath);
	// }
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

void UBlueprintableDeveloperSettings::OnObjectsReplaced(const TMap<UObject*, UObject*>& Tuples)
{
	// const auto* ReplacementPtr = Tuples.Find(this);
	//
	// if (!ReplacementPtr)
	// {
	// 	return;
	// }
	//
	// auto* NewObject = Cast<UBlueprintableDeveloperSettings>(*ReplacementPtr);
	// if (!IsValid(NewObject) || NewObject == this)
	// {
	// 	return;
	// }
	//
	// FCoreUObjectDelegates::OnObjectsReplaced.RemoveAll(this);
	// // UPackage::PackageMarkedDirtyEvent.RemoveAll(this);
	//
	// if (!UBlueprintableDeveloperSettingsManager::IsAppropriateObjectForSettings(NewObject))
	// {
	// 	return;
	// }

	// auto* ThisBlueprint = UBlueprint::GetBlueprintFromClass(GetClass());
	// if (IsValid(ThisBlueprint))
	// {
	// 	ThisBlueprint->OnChanged().RemoveAll(this);
	// 	ThisBlueprint->OnChanged().AddUObject(NewObject, &UBlueprintableDeveloperSettings::OnBlueprintChanged);
	// }

	// if (UBlueprintableDeveloperSettingsManager::IsSettingsRegistered(GetClass()))
	// {
	// 	UnregisterSettings();
	// }
	//
	// NewObject->RegisterSettings();
}

void UBlueprintableDeveloperSettings::OnPreGarbageCollectConditionalBeginDestroy()
{
	UE_LOG(LogTemp, Log, TEXT("OnPreGarbageCollectConditionalBeginDestroy"));
}

void UBlueprintableDeveloperSettings::OnFilesLoaded()
{
	// UBlueprintableDeveloperSettingsManager::LoadBlueprintSettings();
	
	// GEditor->GetTimerManager()->SetTimerForNextTick([&]
	// {
	// 	const auto AssetRegistry = IAssetRegistry::Get();
	// 	AssetRegistry->OnAssetAdded().AddUObject(this, &UBlueprintableDeveloperSettings::OnAssetAdded);
	// 	AssetRegistry->OnAssetRemoved().AddUObject(this, &UBlueprintableDeveloperSettings::OnAssetRemoved);
	// });
}

void UBlueprintableDeveloperSettings::OnAssetAdded(const FAssetData& AssetData)
{
	// if (!AssetData.IsAssetLoaded())
	// {
	// 	return;
	// }
	//
	// const auto AssetClass = AssetData.AssetClassPath;
	// if (AssetClass.GetAssetName() != TEXT("Blueprint"))
	// {
	// 	return;
	// }
	//
	// const auto AddedBlueprint = Cast<UBlueprint>(AssetData.GetAsset());
	// if (!IsValid(AddedBlueprint))
	// {
	// 	return;
	// }
	//
	// const auto GeneratedClass = AddedBlueprint->GeneratedClass;
	// if (!IsValid(GeneratedClass) || !GeneratedClass->IsChildOf(StaticClass()))
	// {
	// 	return;
	// }
	//
	// const auto Package = AssetData.GetPackage();
	// UEditorLoadingAndSavingUtils::SavePackages({ Package }, false);
	//
	// const auto DefaultObject = Cast<UBlueprintableDeveloperSettings>(GeneratedClass->GetDefaultObject());
	// DefaultObject->Blueprint = AddedBlueprint;
}

void UBlueprintableDeveloperSettings::OnAssetRemoved(const FAssetData& AssetData)
{
	// const auto DeletedAsset = Cast<UBlueprint>(AssetData.GetAsset());
	//
	// if (!IsValid(DeletedAsset))
	// {
	// 	return;
	// }
	//
	// const auto GeneratedClass = DeletedAsset->GeneratedClass;
	// if (!IsValid(GeneratedClass) || !GeneratedClass->IsChildOf(StaticClass()))
	// {
	// 	return;
	// }
	//
	// const auto bCreateIfNeeded = false;
	// const auto DefaultObject = Cast<UBlueprintableDeveloperSettings>(GeneratedClass->GetDefaultObject(bCreateIfNeeded));
	//
	// if (IsValid(DefaultObject))
	// {
	// 	DefaultObject->UnregisterSettings();
	// }
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

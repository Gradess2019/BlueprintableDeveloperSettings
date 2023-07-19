// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettings.h"

#include "BlueprintableDeveloperSettingsLibrary.h"

#if WITH_EDITOR
#include "AssetRegistry/IAssetRegistry.h"
#include "ISettingsModule.h"
#include "FileHelpers.h"
#include "Editor.h"
#include "EditorAssetLibrary.h"
#endif


FName UBlueprintableDeveloperSettings::GetContainerName_Implementation() const
{
	return TEXT("Editor");
}

FName UBlueprintableDeveloperSettings::GetCategoryName_Implementation() const
{
	return TEXT("Blueprintable Developer Settings");
}

FName UBlueprintableDeveloperSettings::GetSectionName_Implementation() const
{
	return TEXT("Base Settings");
}

FText UBlueprintableDeveloperSettings::GetDisplayName_Implementation() const
{
	const auto Class = GetClass();
	auto ClassName = Class->GetName();
	ClassName.RemoveFromEnd(TEXT("_C"));
	
	return FText::FromString(ClassName);
}

FText UBlueprintableDeveloperSettings::GetDescription_Implementation() const
{
	return NSLOCTEXT("BlueprintableDeveloperSettings", "BlueprintableDeveloperSettingsDescription", "Default description");
}

#if WITH_EDITOR
void UBlueprintableDeveloperSettings::PostCDOContruct()
{
	Super::PostCDOContruct();

	const auto AssetRegistry = IAssetRegistry::Get();
	const auto Class = GetClass();
	const auto bSuperClass = Class == StaticClass();
	if (bSuperClass)
	{
		AssetRegistry->OnFilesLoaded().AddUObject(this, &UBlueprintableDeveloperSettings::LoadBlueprintSettings);
		AssetRegistry->OnAssetRemoved().AddUObject(this, &UBlueprintableDeveloperSettings::OnAssetRemoved);
		return;
	}

	if (!IsAppropriateObjectForSettings(this))
	{
		return;
	}

	if (!GEditor->OnBlueprintPreCompile().IsBoundToObject(this))
	{
		GEditor->OnBlueprintPreCompile().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintPreCompile);
	}

	if (!GEditor->OnBlueprintCompiled().IsBoundToObject(this))
	{
		GEditor->OnBlueprintCompiled().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintCompiled);
	}
	
	UPackage::PackageMarkedDirtyEvent.AddUObject(this, &UBlueprintableDeveloperSettings::OnPackageMarkedDirty);

	FCoreUObjectDelegates::OnObjectsReplaced.AddUObject(this, &UBlueprintableDeveloperSettings::OnObjectsReplaced);
}

bool UBlueprintableDeveloperSettings::Modify(bool bAlwaysMarkDirty)
{
	return false;
}

bool UBlueprintableDeveloperSettings::Rename(const TCHAR* NewName, UObject* NewOuter, ERenameFlags InFlags)
{
	Flags |= EBlueprintDeveloperSettingsFlags::Renaming;
	const auto Result = Super::Rename(NewName, NewOuter, InFlags);
	
	return Result;
}

void UBlueprintableDeveloperSettings::PostRename(UObject* OldOuter, const FName OldName)
{
	UObject::PostRename(OldOuter, OldName);
	Flags &= ~EBlueprintDeveloperSettingsFlags::Renaming;

	if (!IsAppropriateObjectForSettings(this))
	{
		return;
	}
	
	UnregisterSettings();
	RegisterSettings();
}

void UBlueprintableDeveloperSettings::LoadBlueprintSettings()
{
	TArray<FAssetData> FoundSettingsAssets;
	const auto& ParentClassName = StaticClass()->GetClassPathName();
	UBlueprintableDeveloperSettingsLibrary::FindBlueprints({ParentClassName}, {}, FoundSettingsAssets);

	for (const auto& SettingsAsset : FoundSettingsAssets)
	{
		checkf(!SettingsAsset.IsAssetLoaded(), TEXT("Asset is already loaded"));
		const auto& AssetPath = SettingsAsset.GetObjectPathString();
		const auto LoadedSettingsBlueprint = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(AssetPath));
		const auto GeneratedClass = LoadedSettingsBlueprint->GeneratedClass;
		const auto DefaultObject = Cast<UBlueprintableDeveloperSettings>(GeneratedClass->GetDefaultObject());
		DefaultObject->Blueprint = LoadedSettingsBlueprint;
		UE_LOG(LogTemp, Log, TEXT("Loaded settings blueprint: %s"), *AssetPath);
	}
}

void UBlueprintableDeveloperSettings::RegisterSettings()
{
	checkf(HasAnyFlags(RF_ClassDefaultObject), TEXT("Only CDO can register settings"));

	auto& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	const auto& ContainerName = GetContainerName();
	const auto& CategoryName = GetCategoryName();
	const auto& SectionName = GetSectionName();
	const auto& DisplayName = GetDisplayName();
	const auto& Description = GetDescription();
	SettingsModule.RegisterSettings(ContainerName, CategoryName, SectionName, DisplayName, Description, this);
}

void UBlueprintableDeveloperSettings::UnregisterSettings()
{
	checkf(HasAnyFlags(RF_ClassDefaultObject), TEXT("Only CDO can unregister settings"));

	auto& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	const auto& ContainerName = GetContainerName();
	const auto& CategoryName = GetCategoryName();
	const auto& SectionName = GetSectionName();
	SettingsModule.UnregisterSettings(ContainerName, CategoryName, SectionName);
}

bool UBlueprintableDeveloperSettings::IsAppropriateObjectForSettings(UObject* Object) const
{
	checkf(IsValid(Object), TEXT("Object is not valid"));
	
	const auto Class = Object->GetClass();
	const auto& ClassName = Class->GetName();
	const auto bSettingsClass = Class->IsChildOf(StaticClass());
	const auto bCDO = Object->HasAnyFlags(RF_ClassDefaultObject);
	const auto bSkeletonClass = ClassName.StartsWith(TEXT("SKEL_"));
	const auto bReinstancedClass = ClassName.StartsWith(TEXT("REINST_"));
	const auto bSuperClass = Class == StaticClass();

	return bSettingsClass && bCDO && !bSkeletonClass && !bReinstancedClass && !bSuperClass;
}

void UBlueprintableDeveloperSettings::OnObjectsReplaced(const TMap<UObject*, UObject*>& Tuples)
{
	const auto NewObjectPtr = Tuples.Find(this);

	if (!NewObjectPtr)
	{
		return;
	}

	const auto NewObject = Cast<UBlueprintableDeveloperSettings>(*NewObjectPtr);
	if (!IsValid(NewObject) || NewObject == this)
	{
		return;
	}

	FCoreUObjectDelegates::OnObjectsReplaced.RemoveAll(this);
	UPackage::PackageMarkedDirtyEvent.RemoveAll(this);

	if (Blueprint.IsValid())
	{
		Blueprint->OnChanged().RemoveAll(this);
		Blueprint->OnChanged().AddUObject(NewObject, &UBlueprintableDeveloperSettings::OnBlueprintChanged);
	}
	
	if (!IsAppropriateObjectForSettings(NewObject))
	{
		return;
	}
	
	UnregisterSettings();
	NewObject->RegisterSettings();
}

void UBlueprintableDeveloperSettings::OnAssetRemoved(const FAssetData& AssetData)
{
	const auto DeletedAsset = Cast<UBlueprint>(AssetData.GetAsset());
	
	if (!IsValid(DeletedAsset))
	{
		return;
	}

	const auto GeneratedClass = DeletedAsset->GeneratedClass;
	if (!IsValid(GeneratedClass) || GeneratedClass->IsChildOf(StaticClass()))
	{
		return;
	}

	const auto bCreateIfNeeded = false;
	const auto DefaultObject = Cast<UBlueprintableDeveloperSettings>(GeneratedClass->GetDefaultObject(bCreateIfNeeded));

	if (IsValid(DefaultObject))
	{
		DefaultObject->UnregisterSettings();
	}
}

void UBlueprintableDeveloperSettings::OnPackageMarkedDirty(UPackage* Package, bool bArg)
{
	const auto ThisPackage = GetPackage();
	if (Package != ThisPackage)
	{
		return;
	}

	// We set dirty flag to false because we don't want to re-save blueprint if we change its fields through project / editor settings
	if (Package->IsDirty() && Flags == EBlueprintDeveloperSettingsFlags::None)
	{
		Package->SetDirtyFlag(false);
	}
}

void UBlueprintableDeveloperSettings::OnBlueprintPreCompile(UBlueprint* InBlueprint)
{
	const auto bBlueprintValid = IsValid(InBlueprint);
	const auto bGeneratedClassValid = bBlueprintValid && IsValid(InBlueprint->GeneratedClass) && InBlueprint->GeneratedClass == GetClass();
	const auto bSettingsClass = bGeneratedClassValid && InBlueprint->GeneratedClass->IsChildOf(StaticClass());
	const auto bDefaultClass = GetClass() == StaticClass();
	
	if (!bBlueprintValid || !bGeneratedClassValid || !bSettingsClass || bDefaultClass)
	{
		return;
	}

	Flags |= EBlueprintDeveloperSettingsFlags::Compiling;

	checkf(Blueprint.IsValid(), TEXT("Blueprint is not valid. Make sure that you have called UBlueprintableDeveloperSettings::LoadBlueprints()"));
	if (!Blueprint->OnChanged().IsBoundToObject(this))
	{
		Blueprint->OnChanged().AddUObject(this, &UBlueprintableDeveloperSettings::OnBlueprintChanged);
	}
}

void UBlueprintableDeveloperSettings::OnBlueprintCompiled()
{
	if (!(Flags & EBlueprintDeveloperSettingsFlags::Compiling))
	{
		return;
	}
	
	Flags &= ~EBlueprintDeveloperSettingsFlags::Compiling;
}

void UBlueprintableDeveloperSettings::OnBlueprintChanged(UBlueprint* InBlueprint)
{
	if (InBlueprint != Blueprint)
	{
		return;
	}
	
	Flags |= EBlueprintDeveloperSettingsFlags::Changing;
	
	Blueprint->MarkPackageDirty();
	
	Flags &= ~EBlueprintDeveloperSettingsFlags::Changing;

}
#endif

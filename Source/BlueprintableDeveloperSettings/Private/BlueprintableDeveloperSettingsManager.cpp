// Copyright 2023 Gradess Games. All Rights Reserved.

#include "BlueprintableDeveloperSettingsManager.h"
#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsLibrary.h"
#include "EditorAssetLibrary.h"
#include "ISettingsModule.h"

#include "AssetRegistry/IAssetRegistry.h"

TMap<uint32, FBlueprintableSettingsSectionData> UBlueprintableDeveloperSettingsManager::RegisteredSettings;
TSet<uint32> UBlueprintableDeveloperSettingsManager::RegisteredSettingsHashes;

void UBlueprintableDeveloperSettingsManager::PostCDOContruct()
{
	UObject::PostCDOContruct();

	const auto AssetRegistry = IAssetRegistry::Get();
	AssetRegistry->OnFilesLoaded().AddStatic(UBlueprintableDeveloperSettingsManager::OnFilesLoaded);
	AssetRegistry->OnAssetAdded().AddStatic(UBlueprintableDeveloperSettingsManager::OnAssetAdded);
}

void UBlueprintableDeveloperSettingsManager::RegisterSettings(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());
	checkf(!AreSettingsRegistered(SettingsClass), TEXT("%s is already registered!"), *SettingsClass->GetName());

	auto* SettingsObject = GetMutableDefault<UBlueprintableDeveloperSettings>(SettingsClass);
	checkf(IsValid(SettingsObject), TEXT("SettingsObject is not valid!"));

	const auto& SectionData = SettingsObject->GetSectionData();
	
	auto& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.RegisterSettings(
		SectionData.ContainerName,
		SectionData.CategoryName,
		SectionData.SectionName,
		SectionData.DisplayName,
		SectionData.Description,
		SettingsObject
	);

	const auto SettingsId = SettingsClass->GetUniqueID();
	RegisteredSettings.Add(SettingsId, SectionData);
	RegisteredSettingsHashes.Add(GetTypeHash(SectionData));
}

void UBlueprintableDeveloperSettingsManager::UnregisterSettings(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());
	checkf(AreSettingsRegistered(SettingsClass), TEXT("%s is not registered!"), *SettingsClass->GetName());

	const auto SettingsId = SettingsClass->GetUniqueID();
	const auto& SectionData = RegisteredSettings.FindAndRemoveChecked(SettingsId);
	
	auto& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.UnregisterSettings(
		SectionData.ContainerName,
		SectionData.CategoryName,
		SectionData.SectionName
	);

	RegisteredSettingsHashes.Remove(GetTypeHash(SectionData));
}

void UBlueprintableDeveloperSettingsManager::LoadBlueprintSettings()
{
	TArray<FAssetData> FoundSettingsAssets;
	const auto& ParentClassName = UBlueprintableDeveloperSettings::StaticClass()->GetClassPathName();
	UBlueprintableDeveloperSettingsLibrary::FindBlueprints({ParentClassName}, {}, FoundSettingsAssets);

	for (const auto& SettingsAsset : FoundSettingsAssets)
	{
		checkf(!SettingsAsset.IsAssetLoaded(), TEXT("Asset is already loaded"));
		const auto& AssetPath = SettingsAsset.PackageName.ToString();
		
		auto* SettingsObject = Cast<UBlueprint>(UEditorAssetLibrary::LoadAsset(AssetPath));

		// TODO: Add notification if settings object is not valid

		RegisterSettings(SettingsObject->GeneratedClass);
	}
}

bool UBlueprintableDeveloperSettingsManager::AreSettingsRegistered(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());
	checkf(SettingsClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint), TEXT("%s is not compiled from blueprint!"), *SettingsClass->GetName());
	return RegisteredSettings.Contains(SettingsClass->GetUniqueID());
}

bool UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredByData(const FBlueprintableSettingsSectionData& SectionData)
{
	return RegisteredSettingsHashes.Contains(GetTypeHash(SectionData));
}

bool UBlueprintableDeveloperSettingsManager::IsAppropriateObjectForSettings(const UObject* Object)
{
	checkf(IsValid(Object), TEXT("Object is not valid"));
	
	const auto* Class = Object->GetClass();
	const auto& ClassName = Class->GetName();
	const auto bCDO = Object->HasAnyFlags(RF_ClassDefaultObject);
	const auto bSuperClass = Class == UBlueprintableDeveloperSettings::StaticClass();
	const auto bSettingsClass = Class->IsChildOf(UBlueprintableDeveloperSettings::StaticClass());
	const auto bSkeletonClass = ClassName.StartsWith(TEXT("SKEL_"));
	const auto bReinstancedClass = ClassName.StartsWith(TEXT("REINST_"));
	
	return bCDO && bSettingsClass && !bSuperClass && !bSkeletonClass && !bReinstancedClass;
}

void UBlueprintableDeveloperSettingsManager::OnFilesLoaded()
{
	LoadBlueprintSettings();
}

void UBlueprintableDeveloperSettingsManager::OnAssetAdded(const FAssetData& AssetData)
{
	// static FName BlueprintClassName = FName(TEXT("Blueprint"));

	// const auto& AssetClass = AssetData.AssetClassPath;
	// if (AssetClass.GetAssetName() != BlueprintClassName)
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
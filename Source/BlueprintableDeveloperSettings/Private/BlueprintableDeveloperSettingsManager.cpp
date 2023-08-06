// Copyright 2023 Gradess Games. All Rights Reserved.

#include "BlueprintableDeveloperSettingsManager.h"
#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsLibrary.h"
#include "EditorAssetLibrary.h"
#include "ISettingsModule.h"

#include "Algo/ForEach.h"

#include "AssetRegistry/IAssetRegistry.h"

TMap<FBlueprintableSettingsSectionData, uint32> UBlueprintableDeveloperSettingsManager::RegisteredSettings;

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
	checkf(SettingsClass->HasAnyClassFlags(CLASS_CompiledFromBlueprint), TEXT("%s is not compiled from blueprint!"), *SettingsClass->GetName());
	checkf(!AreSettingsRegistered(SettingsClass), TEXT("%s is already registered!"), *SettingsClass->GetName());
	checkf(IsValid(SettingsClass->ClassDefaultObject), TEXT("CDO for %s is not valid!"), *SettingsClass->GetName());

	auto* SettingsObject = Cast<UBlueprintableDeveloperSettings>(SettingsClass->ClassDefaultObject);
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

	const auto ClassId = SettingsClass->GetUniqueID();
	RegisteredSettings.Add(SectionData, ClassId);
}

void UBlueprintableDeveloperSettingsManager::UnregisterSettings(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());
	checkf(AreSettingsRegistered(SettingsClass), TEXT("%s is not registered!"), *SettingsClass->GetName());

	const auto* SectionData = FindSectionDataByClass(SettingsClass);

	auto& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.UnregisterSettings(
		SectionData->ContainerName,
		SectionData->CategoryName,
		SectionData->SectionName
	);

	RegisteredSettings.Remove(*SectionData);
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
		
		UEditorAssetLibrary::LoadAsset(AssetPath);
		// TODO: Add notification if settings object is not valid
	}
}

bool UBlueprintableDeveloperSettingsManager::AreSettingsRegistered(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());

	if (IsValid(SettingsClass->ClassDefaultObject))
	{
		const auto* SettingsObject = Cast<UBlueprintableDeveloperSettings>(SettingsClass->ClassDefaultObject);
		const auto& SectionData = SettingsObject->GetSectionData();
		return AreSettingsRegisteredByData(SectionData);
	}

	return FindSectionDataByClass(SettingsClass) != nullptr;
}

bool UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredByData(const FBlueprintableSettingsSectionData& SectionData)
{
	return RegisteredSettings.Contains(SectionData);
}

const FBlueprintableSettingsSectionData* UBlueprintableDeveloperSettingsManager::FindSectionDataByClass(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());
	
	const auto ClassId = SettingsClass->GetUniqueID();
	return FindSectionDataByClassId(ClassId);
}

const FBlueprintableSettingsSectionData* UBlueprintableDeveloperSettingsManager::FindSectionDataByClassId(uint32 ClassId)
{
	const auto* FoundData = Algo::FindByPredicate(RegisteredSettings, [ClassId](const auto& Pair) { return Pair.Value == ClassId; });
	return FoundData ? &FoundData->Key : nullptr;
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

uint32 UBlueprintableDeveloperSettingsManager::GetRegisteredClassId(const FBlueprintableSettingsSectionData& SectionData)
{
	return AreSettingsRegisteredByData(SectionData) ? RegisteredSettings[SectionData] : 0;
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
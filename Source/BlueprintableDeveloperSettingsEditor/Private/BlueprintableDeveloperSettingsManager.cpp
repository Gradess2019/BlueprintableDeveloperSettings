// Copyright 2023 Gradess Games. All Rights Reserved.

#include "BlueprintableDeveloperSettingsManager.h"
#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsLibrary.h"
#include "EditorAssetLibrary.h"
#include "FileHelpers.h"
#include "ISettingsModule.h"
#include "Algo/ForEach.h"
#include "AssetRegistry/IAssetRegistry.h"

TMap<int32, FBlueprintableSettingsSectionData> UBlueprintableDeveloperSettingsManager::RegisteredSettings;

void UBlueprintableDeveloperSettingsManager::PostCDOContruct()
{
	UObject::PostCDOContruct();

	const auto AssetRegistry = IAssetRegistry::Get();
	AssetRegistry->OnFilesLoaded().AddStatic(UBlueprintableDeveloperSettingsManager::OnFilesLoaded);
	AssetRegistry->OnInMemoryAssetCreated().AddStatic(UBlueprintableDeveloperSettingsManager::OnAssetAdded);
	GUObjectArray.AddUObjectDeleteListener(this);
}

void UBlueprintableDeveloperSettingsManager::BeginDestroy()
{
	GUObjectArray.RemoveUObjectDeleteListener(this);
	
	UObject::BeginDestroy();
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
	RegisteredSettings.Add(ClassId, SectionData);
}

void UBlueprintableDeveloperSettingsManager::UnregisterSettings(TSubclassOf<UObject> SettingsClass)
{
	checkf(IsValid(SettingsClass), TEXT("SettingsClass is not valid!"));
	checkf(SettingsClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()), TEXT("%s is not child of UBlueprintableDeveloperSettings!"), *SettingsClass->GetName());
	checkf(AreSettingsRegistered(SettingsClass), TEXT("%s is not registered!"), *SettingsClass->GetName());

	const auto ClassId = SettingsClass->GetUniqueID();
	UnregisterSettings(ClassId);
}

void UBlueprintableDeveloperSettingsManager::UnregisterSettings(int32 ClassId)
{
	checkf(ClassId != INDEX_NONE, TEXT("ClassId is not valid!"));
	checkf(AreSettingsRegisteredById(ClassId), TEXT("Settings under ClassId %d are not registered!"), ClassId);
	
	const auto& SectionData = RegisteredSettings.FindAndRemoveChecked(ClassId);

	auto& SettingsModule = FModuleManager::LoadModuleChecked<ISettingsModule>("Settings");
	SettingsModule.UnregisterSettings(
		SectionData.ContainerName,
		SectionData.CategoryName,
		SectionData.SectionName
	);
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

	return AreSettingsRegisteredById(SettingsClass->GetUniqueID());
}

bool UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredById(int32 ClassId)
{
	return RegisteredSettings.Contains(ClassId);
}

bool UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredByData(const FBlueprintableSettingsSectionData& SectionData)
{
	return FindClassIdBySectionData(SectionData) != INDEX_NONE;
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
	return RegisteredSettings.Find(ClassId);
}

int32 UBlueprintableDeveloperSettingsManager::FindClassIdBySectionData(const FBlueprintableSettingsSectionData& SectionData)
{
	const auto* FoundData = Algo::FindByPredicate(RegisteredSettings, [&SectionData](const auto& Pair) { return Pair.Value == SectionData; });
	return FoundData != nullptr ? FoundData->Key : INDEX_NONE;
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

void UBlueprintableDeveloperSettingsManager::NotifyUObjectDeleted(const UObjectBase* Object, int32 Index)
{
	if (AreSettingsRegisteredById(Index))
	{
		UnregisterSettings(Index);
	}
}

void UBlueprintableDeveloperSettingsManager::OnUObjectArrayShutdown()
{
	GUObjectArray.RemoveUObjectDeleteListener(this);
}

void UBlueprintableDeveloperSettingsManager::OnFilesLoaded()
{
	LoadBlueprintSettings();
}

void UBlueprintableDeveloperSettingsManager::OnAssetAdded(UObject* InAsset)
{
	const auto* Blueprint = Cast<UBlueprint>(InAsset);
	if (!IsValid(Blueprint))
	{
		return;
	}

	const auto GeneratedClass = Blueprint->GeneratedClass;
	if (!IsValid(GeneratedClass) || !GeneratedClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()))
	{
		return;
	}

	const auto Package = Blueprint->GetPackage();
	GEditor->GetTimerManager()->SetTimerForNextTick([Package]
	{
		UEditorLoadingAndSavingUtils::SavePackages({ Package }, false);
	});
}
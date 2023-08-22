// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettings.h"

#include "BlueprintableDeveloperSettingsManager.h"

#if WITH_EDITOR
#include "AssetRegistry/IAssetRegistry.h"
#include "FileHelpers.h"
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

void UBlueprintableDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	auto* Package = GetPackage();
	checkf(IsValid(Package), TEXT("Package is not valid"));

	// We set the dirty flag to false so that the package will not be marked dirty when we change values in settings 
	Package->SetDirtyFlag(false);
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
#endif

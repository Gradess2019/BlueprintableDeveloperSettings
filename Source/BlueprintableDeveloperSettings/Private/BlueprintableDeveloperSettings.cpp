// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettings.h"

#if WITH_EDITOR
#include "AssetRegistry/IAssetRegistry.h"
#endif

UBlueprintableDeveloperSettings::FOnBlueprintableDeveloperSettingsEvent UBlueprintableDeveloperSettings::OnDuplicate;

UBlueprintableDeveloperSettings::UBlueprintableDeveloperSettings()
{
	ConfigName = TEXT("BlueprintableDeveloperSettings");
	
	SectionData.ContainerName = TEXT("Editor");
	SectionData.CategoryName = TEXT("Blueprintable Developer Settings");
	SectionData.SectionName = TEXT("Base Settings");
	SectionData.DisplayName = FText::FromName(GetFName());
	SectionData.Description = FText::FromName(GetFName());
}

void UBlueprintableDeveloperSettings::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);

	if (Ar.IsLoading())
	{
		auto* Class = GetClass();
		const auto* ClassDefaultObject = Cast<UBlueprintableDeveloperSettings>(Class->ClassDefaultObject);

		checkf(IsValid(ClassDefaultObject), TEXT("ClassDefaultObject of %s class is not valid"), *Class->GetName());
		
		Class->ClassConfigName = ClassDefaultObject->ConfigName;
		Class->ClassFlags |= CLASS_Config;
		
		if (ClassDefaultObject->bDefaultConfig)
		{
			Class->ClassFlags |= CLASS_DefaultConfig;
		}
		
		LoadConfig();
	}
}

#if WITH_EDITOR
void UBlueprintableDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, bDefaultConfig))
	{
		SwitchConfigs();
	}

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, ConfigName))
	{
		ConfigsCleanup();
	}

	UpdateConfig();

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
		OnDuplicate.Broadcast(CDO);
	}
}

void UBlueprintableDeveloperSettings::SwitchConfigs()
{
	auto* Class = GetClass();

	const auto& OldConfigFilename = bDefaultConfig ? GetConfigFilename(this) : GetDefaultConfigFilename();
	const auto& ConfigSection = Class->GetPathName();
		
	if (bDefaultConfig)
	{
		Class->ClassFlags |= CLASS_DefaultConfig;
		TryUpdateDefaultConfigFile();
	}
	else
	{
		Class->ClassFlags &= ~CLASS_DefaultConfig;
		SaveConfig();
	}

	GConfig->EmptySection(*ConfigSection, OldConfigFilename);
}

void UBlueprintableDeveloperSettings::UpdateConfig()
{
	if (bDefaultConfig)
	{
		TryUpdateDefaultConfigFile();
	}
	else
	{
		SaveConfig();
	}
}

void UBlueprintableDeveloperSettings::ConfigsCleanup()
{
	auto* Class = GetClass();
	const auto& NewConfigName = ConfigName;
	const auto& OldConfigFileName = GetConfigFilename(this);
		
	const auto& ConfigSection = Class->GetPathName();
		
	GConfig->EmptySection(*ConfigSection, OldConfigFileName);
		
	Class->ClassFlags |= CLASS_Config;
	Class->ClassConfigName = NewConfigName;

	SaveConfig();
}
#endif

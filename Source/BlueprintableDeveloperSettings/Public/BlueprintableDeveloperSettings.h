// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "BlueprintableDeveloperSettingsTypes.h"

#include "BlueprintableDeveloperSettings.generated.h"


/**
 * This is a base class for developer settings that can be registered in "Project Settings" or "Editor Preferences".
 * It handles automatic loading and saving of variables marked as "Config Variable" from the config file.
 */
UCLASS(Blueprintable, BlueprintType)
class BLUEPRINTABLEDEVELOPERSETTINGS_API UBlueprintableDeveloperSettings : public UObject
{
	GENERATED_BODY()

	friend class UBlueprintableDeveloperSettingsCompilerExtension;
	friend class FBlueprintableDeveloperSettingsDetails;

public:
	DECLARE_EVENT_OneParam(UBlueprintableDeveloperSettings, FOnBlueprintableDeveloperSettingsEvent, const UBlueprintableDeveloperSettings*);

	static FOnBlueprintableDeveloperSettingsEvent OnDuplicate;

protected:
	/** Controls whether these settings should be registered. */
	UPROPERTY(EditDefaultsOnly, meta = (HideInSettings), Category = "BlueprintableDeveloperSettings")
	bool bRegister = true;

	/**
	 * Controls whether these settings serve as the default configuration.
	 * If set to true, the values will be automatically saved in the Default*.ini configuration file.
	 */
	UPROPERTY(EditDefaultsOnly, meta = (HideInSettings), Category = "BlueprintableDeveloperSettings")
	bool bDefaultConfig = false;

	/** The name identifying the config name. */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetConfigName", meta=(HideInSettings), Category = "BlueprintableDeveloperSettings")
	FName ConfigName;

	/** Data defining the section for these settings, visible in "Project Settings" / "Editor Preferences" */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetSectionData", Category = "BlueprintableDeveloperSettings", meta = (ShowOnlyInnerProperties, HideInSettings))
	FBlueprintableSettingsSectionData SectionData;

public:
	UBlueprintableDeveloperSettings();

	virtual void Serialize(FArchive& Ar) override;

	UFUNCTION(BlueprintGetter, Category = "BlueprintableDeveloperSettings")
	bool IsRegistered() const { return bRegister; }

	UFUNCTION(BlueprintGetter, Category = "BlueprintableDeveloperSettings")
	bool IsDefaultConfig() const { return bDefaultConfig; }

	UFUNCTION(BlueprintGetter, Category = "BlueprintableDeveloperSettings")
	const FName& GetConfigName() const { return ConfigName; }

	UFUNCTION(BlueprintGetter, Category = "BlueprintableDeveloperSettings")
	const FBlueprintableSettingsSectionData& GetSectionData() const { return SectionData; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;

protected:
	virtual void SwitchConfigs();
	virtual void ConfigsCleanup();
	virtual void UpdateConfig();
#endif
};

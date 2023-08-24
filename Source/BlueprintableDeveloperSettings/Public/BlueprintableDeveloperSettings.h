// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <UObject/Object.h>

#include "BlueprintableDeveloperSettingsTypes.h"

#include "BlueprintableDeveloperSettings.generated.h"


/**
 * 
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
	/** Should register these settings */
	UPROPERTY(EditDefaultsOnly, meta = (HideInSettings))
	bool bRegister = true;

	UPROPERTY(EditDefaultsOnly, meta = (HideInSettings))
	bool bDefaultConfig = false;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = "GetConfigName", meta=(HideInSettings))
	FName ConfigName;

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

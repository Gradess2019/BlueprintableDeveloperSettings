// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintableDeveloperSettingsTypes.generated.h"

class UBlueprintableDeveloperSettings;
USTRUCT(BlueprintType)
struct FBlueprintableSettingsSectionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BlueprintableSettingsSection")
	FName ContainerName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BlueprintableSettingsSection")
	FName CategoryName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BlueprintableSettingsSection")
	FName SectionName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BlueprintableSettingsSection")
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "BlueprintableSettingsSection")
	FText Description;

	bool operator==(const FBlueprintableSettingsSectionData& Other) const
	{
		return ContainerName == Other.ContainerName && CategoryName == Other.CategoryName && SectionName == Other.SectionName;
	}
};

inline uint32 GetTypeHash(const FBlueprintableSettingsSectionData& Key)
{
	return HashCombine(GetTypeHash(Key.ContainerName), HashCombine(GetTypeHash(Key.CategoryName), GetTypeHash(Key.SectionName)));
}
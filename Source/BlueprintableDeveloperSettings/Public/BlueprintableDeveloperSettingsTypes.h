// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>

#include "BlueprintableDeveloperSettingsTypes.generated.h"

#define ENUM_CLASS_FLAGS_UINT8(Enum) \
	inline uint8& operator|=(uint8& Lhs, Enum Rhs) { return Lhs |= static_cast<uint8>(Rhs); } \
	inline uint8& operator&=(uint8& Lhs, Enum Rhs) { return Lhs &= static_cast<uint8>(Rhs); } \
	inline uint8& operator^=(uint8& Lhs, Enum Rhs) { return Lhs ^= static_cast<uint8>(Rhs); } \
	inline uint8 operator| (uint8 Lhs, Enum Rhs) { return Lhs | static_cast<uint8>(Rhs); } \
	inline uint8 operator& (uint8 Lhs, Enum Rhs) { return Lhs & static_cast<uint8>(Rhs); } \
	inline uint8 operator^ (uint8 Lhs, Enum Rhs) { return Lhs ^ static_cast<uint8>(Rhs); } \
	inline uint8 operator==(uint8 Lhs, Enum Rhs) { return Lhs == static_cast<uint8>(Rhs); }

UENUM(BlueprintType, meta=(Bitflags))
enum class EBlueprintDeveloperSettingsFlags : uint8
{
	None = 0,

	Compiling = 1 << 1,
	Changing = 1 << 2,
	Renaming = 1 << 3
};

ENUM_CLASS_FLAGS(EBlueprintDeveloperSettingsFlags);
ENUM_CLASS_FLAGS_UINT8(EBlueprintDeveloperSettingsFlags);

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

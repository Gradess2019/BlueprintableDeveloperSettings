// Copyright 2023 Gradess Games. All Rights Reserved.

#if WITH_EDITOR
#include "BlueprintableDeveloperSettingsCompilerExtension.h"

#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsManager.h"

#include "Kismet2/BlueprintEditorUtils.h"

FBlueprintableSettingsSectionData UBlueprintableDeveloperSettingsCompilerExtension::GetSectionData(UClass* InClass, UObject* InCDO)
{
	const auto* Property = FindFProperty<FStructProperty>(InClass, GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, SectionData));
	return *Property->ContainerPtrToValuePtr<FBlueprintableSettingsSectionData>(InCDO);
}

bool UBlueprintableDeveloperSettingsCompilerExtension::AreSettingsRegistered(UBlueprint* InBlueprint, UObject* InCDO)
{
	const auto SectionData = GetSectionData(InBlueprint->GeneratedClass, InCDO);
	return UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredByData(SectionData);
}

bool UBlueprintableDeveloperSettingsCompilerExtension::ValidateSettings(UBlueprint* InBlueprint, UObject* InCDO, UClass* InSettingsClass)
{
	if (AreSettingsRegistered(InBlueprint, InCDO))
	{
		const auto SectionData = GetSectionData(InBlueprint->GeneratedClass, InCDO);
		const auto RegisteredClassId = UBlueprintableDeveloperSettingsManager::FindClassIdBySectionData(SectionData);
		const auto CompiledClassId = InSettingsClass->GetUniqueID();
		
		if (RegisteredClassId != CompiledClassId)
		{
			InBlueprint->Message_Error(TEXT("Couldn't register settings for @@. Settings with such Container, Category and Section names are already registered!"), *InBlueprint->GetName());
			InBlueprint->Status = BS_Error;
			return false;
		}
	}
	
	return true;
}

void UBlueprintableDeveloperSettingsCompilerExtension::ProcessBlueprintCompiled(const FKismetCompilerContext& CompilationContext, const FBlueprintCompiledData& Data)
{
	const auto bValidBlueprint = IsValid(CompilationContext.NewClass) && CompilationContext.NewClass->IsChildOf(UBlueprintableDeveloperSettings::StaticClass());
	if (!bValidBlueprint)
	{
		return;
	}

	if (IsValid(CompilationContext.OldClass))
	{
		if (!ValidateSettings(CompilationContext.Blueprint, CompilationContext.OldClass->ClassDefaultObject, CompilationContext.NewClass))
		{
			return;
		}

		if (UBlueprintableDeveloperSettingsManager::AreSettingsRegisteredById(CompilationContext.NewClass->GetUniqueID()))
		{
			UBlueprintableDeveloperSettingsManager::UnregisterSettings(CompilationContext.NewClass);
		}
	}

	CompilationContext.Blueprint->OnCompiled().AddUObject(this, &UBlueprintableDeveloperSettingsCompilerExtension::OnBlueprintCompiled);
}

void UBlueprintableDeveloperSettingsCompilerExtension::OnBlueprintCompiled(UBlueprint* Blueprint)
{
	Blueprint->OnCompiled().RemoveAll(this);
	if (Blueprint->Status != BS_UpToDate && Blueprint->Status != BS_UpToDateWithWarnings)
	{
		return;
	}

	if (!ValidateSettings(Blueprint, Blueprint->GeneratedClass->ClassDefaultObject, Blueprint->GeneratedClass))
	{
		return;
	}
	
	UBlueprintableDeveloperSettingsManager::RegisterSettings(Blueprint->GeneratedClass);
}
#endif

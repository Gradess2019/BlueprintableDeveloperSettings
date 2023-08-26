// Copyright 2023 Gradess Games. All Rights Reserved.

#include "BlueprintableDeveloperSettingsEditorModule.h"
#include "BlueprintableDeveloperSettings.h"
#include "BlueprintableDeveloperSettingsDetails.h"
#include "BlueprintableDeveloperSettingsCompilerExtension.h"

#include <BlueprintCompilationManager.h>

#define LOCTEXT_NAMESPACE "FBlueprintableDeveloperSettingsEditorModule"

void FBlueprintableDeveloperSettingsEditorModule::StartupModule()
{
	CompilerExtension = NewObject<UBlueprintableDeveloperSettingsCompilerExtension>();
	CompilerExtension->AddToRoot();

	FBlueprintCompilationManager::RegisterCompilerExtension(UBlueprint::StaticClass(), CompilerExtension);

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	const auto& ClassName = UBlueprintableDeveloperSettings::StaticClass()->GetFName();
	const auto& DetailLayoutDelegate = FOnGetDetailCustomizationInstance::CreateStatic(FBlueprintableDeveloperSettingsDetails::MakeInstance);
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}

void FBlueprintableDeveloperSettingsEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBlueprintableDeveloperSettingsEditorModule, BlueprintableDeveloperSettingsEditorModule)

// Copyright 2023 Gradess Games. All Rights Reserved.
#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "BlueprintCompilerExtension.h"
#include "BlueprintableDeveloperSettingsCompilerExtension.generated.h"

UCLASS()
class BLUEPRINTABLEDEVELOPERSETTINGS_API UBlueprintableDeveloperSettingsCompilerExtension : public UBlueprintCompilerExtension
{
	GENERATED_BODY()
public:
	
	
protected:
	virtual void ProcessBlueprintCompiled(const FKismetCompilerContext& CompilationContext, const FBlueprintCompiledData& Data) override;
	
	void OnBlueprintCompiled(UBlueprint* Blueprint);
};
#endif
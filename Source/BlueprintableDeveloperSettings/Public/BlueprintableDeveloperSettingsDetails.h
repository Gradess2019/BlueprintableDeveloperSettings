// Copyright 2023 Gradess Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"

class BLUEPRINTABLEDEVELOPERSETTINGS_API FBlueprintableDeveloperSettingsDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

protected:
	bool IsBlueprintInspector(IDetailLayoutBuilder& DetailBuilder);
	TArray<TSharedRef<IPropertyHandle>> CollectProperties(IDetailLayoutBuilder& DetailBuilder);
};

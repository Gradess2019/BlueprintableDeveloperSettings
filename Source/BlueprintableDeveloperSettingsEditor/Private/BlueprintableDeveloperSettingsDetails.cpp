// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettingsDetails.h"
#include "BlueprintableDeveloperSettings.h"

#include <DetailCategoryBuilder.h>
#include <DetailLayoutBuilder.h>
#include <Algo/ForEach.h>

TSharedRef<IDetailCustomization> FBlueprintableDeveloperSettingsDetails::MakeInstance()
{
	return MakeShared<FBlueprintableDeveloperSettingsDetails>();
}

void FBlueprintableDeveloperSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	static FName HideInSettingsMeta(TEXT("HideInSettings"));

	const auto* DetailsView = DetailBuilder.GetDetailsView();

	if (!ensure(DetailsView))
	{
		return;
	}

	if (IsBlueprintInspector(DetailBuilder))
	{
		return;
	}

	const auto PropertyHandles = CollectProperties(DetailBuilder);

	Algo::ForEach(PropertyHandles, [&](const TSharedRef<IPropertyHandle>& PropertyHandle)
	{
		if (PropertyHandle->HasMetaData(HideInSettingsMeta) || !PropertyHandle->GetProperty()->HasAnyPropertyFlags(CPF_Config))
		{
			DetailBuilder.HideProperty(PropertyHandle);
		}
	});
}

bool FBlueprintableDeveloperSettingsDetails::IsBlueprintInspector(IDetailLayoutBuilder& DetailBuilder)
{
	static FName BlueprintIdentifier(TEXT("BlueprintInspector"));
	const auto DetailsViewIdentifier = DetailBuilder.GetDetailsView()->GetIdentifier();

	if (DetailsViewIdentifier == BlueprintIdentifier)
	{
		return true;
	}
	return false;
}

TArray<TSharedRef<IPropertyHandle>> FBlueprintableDeveloperSettingsDetails::CollectProperties(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<FName> Categories;
	DetailBuilder.GetCategoryNames(Categories);

	TArray<TSharedRef<IPropertyHandle>> Properties;

	for (const auto& Category : Categories)
	{
		auto& CategoryBuilder = DetailBuilder.EditCategory(Category);

		TArray<TSharedRef<IPropertyHandle>> CategoryProperties;
		CategoryBuilder.GetDefaultProperties(CategoryProperties);

		Properties.Append(CategoryProperties);
	}

	return Properties;
}

// Copyright 2023 Gradess Games. All Rights Reserved.


#include "BlueprintableDeveloperSettingsDetails.h"

#include "BlueprintableDeveloperSettings.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

#include "Algo/ForEach.h"

TSharedRef<IDetailCustomization> FBlueprintableDeveloperSettingsDetails::MakeInstance()
{
	return MakeShared<FBlueprintableDeveloperSettingsDetails>();
}

void FBlueprintableDeveloperSettingsDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	const auto* DetailsView = DetailBuilder.GetDetailsView();

	if (!ensure(DetailsView))
	{
		return;
	}

	if (IsBlueprintInspector(DetailBuilder))
	{
		return;
	}

	HideSectionData(DetailBuilder);
	HideNonConfigVariables(DetailBuilder);
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

void FBlueprintableDeveloperSettingsDetails::HideSectionData(IDetailLayoutBuilder& DetailBuilder)
{
	const auto Property = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UBlueprintableDeveloperSettings, SectionData));

	uint32 ChildrenCount;
	Property->GetNumChildren(ChildrenCount);

	for (uint32 Id = 0; Id < ChildrenCount; ++Id)
	{
		auto Child = Property->GetChildHandle(Id);
		DetailBuilder.HideProperty(Child);
	}
}

void FBlueprintableDeveloperSettingsDetails::HideNonConfigVariables(IDetailLayoutBuilder& DetailBuilder)
{
	const auto& SelectedObjects = DetailBuilder.GetSelectedObjects();

	for (auto Object : SelectedObjects)
	{
		if (!Object.IsValid() || !Object->GetClass()->IsChildOf(UBlueprintableDeveloperSettings::StaticClass()))
		{
			continue;
		}

		const auto* SettingsObject = Cast<UBlueprintableDeveloperSettings>(Object.Get());
		const auto* SettingsClass = SettingsObject->GetClass();

		for (TFieldIterator<FProperty> It(SettingsClass, EFieldIteratorFlags::IncludeSuper); It; ++It)
		{
			const auto* Property = *It;

			if (Property->HasAnyPropertyFlags(CPF_Config))
			{
				continue;
			}

			const auto Name = Property->GetFName();
			const auto PropertyHandle = FindPropertyHandle(DetailBuilder, Name);

			DetailBuilder.HideProperty(PropertyHandle);
		}
	}
}

TSharedRef<IPropertyHandle> FBlueprintableDeveloperSettingsDetails::FindPropertyHandle(IDetailLayoutBuilder& DetailBuilder, const FName& PropertyName)
{
	auto PropertyHandle = DetailBuilder.GetProperty(PropertyName);
	if (PropertyHandle->IsValidHandle())
	{
		return PropertyHandle;
	}

	TArray<FName> Categories;
	DetailBuilder.GetCategoryNames(Categories);

	for (auto& Category : Categories)
	{
		auto& CategoryBuilder = DetailBuilder.EditCategory(Category);

		TArray<TSharedRef<IPropertyHandle>> DefaultProperties;
		CategoryBuilder.GetDefaultProperties(DefaultProperties);

		auto* PropertyHandlePtr = DefaultProperties.FindByPredicate([&](const TSharedRef<IPropertyHandle>& PropertyHandle)
		{
			if (PropertyHandle->IsValidHandle() && PropertyHandle->GetProperty()->GetFName() == PropertyName)
			{
				return true;
			}
			return false;
		});

		if (PropertyHandlePtr)
		{
			return *PropertyHandlePtr;
		}
	}

	// PropertyHandle will be invalid
	return PropertyHandle;
}

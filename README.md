# Blueprintable Developer Settings

## Description
Blueprintable Developer Settings is a powerful and versatile plugin designed to enhance your Unreal Engine development workflow by seamlessly integrating configurable settings into your projects. Building upon the foundation of the C++ UDeveloperSettings class, the plugin empowers developers to create, manage, and customize settings with unparalleled ease.

## How to install

### Marketplace
Coming soon...

### GitHub
1. Download [latest release](https://github.com/Gradess2019/BlueprintableDeveloperSettings/releases/latest) for your Unreal Engine version
2. Unzip into: **\<ProjectDirectory\>/Plugins** (create Plugins directory if it doesn't exist)
3. If you are using C++: Right Mouse Button on your **.uproject** file -> Generate Visual Studio project files
4. Launch project
5. If it's not enabled: Go to Edit -> Plugins -> "Project" category -> Editor -> Enable "Blueprintable Developer Settings" and restart the editor
7. Done

## Restrictions
It is not possible to have two or more blueprints sharing the same Container, Category Name, and Section combination.

## How to use

1. **Blueprint Creation:**
- Start by creating a new blueprint.
- Select "Blueprintable Developer Settings" as the parent blueprint class.

    ![image](https://github.com/Gradess2019/BlueprintableDeveloperSettings/assets/38568823/ac342483-373d-4f7c-9896-9408f35cf6c1)

- Upon blueprint creation, your settings will be automatically registered.
To verify, navigate to "Editor Preferences" and locate "Blueprintable Developer Settings".


- Open the blueprint you've created.

2. **Data Setup:**
- In the class defaults settings, configure the section data.

    ![image](https://github.com/Gradess2019/BlueprintableDeveloperSettings/assets/38568823/837a515e-6baa-4af3-92f4-84c4faad9ae2)

- Specify registration details such as Project/Editor tab, category, and section.

    ![image](https://github.com/Gradess2019/BlueprintableDeveloperSettings/assets/38568823/dc5c2090-3f3e-4a8c-af68-0ee23976a19b)


- Introduce a variable to the blueprint and set its default value if necessary.

- To enable a variable to be configured within settings and stored in the config file, toggle the "Config Variable" flag within the "Advanced" section.

    ![image](https://github.com/Gradess2019/BlueprintableDeveloperSettings/assets/38568823/dc58c821-b21d-4fcf-8258-a4ba9fa7ae8c)


- Compile the blueprint and ensure you save your changes.
- Navigate to Project Settings / Editor Preferences to locate and access your configured settings.
- Adjust the settings as needed to match your preferences and project requirements.

3. **Blueprint Usage:**
- In any blueprint, insert a "Get Class Defaults" node.
- Choose the settings blueprint class you created earlier.

    ![image](https://github.com/Gradess2019/BlueprintableDeveloperSettings/assets/38568823/d9da3614-1cc9-4eaf-a1bb-964209b6c135)

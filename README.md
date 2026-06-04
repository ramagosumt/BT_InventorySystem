# Amicus School of Engineering

### Unreal Engine 5.6 C++ Gameplay Systems Framework

*"Lupus solitarius perit dum grex vivit."*

Amicus School of Engineering is a third-person interactive fiction RPG prototype built around reusable Unreal Engine C++ gameplay systems. The project began as a multi-paradigm inventory framework and expanded into a broader gameplay architecture supporting interaction, dialogue, item data, UI, time simulation, and future quest/NPC schedule systems.

---

## Programming Portfolio Notes

This repository is submitted as a programming portfolio sample for Unreal Engine C++ gameplay architecture.

The project was developed independently as my bachelor thesis and is intended to demonstrate how I design scalable gameplay systems using interfaces, components, data-driven workflows, and delegate-based communication.

This is not only an inventory project. The inventory system was the original entry point, but the current codebase functions as a reusable RPG gameplay framework.

---

## Why This Project

I selected this project as my primary programming portfolio piece because it shows my progression from implementing isolated game mechanics to designing interconnected gameplay systems.

The codebase demonstrates:

* Interface-driven interaction
* Component-based gameplay architecture
* Data-driven dialogue and item definitions
* Multi-paradigm inventory systems
* Clean separation between world actors, runtime objects, and data assets
* Event-driven time simulation
* Runtime UI generation
* Foundation for quest and NPC schedule systems

---

## Core Architecture

The project is organized around several reusable gameplay layers:

```text
Interaction Layer
Dialogue Layer
Item / Inventory Layer
Time Simulation Layer
UI Layer
Framework Layer
```

Major source folders:

```text
Actors/        World objects, pickups, 3D inventory actors, time manager
Characters/    Player character and base NPC classes
Components/    Reusable gameplay components
Data/          Data table structs for dialogue and items
Frameworks/    GameMode, PlayerController, GameState, HUD
Interfaces/    Interaction and time interfaces
Objects/       Runtime item object types
UIs/           Dialogue, interaction, inventory, and menu widgets
```

---

## Architecture Highlights

### Interface-Driven Interaction

The interaction system uses `IInteractionInterface` and reusable interaction components to decouple interactable actors from player tracing logic.

New interactable actors can be added by implementing the interface rather than modifying core interaction code.

### Data / Object / Actor Separation

Items are separated into three layers:

* `BaseItemData` defines item identity and static data
* `BaseItem` represents runtime inventory objects
* `BasePickupItem` represents physical world pickups

This avoids placing all item behavior into a single actor class and makes inventory, world interaction, and UI easier to maintain independently.

### Multi-Paradigm Inventory

The project supports three inventory paradigms:

* Grid inventory
* List inventory
* Slot inventory

Each inventory type is implemented as a separate component while sharing common item data and UI principles.

### Data-Driven Dialogue

Dialogue is driven through structured dialogue data rather than hardcoded conversation logic.

The dialogue system supports branching, choice widgets, row selection, loop behavior, and cinematic mode integration.

### Event-Driven Time Simulation

The time system is hosted through `CentralTimeComponent` and coordinated by `TimeManager`.

Time progression broadcasts events such as:

* `OnNewMinute`
* `OnNewHour`
* `OnMorning`
* `OnEvening`
* `OnNewDay`

Other systems can respond to time without directly depending on time implementation details.

---

## Code Review Guide

Recommended files to inspect first:

### Interaction

* `Public/Interfaces/InteractionInterface.h`
* `Public/Components/BaseInteractableComponent.h`
* `Private/Components/BaseInteractableComponent.cpp`
* `Private/Components/TraceInteractionComponent.cpp`

Shows interface-driven interaction, focus/unfocus behavior, and trace-based object detection.

### Dialogue

* `Public/Components/DialogueComponent.h`
* `Private/Components/DialogueComponent.cpp`
* `Public/Data/BaseDialogueData.h`
* `Private/UIs/DialogueWidget.cpp`
* `Private/UIs/DialogueChoiceWidget.cpp`

Shows data-driven branching dialogue and runtime dialogue UI generation.

### Inventory

* `Public/Components/GridInventoryComponent.h`
* `Public/Components/ListInventoryComponent.h`
* `Public/Components/SlotInventoryComponent.h`
* `Public/Data/BaseItemData.h`
* `Public/Objects/BaseItem.h`
* `Public/Actors/BasePickupItem.h`

Shows item data architecture, inventory object ownership, and three inventory implementations.

### Time

* `Public/Components/CentralTimeComponent.h`
* `Private/Components/CentralTimeComponent.cpp`
* `Public/Actors/TimeManager.h`
* `Private/Actors/TimeManager.cpp`
* `Public/Frameworks/MainGameState.h`

Shows global time simulation, delegate broadcasts, time scaling, and day/night cycle management.

---

## Build Instructions

Requirements:

* Unreal Engine 5.6
* Visual Studio 2022

Setup:

1. Clone the repository
2. Right-click `BachelorThesis.uproject`
3. Select **Generate Visual Studio project files**
4. Open the generated solution
5. Build `Development Editor`
6. Open `BachelorThesis.uproject`

---

## Game Overview

Players follow 18-year-old Nathan Nguyen as he experiences life at Amicus School of Engineering through club activities, relationships, exploration, and hidden mysteries beneath the school.

The game is inspired by:

| System                      | Inspiration           |
| --------------------------- | --------------------- |
| World density               | Yakuza 0              |
| Day structure               | Persona 5             |
| School life                 | Bully                 |
| NPC identity                | Watch Dogs 2          |
| Narrative branching         | AI: The Somnium Files |
| Environmental mystery       | Control               |
| Festivals and relationships | Stardew Valley        |

---

## Current Systems

| System               | Status      |
| -------------------- | ----------- |
| Interaction          | Complete    |
| Dialogue             | Complete    |
| Inventory            | Complete    |
| Time System          | Complete    |
| NPC Schedule System  | In Progress |
| Quest System         | Planned     |
| Vertical Slice Level | Planned     |

---

## Scope Note

This is an active vertical slice framework rather than a finished commercial game. Completed work focuses on reusable Unreal Engine C++ gameplay architecture. Future work will build quest progression, NPC scheduling, and content production on top of the existing framework.

---

Solo developed by **@ramagosumt**

# Amicus School of Engineering

### An Interactive Fiction Adventure | Unreal Engine 5.6 | C++ Focus

*"Lupus solitarius perit dum grex vivit."*

Follow 18-year-old Nathan Nguyen as he lives out his dream college experience at Amicus School of Engineering through club activities, relationships, and truths hidden beneath the school's surface.

---

# Programming Portfolio Notes

This repository is submitted as a programming portfolio sample focused on Unreal Engine 5.6 C++ gameplay architecture.

Developed independently as my bachelor thesis project, Amicus School of Engineering serves as a reusable gameplay systems framework for a larger interactive fiction RPG. While presented as a game prototype, the primary focus of this project is the design and implementation of extensible gameplay systems rather than content production.

For code reviewers, I recommend starting with the **Architecture Highlights** section and then using the **Code Review Guide** to navigate the source code.

---

# Why This Project

I selected this repository as my primary programming portfolio piece because it demonstrates my approach to gameplay architecture in Unreal Engine C++.

Rather than implementing isolated mechanics, I focused on building reusable systems that communicate through interfaces, delegates, components, and data-driven workflows. The project emphasizes extensibility, decoupling, maintainability, and long-term scalability.

Major systems personally designed and implemented include:

* Interface-driven interaction framework
* Data-driven branching dialogue architecture
* Multi-paradigm inventory framework (Grid, List, Slot)
* Shared item data and pickup pipeline
* Event-driven time simulation system
* Runtime UI generation
* Day/night cycle management
* Cross-system communication through delegates and interfaces

---

# Game Overview

Set in a fictional urban city, Amicus School of Engineering is a third-person interactive fiction adventure inspired by:

| System                    | Inspiration           |
| ------------------------- | --------------------- |
| World Density             | Yakuza 0              |
| Day Structure             | Persona 5             |
| School Life               | Bully                 |
| NPC Identity              | Watch Dogs 2          |
| Narrative Branching       | AI: The Somnium Files |
| Environmental Mystery     | Control               |
| Festivals & Relationships | Stardew Valley        |

Players explore a living college world, build relationships, complete quests, and gradually uncover the secrets hidden beneath everyday campus life.

**Target Platform:** PC

**Target Length:** 8-10 Hours

**Engine:** Unreal Engine 5.6

**Language:** C++

---

# Architecture Highlights

## Interface-Driven Interaction

`IInteractionInterface` fully decouples interactable actors from gameplay systems.

Adding a new interactable object requires implementing the interface only. No changes are required to existing gameplay code.

Benefits:

* No cast chains
* No concrete class dependencies
* Extensible by design
* Open for extension, closed for modification

---

## Data-Driven Systems

Dialogue and item definitions are driven through `UDataTable` assets.

Designers can create content without modifying C++ source code or Blueprint graphs.

Benefits:

* Fast iteration
* Separation of code and content
* Reduced maintenance cost

---

## Component-Based Design

Gameplay systems are implemented as reusable `UActorComponent` modules.

Examples:

* Interaction Components
* Dialogue Components
* Inventory Components
* Time Components

Benefits:

* Reusable behavior
* Improved modularity
* Reduced code duplication

---

## Event-Driven Time Simulation

The time system communicates entirely through multicast delegates.

Examples:

* OnNewMinute
* OnNewHour
* OnMorning
* OnEvening
* OnNewDay

Benefits:

* Loose coupling
* Reactive gameplay systems
* Simplified future expansion

---

## Clean Data / World Separation

World objects and inventory objects are intentionally separated.

### World Representation

* AActor Pickup Objects

### Inventory Representation

* UObject Item Instances
* BaseItemData Definitions

Benefits:

* Cleaner ownership model
* Easier serialization
* Better scalability

---

# Build Instructions

### Requirements

* Unreal Engine 5.6
* Visual Studio 2022

### Setup

1. Clone repository
2. Right-click `BachelorThesis.uproject`
3. Select **Generate Visual Studio Project Files**
4. Open solution
5. Build **Development Editor**
6. Launch `BachelorThesis.uproject`

---

# Code Review Guide

Recommended files for review are organized by system.

---

## Interaction System

### Files

* `Interfaces/InteractionInterface.h`
* `Components/BaseInteractableComponent.h`
* `Components/BaseInteractableComponent.cpp`
* `Components/TraceInteractionComponent.cpp`

### Concepts Demonstrated

* Interface-driven architecture
* Gameplay decoupling
* Extensible interaction workflows
* Reusable actor behavior

---

## Dialogue System

### Files

* `Components/DialogueComponent.h`
* `Components/DialogueComponent.cpp`
* `Data/DialogueData.h`
* `UIs/DialogueWidget.cpp`
* `UIs/DialogueChoiceWidget.cpp`

### Concepts Demonstrated

* Data-driven branching dialogue
* Runtime UI generation
* State-based conversation flow
* Cinematic mode integration

---

## Inventory System

### Files

* `Components/GridInventoryComponent.h`
* `Components/ListInventoryComponent.h`
* `Components/SlotInventoryComponent.h`
* `Data/BaseItemData.h`
* `Objects/BaseItem.h`
* `Actors/BasePickupItem.h`

### Concepts Demonstrated

* Object-oriented architecture
* Multiple inventory paradigms
* Shared item data layer
* Data/world separation
* Extensible inventory framework

---

## Time System

### Files

* `Components/CentralTimeComponent.h`
* `Components/CentralTimeComponent.cpp`
* `Actors/TimeManager.h`
* `Actors/TimeManager.cpp`
* `Frameworks/MainGameState.h`

### Concepts Demonstrated

* Event-driven simulation
* Delegate-based communication
* Global system coordination
* Day/night cycle management
* Time scaling and pausing

---

# Current Systems

| System               | Status         |
| -------------------- | -------------- |
| Interaction          | ✅ Complete     |
| Dialogue             | ✅ Complete     |
| Inventory            | ✅ Complete     |
| Time System          | ✅ Complete     |
| Quest System         | 🔄 In Progress |
| NPC Schedule System  | ⬜ Planned      |
| Vertical Slice Level | ⬜ Planned      |

---

# Scope Note

This project is an active gameplay framework and vertical slice prototype rather than a finished commercial game.

Completed systems focus on reusable, extensible Unreal Engine C++ architecture. Remaining development is centered on quest progression, NPC scheduling, and content creation built on top of the existing framework.

---

Solo developed by **@ramagosumt**

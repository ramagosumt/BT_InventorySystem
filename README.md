# Amicus School of Engineering
### An Interactive Fiction Adventure | Developed in Unreal Engine 5.6 - C++ Focus

> *"Lupus solitarius perit dum grex vivit."*

Follow 18-year-old Nathan Nguyen as he lives out his dream college experience at Amicus School of Engineering - through club activities, relationships, and truths hidden beneath the school's surface.

---

## Overview

Amicus School of Engineering is a third-person interactive fiction adventure inspired by Yakuza 0's dense world design, Persona 5's day structure, and Control's layered environmental storytelling. The player explores a living college world, builds relationships, completes quests, and gradually uncovers the secrets beneath everyday campus life.

This repository contains the core gameplay framework and vertical slice prototype, built entirely in Unreal Engine 5.6 with a C++ focus.

---

## Current Systems

### Interaction System
- Trace-based interaction with hold duration support
- Interface-driven architecture - any actor implementing `IInteractionInterface` is automatically interactable with zero changes to core systems
- Focus/unfocus visual feedback via custom depth rendering

### Dialogue System
- Data-table driven branching dialogue - no hardcoded conversation logic
- Full branching, variable row selection, and loop support
- Cinematic mode integration - pauses gameplay and UI during conversations
- Choice-based dialogue with dynamic widget generation

### Inventory System
- Three inventory types: Grid, List, and Slot - each independently functional
- Shared `BaseItemData` layer for normalized item identity across all inventory types
- Pickup actors cleanly separated from item data objects (world representation vs data)
- Journal and Quest item variants with dialogue integration on pickup

### Time System *(In Progress)*
- `CentralTimeComponent` hosted on `GameState` - globally accessible, no singleton abuse
- Minute-by-minute simulation with configurable day length and time scale
- Delegate broadcasts for `OnNewMinute`, `OnNewHour`, `OnNewDay` - any system can listen without coupling to time logic
- Day/night cycle driven by timeline curves on a dedicated `CentralTimeManager` actor
- `PauseTime` / `ResumeTime` support for dialogue and cinematic moments

### Quest System *(Coming Soon)*
- Data-driven quest structure with branching conditions
- Inventory and dialogue state integration
- Multi-objective support

### NPC Schedule System *(Coming Soon)*
- Time-window-based NPC availability
- Location locking during scheduled activities
- Foundation for relationship and routine systems

---

## Architecture Highlights

- **Interface-driven interaction** - `IInteractionInterface` decouples all interactable actors from core systems. Adding a new interactable type requires zero changes to existing code.
- **Data-table driven dialogue and items** - designers work in structured data tables, not C++ or Blueprint graphs.
- **Component-based design** - systems are composable components, not monolithic classes.
- **Clean separation of concerns** - world actors, data objects, UI widgets, and game systems are independently maintainable layers.

---

## Vertical Slice Goal

A single playable school day representing the core experience:
- Wake up and travel from dorm to class
- Attend class with an interactive task
- Explore campus and discover NPC schedules
- Interact with a key character, receive a quest
- Follow leads across campus locations
- End-of-day resolution

---

## Inspirations

| System | Inspiration |
|---|---|
| World density | Yakuza 0 |
| Day structure | Persona 5 |
| School life | Bully, Yandere Simulator |
| NPC identity | Watch Dogs 2 |
| Narrative branching | AI: The Somnium Files |
| Environmental mystery | Control |
| Festivals and relationships | Stardew Valley |

---

## Technical Stack

- **Engine:** Unreal Engine 5.6
- **Language:** C++ (primary), Blueprint (prototyping only)
- **Platform Target:** PC (Steam, Itch.io)
- **Development:** Solo

---

## Development Status

Active development. Currently building toward a vertical slice prototype.

| System | Status |
|---|---|
| Interaction | ✅ Complete |
| Dialogue | ✅ Complete |
| Inventory | ✅ Complete |
| Time System | 🔄 In Progress |
| Quest System | ⬜ Planned |
| NPC Schedules | ⬜ Planned |
| Vertical Slice Level | ⬜ Planned |

---

*Solo developed by [@ramagosumt](https://github.com/ramagosumt)*

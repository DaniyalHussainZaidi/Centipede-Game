# Centipede-Game
This project is a technical re-implementation of Atari’s classic Centipede (1981) arcade shooter. The game operates on a 2D grid-based environment and simulates real time entity movement, collision detection, projectile dynamics, and level progression using structured programming techniques.

# Core Gameplay Mechanics

The game world is represented as a multi-dimensional array, defining spatial positions for all objects (fighter, centipede, mushrooms, creatures, projectiles).

The player controls a fighter constrained to the bottom 5 rows of the grid, with discrete step-based movement via keyboard input.

The centipede consists of 12 independently tracked segments at Level 1 and moves horizontally across the grid, descending one row upon reaching mushrooms or screen edges.

Laser firing is implemented with pixel-level vertical traversal, updating the game state each frame.

Collision handling includes:

Splitting centipede segments upon laser impact

Dynamic head regeneration based on difficulty level

Mushroom state transition (intact → damaged → destroyed)

Poisonous mushroom generation

Centipede descent, speed, and head behavior change dynamically across levels.

# Game Entities

Fighter: Player-controlled unit with restricted movement zone and projectile ability.

Centipede: Segment-based enemy with autonomous movement, real-time splitting, and head reassignment rules.

Mushrooms: Static grid obstacles requiring two hits to destroy; influence centipede descent.

Poisonous Mushrooms: Generated upon incorrect centipede hits; lethal upon contact.

Fleas: Vertical movement, mushroom spawning

Spiders: Random zig-zag motion in player region

Scorpions: Lateral motion; convert mushrooms into poisonous variants

# Level Progression

Level 1 begins with a 12-segment centipede and 20–30 mushrooms placed randomly.

Each subsequent level increases:

Centipede movement speed (doubles per level)

Mushroom density (+20%)

Number of independent fast-moving head segments

Higher levels introduce complex scenarios such as mushroom-saturated upper grids and accelerated descent paths.

# Scoring System

Destroy Mushroom: 1 point

Centipede Body Segment: 10 points

Centipede Head: 20 points

Bonus Scoring (optional features):

Regenerated mushrooms: 5 points

Spiders: 300 / 600 / 900 points (proximity-based)

Scorpions: 1000 points

Extra lives awarded at specific score thresholds (up to 6 total).

# Technical Requirements

Students must work with the provided skeleton code, extending it with proper game logic, movement algorithms, and procedural decomposition.

Emphasis on:

Clean code structure

Meaningful identifiers

Consistent formatting

Modular design and function reuse

Robust error handling for invalid input and boundary conditions.

Entire implementation must adhere to course integrity rules.

# Objective

Develop a fully functional, grid-based arcade shooter through structured programming, demonstrating proficiency in arrays, control flow, state management, and multi-entity interaction.

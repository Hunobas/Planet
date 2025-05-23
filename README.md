# Save The Earth! Without Keyboard (in production)

![Untitledvideo-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/da3ba9e3-a6de-4619-a9be-64dce96ffd9b)

![image](https://github.com/user-attachments/assets/416d7b18-5f91-43bd-9432-6e73abd8ea92)

# About
Unreal 3D TPS shooter game inspired by Vampire Survivors.

- [Go To Play](https://drive.google.com/drive/folders/1Eu-mHCERa7LGEWnDuUfecRwAdXO3PLlG?usp=sharing)
- [Play Demo](https://www.youtube.com/watch?v=y_v1nA_0aPI&ab_channel=%ED%9B%84%EB%85%B8%EB%B0%94%EC%8A%A4)

### Features
- 1 playable **Planet**
- 1 **Mainweapon**
- 1 **Subweapon**
- Rotation Status special gimmick: accumulates every time player camera moves and triggers any secondary attack
- 3 unique **Enemy Types**

### More to come features
- 1 more playable **Planet**
- 1 more **Mainweapon**
- 5 more **Subweapon**
- 3 more **Bosses** with 3 more **Stages**
- 1 more **enemy type**
- many many kinds of **Passive Items**

# Controls
### USE MOUSE ONLY

# Game System

### Planet Torque

![Untitledvideo1-ezgif com-video-to-gif-converter (1)](https://github.com/user-attachments/assets/d6f363f7-b987-4a16-8ce2-d919cd6fbc3f)

In-game stats that accumulate based on mouse input.
This stat cycles between values from 0 to 360 while accumulating, and you can control or trigger the firing of a weapon linked to this stat.

### Enemy Wave

![Untitledvideo2-ezgif com-video-to-gif-converter (1)](https://github.com/user-attachments/assets/dd68c407-825d-426a-b3ce-9d28def10dcd)

**Celestial Body** Actor: An actor attached to the player, creating spawn point components on a spherical virtual surface where enemies are spawned.

**Field Score** and **Spawnable Enemy List**: Small waves of enemies are spawned every 5 seconds, with each enemy having an individual field score. When the total field score, **Current Field Score**, exceeds **Max Field Score**, the spawning of small waves is designed to stop. The **Max Field Score** increases over time following a sigmoid function. Additionally, as in-game time progresses, enemies with higher field scores are added to the **Spawnable Enemy List**, while those with lower field scores are removed.

**Threatening Waves**: Large waves are summoned every 1 minute to keep the player on edge. The summoned large waves also affect the **Current Field Score**.

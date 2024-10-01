# Intermidiate AI

### How the program works : 

Blue attacks Green
<br>
Red attacks Blue
<br>
Green attacks Red

Hold "c" to have the boids attracted to the mouse,
While holding "c", hold the "spacebar" to have them avoid the mouse.

![](https://github.com/Loris-Moreau/Raylib-Project/blob/Intermidiate-AI/Images/Boids_Sim_10-01-2024.gif "Flock vid")

### Boids : 

Boids is an artificial life program that produces highly realistic simulations of the flocking behaviour of birds. 
Each "boid" (an abbreviation of "bird-oid object") follows a straightforward set of rules, which can be summarised as follows :

- **Separation** : boids move away from other boids that are too close

  ![Flock](https://github.com/Loris-Moreau/Raylib-Project/blob/Intermidiate-AI/Images/flock.png "Flock Principle")

  
- **Alignment** : boids attempt to match the velocities of their neighbors

  ![Alignement](https://github.com/Loris-Moreau/Raylib-Project/blob/Intermidiate-AI/Images/alignment.png "Alignement")

  
- **Cohesion** : boids move toward the center of mass of their neighbors

  ![Cohesion](https://github.com/Loris-Moreau/Raylib-Project/blob/Intermidiate-AI/Images/cohesion.png "Cohesion")


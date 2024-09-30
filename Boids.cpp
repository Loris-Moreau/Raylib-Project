#include "Boids.h"

void Boids::applyRules(const std::vector<Boids>& flock, const std::vector<Obstacle>& obstacles, 
                    float minDistance, float alignmentFactor, float cohesionFactor, 
                    float maxSpeed, const Vector2& boundsMin, const Vector2& boundsMax)
{
    Vector2 separation = {0, 0};
    Vector2 alignment = {0, 0};
    Vector2 cohesion = {0, 0};
    int neighborCount = 0;

    for (const auto& other : flock)
    {
        if (&other == this) continue; // Skip the current boid (itself)

        Vector2 difference = Vector2Subtract(position, other.position);
        float distance = Vector2Length(difference);

        if (distance < minDistance && distance > 0)
        {
            // Separation rule: move away from nearby boids
            Vector2 normDiff = Vector2Normalize(difference);
            separation = Vector2Add(separation, Vector2Scale(normDiff, (minDistance - distance) * 0.5f)); // Increased separation force
        }

        if (distance > 0 && distance < minDistance * 5)
        {
            // Alignment: Adjust velocity to match nearby boids
            alignment = Vector2Add(alignment, other.velocity);

            // Cohesion: Move towards the center of mass of nearby boids
            cohesion = Vector2Add(cohesion, other.position);
            neighborCount++;
        }
    }

    if (neighborCount > 0)
    {
        // Average the alignment and cohesion influences
        alignment = Vector2Scale(alignment, 1.0f / neighborCount);
        alignment = Vector2Scale(Vector2Normalize(alignment), alignmentFactor);

        cohesion = Vector2Scale(cohesion, 1.0f / neighborCount);
        Vector2 cohesionForce = Vector2Subtract(cohesion, position);
        cohesion = Vector2Scale(Vector2Normalize(cohesionForce), cohesionFactor);
    }

    // Add obstacle avoidance
    for (const auto& obstacle : obstacles)
    {
        // Check if the boid is colliding with the rectangular obstacle
        Rectangle obstacleRect = {obstacle.position.x, obstacle.position.y, static_cast<float>(obstacle.size_x), static_cast<float>(obstacle.size_y)};
        
        // Check for collision between boid (circle) and obstacle (rectangle)
        if (CheckCollisionCircleRec(position, minDistance/3, obstacleRect))
        {
            // Calculate the vector from the boid to the closest point on the obstacle's boundary
            Vector2 obstacleCenter = {obstacle.position.x + obstacle.size_x / 2.0f, obstacle.position.y + obstacle.size_y / 2.0f};
            Vector2 obstacleVec = Vector2Subtract(position, obstacleCenter);
            
            // Normalize the vector to create a repulsion force
            Vector2 normObstacleVec = Vector2Normalize(obstacleVec);
            
            // Calculate the repulsion strength based on proximity
            float repulsionStrength = minDistance;  // You can scale this value to adjust repulsion strength
            separation = Vector2Add(separation, Vector2Scale(normObstacleVec, repulsionStrength));
        }
    }

    // Update velocity based on the three rules and obstacle avoidance
    velocity = Vector2Add(velocity, separation);
    velocity = Vector2Add(velocity, alignment);
    velocity = Vector2Add(velocity, cohesion);

    // Limit velocity to maxSpeed
    if (Vector2Length(velocity) > maxSpeed)
    {
        velocity = Vector2Scale(Vector2Normalize(velocity), maxSpeed);
    }

    // Handle boundary conditions (bounce back when hitting the walls)
    checkBoundaries(boundsMin, boundsMax);
}


    void Boids::checkBoundaries(const Vector2& boundsMin, const Vector2& boundsMax)
    {
        if (position.x < boundsMin.x || position.x > boundsMax.x)
        {
            velocity.x *= -1; // Reverse x direction
        }
        if (position.y < boundsMin.y || position.y > boundsMax.y)
        {
            velocity.y *= -1; // Reverse y direction
        }
    }

    void Boids::updatePosition()
    {
        position = Vector2Add(position, velocity);
    }

    void Boids::DrawBoid() const
    {
        // Draw the boid as a circle in raylib
        DrawCircleV(position, 5, BLUE); // Circle with radius 5 for each boid
    }


// Function to simulate a step in the simulation
void Boids::simulateStep(std::vector<Boids>& flock, const std::vector<Obstacle>& obstacles,
                  float minDistance, float alignmentFactor, float cohesionFactor, float maxSpeed,
                  const Vector2& boundsMin, const Vector2& boundsMax)
{
    for (auto& boid : flock)
    {
        boid.applyRules(flock, obstacles, minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax);
    }

    for (auto& boid : flock)
    {
        boid.updatePosition();
    }
}
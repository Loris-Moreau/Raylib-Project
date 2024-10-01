#include "Boids.h"

void Boids::applyRules( const std::vector<Boids>& flock,
                        const std::vector<Obstacle>& obstacles,
                        float minDistance,
                        float alignmentFactor,
                        float cohesionFactor,
                        float maxSpeed,
                        const Vector2& boundsMin,
                        const Vector2& boundsMax,
                        boidType predatorType,
                        float predatorAvoidFactor,
                        float preyAttractFactor
                       )
{
    Vector2 separation = {0, 0};
    Vector2 alignment = {0, 0};
    Vector2 cohesion = {0, 0};
    Vector2 predatorAvoidance = {0, 0};
    Vector2 preyAttraction = {0, 0};
    int neighborCount = 0;

    for (const Boids& other : flock)
    {
        if (&other == this) continue; // Skip self

        const Vector2 difference = Vector2Subtract(position, other.position);
        const float distance = Vector2Length(difference);
        
        if (distance < minDistance && distance > 0)
        {
            // Separation rule : move away from nearby boids
            const Vector2 normDiff = Vector2Normalize(difference);
            separation = Vector2Add(separation, Vector2Scale(normDiff, (minDistance - distance) * separationFactor)); 
        }

        if (distance > 0 && distance < minDistance * 5)
        {
            // Alignment : Adjust velocity to match nearby boids
            alignment = Vector2Add(alignment, other.velocity);

            // Cohesion : Move towards the center of mass of nearby boids
            cohesion = Vector2Add(cohesion, other.position);
            neighborCount++;
            
            // Boids follow or avoid mouse
            bool mouseActive = false;
            if(mouseActive)
            {
                float mouse_x = float(GetMouseX());
                float mouse_y = float(GetMouseY());
                Vector2 mPos = Vector2{mouse_x, mouse_y};

                bool followMouse = true;
                if(followMouse)
                {
                    // Ha, they think I'm one of them
                    cohesion = Vector2Add(cohesion, mPos);
                }
                else
                {
                    // Ah, they found out
                    separation = Vector2Add(separation, mPos);
                }
            }
            // Predator avoidance logic : if this boid has a predator nearby, avoid it
            if (other.type == predatorType && distance < minDistance * 3)
            {
                Vector2 predatorDiff = Vector2Normalize(difference);
                predatorAvoidance = Vector2Add(predatorAvoidance, Vector2Scale(predatorDiff, predatorAvoidFactor));
            }

            // Prey attraction logic : chase prey
            if (type == predatorType && distance < minDistance * 3)
            {
                Vector2 preyDiff = Vector2Normalize(Vector2Subtract(other.position, position));
                preyAttraction = Vector2Add(preyAttraction, Vector2Scale(preyDiff, preyAttractFactor));
            }
        }
    }
    
    if (neighborCount > 0)
    {
        // Average the alignment and cohesion influences
        alignment = Vector2Scale(alignment, 1.0f / float(neighborCount));
        alignment = Vector2Scale(Vector2Normalize(alignment), alignmentFactor);

        cohesion = Vector2Scale(cohesion, 1.0f / float(neighborCount));
        const Vector2 cohesionForce = Vector2Subtract(cohesion, position);
        cohesion = Vector2Scale(Vector2Normalize(cohesionForce), cohesionFactor);
    }
    
    // Add obstacle avoidance
    for (const auto& obstacle : obstacles)
    {
        // Check if the boid is colliding with the rectangular obstacle
        const Rectangle obstacleRect = {obstacle.position.x, obstacle.position.y, float(obstacle.size_x), float(obstacle.size_y)};
        
        // Check for collision between boid (circle) and obstacle (rectangle)
        if (CheckCollisionCircleRec(position, radius * 3, obstacleRect))
        {
            // Calculate the vector from the boid to the closest point on the obstacle's boundary
            const Vector2 obstacleEdge = {obstacle.position.x + float(obstacle.size_x) / 2.0f, obstacle.position.y + float(obstacle.size_y) / 2.0f};
            const Vector2 obstacleVec = Vector2Subtract(position, obstacleEdge);
            
            // Normalize the vector to create a repulsion force
            const Vector2 normObstacleV = Vector2Normalize(obstacleVec);
            
            // Calculate the repulsion strength based on proximity
            separation = Vector2Add(separation, Vector2Scale(normObstacleV, repulsionStrength));
        }
    }

    // Update velocity based on the three rules and obstacle avoidance
    velocity = Vector2Add(velocity, separation);
    velocity = Vector2Add(velocity, alignment);
    velocity = Vector2Add(velocity, cohesion);
    velocity = Vector2Add(velocity, predatorAvoidance);
    velocity = Vector2Add(velocity, preyAttraction);

    // Limit velocity to maxSpeed
    if (Vector2Length(velocity) > maxSpeed)
    {
        velocity = Vector2Scale(Vector2Normalize(velocity), maxSpeed);
    }

    // Handle boundary conditions (bounce back when hitting the walls)
    checkBoundaries(boundsMin, boundsMax);
}

void Boids::checkBoundaries(const Vector2& boundsMin, const Vector2& boundsMax) // TODO need to fix, collision against border are bad
{
    if (position.x < boundsMin.x || position.x > boundsMax.x)
    {
        velocity.x *= -1; 
    }
    if (position.y < boundsMin.y || position.y > boundsMax.y)
    {
        velocity.y *= -1; 
    }
}

void Boids::updatePosition()
{
    position = Vector2Add(position, velocity);
}

void Boids::DrawBoid() const
{
    DrawCircleV(position, radius, color);
}

// Function to simulate a step in the simulation
void Boids::simulateStep( std::vector<Boids>& flock,
                          const std::vector<Obstacle>& obstacles,
                          float minDistance,
                          float alignmentFactor,
                          float cohesionFactor,
                          float maxSpeed,
                          const Vector2& boundsMin,
                          const Vector2& boundsMax
                         )
{
    for (auto& boid : flock)
    {
        // predator & prey types for each group
        boidType predatorType;
        float predatorAvoidFactor, preyAttractFactor;
        
        if (boid.type == blue)
        {
            predatorType = red;
            predatorAvoidFactor = 0.5f; // High avoidance
            preyAttractFactor = 0.25f;   // Low attraction
        }
        else if (boid.type == red)
        {
            predatorType = green;
            predatorAvoidFactor = 0.25f; // Low avoidance
            preyAttractFactor = 0.5f;   // High attraction
        }
        else // (boid.type == green)
        {
            predatorType = blue;
            predatorAvoidFactor = 0.5f; // Balanced avoidance
            preyAttractFactor = 0.5f;   // Balanced attraction
        }
        
        boid.applyRules(flock, obstacles, minDistance, alignmentFactor, cohesionFactor, maxSpeed, boundsMin, boundsMax, predatorType, predatorAvoidFactor, preyAttractFactor);
    }

    for (auto& boid : flock)
    {
        boid.updatePosition();
    }
}

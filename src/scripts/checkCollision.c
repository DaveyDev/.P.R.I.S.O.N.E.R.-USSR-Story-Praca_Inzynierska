
bool checkCollision(Rectangle rect1, Rectangle rect2) {
    // Check if any of the sides from rect1 are outside rect2
    if (rect1.x + rect1.width < rect2.x || rect1.x > rect2.x + rect2.width ||
        rect1.y + rect1.height < rect2.y || rect1.y > rect2.y + rect2.height) {
        // No collision
        return false;
    }

    // Collided
    return true;
}